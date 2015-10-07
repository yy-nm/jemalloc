#include "arena_stats.h"
#include "tcache.h"
#include "prof.h"

typedef struct arena_s arena_t;
typedef struct arena_chunk_s arena_chunk_t;
typedef struct arena_chunk_map_bits_s arena_chunk_map_bits_t;
typedef struct arena_chunk_map_misc_s arena_chunk_map_misc_t;
typedef struct arena_runs_dirty_link_s arena_runs_dirty_link_t;
typedef struct arena_run_s arena_run_t;
typedef struct arena_bin_s arena_bin_t;

typedef unsigned long bitmap_t;


struct arena_bin_s {
	/*
	 * All operations on runcur, runs, and stats require that lock be
	 * locked.  Run allocation/deallocation are protected by the arena lock,
	 * which may be acquired while holding one or more bin locks, but not
	 * vise versa.
	 */
	malloc_mutex_t	lock;

	/*
	 * Current run being used to service allocations of this bin's size
	 * class.
	 */
	arena_run_t	*runcur;

	/*
	 * Tree of non-full runs.  This tree is used when looking for an
	 * existing run when runcur is no longer usable.  We choose the
	 * non-full run that is lowest in memory; this policy tends to keep
	 * objects packed well, and it can also help reduce the number of
	 * almost-empty chunks.
	 */
	 // sizeof(arena_run_tree_t) = 32 + 8
	arena_run_tree_t runs;

	/* Bin statistics. */
	// sizeof(malloc_bin_stats_t) = 9 * 8
	malloc_bin_stats_t stats;
};

// sizeof(arena_run_s) = 16
struct arena_run_s {
	/* Index of bin this run is associated with. */
	// szind_t = unsigned
	szind_t		binind;

	/* Number of free regions in run. */
	unsigned	nfree;

	/* Per region allocated/deallocated bitmap. */
	// BITMAP_GROUPS_MAX = 1
	// bitmap_t = unsigned long
	bitmap_t	bitmap[BITMAP_GROUPS_MAX];
};

struct arena_runs_dirty_link_s {
	struct {
		arena_runs_dirty_link_t *qre_next;
		arena_runs_dirty_link_t *qre_next;
	} rd_link;
};

/*
 * Each arena_chunk_map_misc_t corresponds to one page within the chunk, just
 * like arena_chunk_map_bits_t.  Two separate arrays are stored within each
 * chunk header in order to improve cache locality.
 */
 // sizeof(arena_chunk_map_misc_s) = 32
struct arena_chunk_map_misc_s {
	/*
	 * Linkage for run trees.  There are two disjoint uses:
	 *
	 * 1) arena_t's runs_avail tree.
	 * 2) arena_run_t conceptually uses this linkage for in-use non-full
	 *    runs, rather than directly embedding linkage.
	 */
	 // union = 16
	union {
		/* Linkage for list of dirty runs. */
		arena_runs_dirty_link_t		rd;

		/* Profile counters, used for large object runs. */
		union {
			void				*prof_tctx_pun;
			prof_tctx_t			*prof_tctx;
		};

		/* Small region run metadata. */
		arena_run_t			run;
	};
    struct {
    	arena_chunk_map_misc_t *rbn_left;
    	arena_chunk_map_misc_t *rbn_right_red;
    } rb_link;
};

typedef struct {
	arena_chunk_map_misc_t *rbt_root;
	arena_chunk_map_misc_t rbt_nil;
} arena_avail_tree_t;

typedef struct {
	arena_chunk_map_misc_t *rbt_root;
	arena_chunk_map_misc_t rbt_nil;
} arena_run_tree_t;

/* Each element of the chunk map corresponds to one page within the chunk. */
struct arena_chunk_map_bits_s {
	/*
	 * Run address (or size) and various flags are stored together.  The bit
	 * layout looks like (assuming 32-bit system):
	 *
	 *   ???????? ???????? ???nnnnn nnndumla
	 *
	 * ? : Unallocated: Run address for first/last pages, unset for internal
	 *                  pages.
	 *     Small: Run page offset.
	 *     Large: Run page count for first page, unset for trailing pages.
	 * n : binind for small size class, BININD_INVALID for large size class.
	 * d : dirty?
	 * u : unzeroed?
	 * m : decommitted?
	 * l : large?
	 * a : allocated?
	 *
	 * Following are example bit patterns for the three types of runs.
	 *
	 * p : run page offset
	 * s : run size
	 * n : binind for size class; large objects set these to BININD_INVALID
	 * x : don't care
	 * - : 0
	 * + : 1
	 * [DUMLA] : bit set
	 * [dumla] : bit unset
	 *
	 *   Unallocated (clean):
	 *     ssssssss ssssssss sss+++++ +++dum-a
	 *     xxxxxxxx xxxxxxxx xxxxxxxx xxx-Uxxx
	 *     ssssssss ssssssss sss+++++ +++dUm-a
	 *
	 *   Unallocated (dirty):
	 *     ssssssss ssssssss sss+++++ +++D-m-a
	 *     xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
	 *     ssssssss ssssssss sss+++++ +++D-m-a
	 *
	 *   Small:
	 *     pppppppp pppppppp pppnnnnn nnnd---A
	 *     pppppppp pppppppp pppnnnnn nnn----A
	 *     pppppppp pppppppp pppnnnnn nnnd---A
	 *
	 *   Large:
	 *     ssssssss ssssssss sss+++++ +++D--LA
	 *     xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
	 *     -------- -------- ---+++++ +++D--LA
	 *
	 *   Large (sampled, size <= LARGE_MINCLASS):
	 *     ssssssss ssssssss sssnnnnn nnnD--LA
	 *     xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
	 *     -------- -------- ---+++++ +++D--LA
	 *
	 *   Large (not sampled, size == LARGE_MINCLASS):
	 *     ssssssss ssssssss sss+++++ +++D--LA
	 *     xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
	 *     -------- -------- ---+++++ +++D--LA
	 */
	size_t				bits;
	#define	CHUNK_MAP_ALLOCATED	((size_t)0x01U)
	#define	CHUNK_MAP_LARGE		((size_t)0x02U)
	#define	CHUNK_MAP_STATE_MASK	((size_t)0x3U)

	#define	CHUNK_MAP_DECOMMITTED	((size_t)0x04U)
	#define	CHUNK_MAP_UNZEROED	((size_t)0x08U)
	#define	CHUNK_MAP_DIRTY		((size_t)0x10U)
	#define	CHUNK_MAP_FLAGS_MASK	((size_t)0x1cU)

	#define	CHUNK_MAP_BININD_SHIFT	5
	#define	BININD_INVALID		((size_t)0xffU)
	#define	CHUNK_MAP_BININD_MASK	(BININD_INVALID << CHUNK_MAP_BININD_SHIFT)
	#define	CHUNK_MAP_BININD_INVALID CHUNK_MAP_BININD_MASK

	#define	CHUNK_MAP_RUNIND_SHIFT	(CHUNK_MAP_BININD_SHIFT + 8)
	#define	CHUNK_MAP_SIZE_SHIFT	(CHUNK_MAP_RUNIND_SHIFT - LG_PAGE)
	#define	CHUNK_MAP_SIZE_MASK						\
    (~(CHUNK_MAP_BININD_MASK | CHUNK_MAP_FLAGS_MASK | CHUNK_MAP_STATE_MASK))
};

struct arena_chunk_s {
	/*
	 * A pointer to the arena that owns the chunk is stored within the node.
	 * This field as a whole is used by chunks_rtree to support both
	 * ivsalloc() and core-based debugging.
	 */
	extent_node_t		node;

	/*
	 * Map of pages within chunk that keeps track of free/large/small.  The
	 * first map_bias entries are omitted, since the chunk header does not
	 * need to be tracked in the map.  This omission saves a header page
	 * for common chunk sizes (e.g. 4 MiB).
	 */
	arena_chunk_map_bits_t	map_bits[1]; /* Dynamically sized. */
};

struct arena_s {
	/* This arena's index within the arenas array. */
	unsigned		ind;

	/*
	 * Number of threads currently assigned to this arena.  This field is
	 * protected by arenas_lock.
	 */
	unsigned		nthreads;

	/*
	 * There are three classes of arena operations from a locking
	 * perspective:
	 * 1) Thread assignment (modifies nthreads) is protected by arenas_lock.
	 * 2) Bin-related operations are protected by bin locks.
	 * 3) Chunk- and run-related operations are protected by this mutex.
	 */
	// pthread_mutex_t
	malloc_mutex_t		lock;

	arena_stats_t		stats;
	/*
	 * List of tcaches for extant threads associated with this arena.
	 * Stats from these are merged incrementally, and at exit if
	 * opt_stats_print is enabled.
	 */
	struct {
		tcache_t *qlh_first;
	} tcache_ql;

	uint64_t		prof_accumbytes;

	/*
	 * PRNG state for cache index randomization of large allocation base
	 * pointers.
	 */
	uint64_t		offset_state;

	//enum sizeof(enum) = 4
	dss_prec_t		dss_prec;

	/*
	 * In order to avoid rapid chunk allocation/deallocation when an arena
	 * oscillates right on the cusp of needing a new chunk, cache the most
	 * recently freed chunk.  The spare is left in the arena's chunk trees
	 * until it is deleted.
	 *
	 * There is one spare chunk per arena, rather than one spare total, in
	 * order to avoid interactions between multiple threads that could make
	 * a single spare inadequate.
	 */
	arena_chunk_t		*spare;

	/* Minimum ratio (log base 2) of nactive:ndirty. */
	ssize_t			lg_dirty_mult;

	/* True if a thread is currently executing arena_purge(). */
	bool			purging;

	/* Number of pages in active runs and huge regions. */
	size_t			nactive;

	/*
	 * Current count of pages within unused runs that are potentially
	 * dirty, and for which madvise(... MADV_DONTNEED) has not been called.
	 * By tracking this, we can institute a limit on how much dirty unused
	 * memory is mapped for each arena.
	 */
	size_t			ndirty;

	/*
	 * Size/address-ordered tree of this arena's available runs.  The tree
	 * is used for first-best-fit run allocation.
	 */
	 // sizeof(arena_avail_tree_t) = 32 + 8 = 40
	arena_avail_tree_t	runs_avail;

	/*
	 * Unused dirty memory this arena manages.  Dirty memory is conceptually
	 * tracked as an arbitrarily interleaved LRU of dirty runs and cached
	 * chunks, but the list linkage is actually semi-duplicated in order to
	 * avoid extra arena_chunk_map_misc_t space overhead.
	 *
	 *   LRU-----------------------------------------------------------MRU
	 *
	 *        /-- arena ---\
	 *        |            |
	 *        |            |
	 *        |------------|                             /- chunk -\
	 *   ...->|chunks_cache|<--------------------------->|  /----\ |<--...
	 *        |------------|                             |  |node| |
	 *        |            |                             |  |    | |
	 *        |            |    /- run -\    /- run -\   |  |    | |
	 *        |            |    |       |    |       |   |  |    | |
	 *        |            |    |       |    |       |   |  |    | |
	 *        |------------|    |-------|    |-------|   |  |----| |
	 *   ...->|runs_dirty  |<-->|rd     |<-->|rd     |<---->|rd  |<----...
	 *        |------------|    |-------|    |-------|   |  |----| |
	 *        |            |    |       |    |       |   |  |    | |
	 *        |            |    |       |    |       |   |  \----/ |
	 *        |            |    \-------/    \-------/   |         |
	 *        |            |                             |         |
	 *        |            |                             |         |
	 *        \------------/                             \---------/
	 */
	 // sizeof(arena_runs_dirty_link_t) = 16
	arena_runs_dirty_link_t	runs_dirty;
	// sizeof(extent_node_t) = 104
	extent_node_t		chunks_cache;

	/* Extant huge allocations. */
	struct
	{
		extent_node_t *qlh_first;
	} huge;
	/* Synchronizes all huge allocation/update/deallocation. */
	// sizeof(malloc_mutex_t) = 40
	malloc_mutex_t		huge_mtx;

	/*
	 * Trees of chunks that were previously allocated (trees differ only in
	 * node ordering).  These are used when allocating chunks, in an attempt
	 * to re-use address space.  Depending on function, different tree
	 * orderings are needed, which is why there are two trees with the same
	 * contents.
	 */
	 // sizeof(extent_tree_t) = sizeof(extent_node_t) + 8
	extent_tree_t		chunks_szad_cached;
	extent_tree_t		chunks_ad_cached;
	extent_tree_t		chunks_szad_retained;
	extent_tree_t		chunks_ad_retained;

	malloc_mutex_t		chunks_mtx;
	/* Cache of nodes that were allocated via base_alloc(). */
	struct
	{
		extent_node_t *qlh_first;
	} node_cache;
	malloc_mutex_t		node_cache_mtx;

	// 回调函数
	/* User-configurable chunk hook functions. */
	// sizeof(chunk_hooks_t) = 7 * 8
	chunk_hooks_t		chunk_hooks;

	/* bins is used to store trees of free regions. */
	// sizeof(arena_bin_t) = 
	// NBINS = 36
	arena_bin_t		bins[NBINS];
};