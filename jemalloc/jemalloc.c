#include "arena.h"



/* Tree of extents.  Use accessor functions for en_* fields. */
struct extent_node_s {
	/* Arena from which this extent came, if any. */
	arena_t			*en_arena;

	/* Pointer to the extent that this tree node is responsible for. */
	void			*en_addr;

	/* Total region size. */
	size_t			en_size;

	/*
	 * The zeroed flag is used by chunk recycling code to track whether
	 * memory is zero-filled.
	 */
	bool			en_zeroed;

	/*
	 * True if physical memory is committed to the extent, whether
	 * explicitly or implicitly as on a system that overcommits and
	 * satisfies physical memory needs on demand via soft page faults.
	 */
	bool			en_committed;

	/*
	 * The achunk flag is used to validate that huge allocation lookups
	 * don't return arena chunks.
	 */
	bool			en_achunk;

	/* Profile counters, used for huge objects. */
	prof_tctx_t		*en_prof_tctx;

	/* Linkage for arena's runs_dirty and chunks_cache rings. */
	arena_runs_dirty_link_t	rd;
	struct {
		extent_node_t *qre_next;
		extent_node_t *qre_prev;
	} cc_link;

	union {
		/* Linkage for the size/address-ordered tree. */
		struct {
			extent_node_t *rbn_left;
			extent_node_t *rbn_right_red;
		} szad_link;

		/* Linkage for arena's huge and node_cache lists. */
		struct {
			extent_node_t *qre_next;
			extent_node_t *qre_prev;
		} ql_link;
	};

	/* Linkage for the address-ordered tree. */
	struct {
		extent_node_t *rbn_left;
		extent_node_t *rbn_right_red;
	} ad_link;
};

typedef struct extent_node_s extent_node_t;

typedef struct {
	extent_node_t *rbt_root;
	extent_node_t rbt_nil;
} extent_tree_t;