

typedef struct tsd_s tsd_t;


#define	MALLOC_TSD							\
/*  O(name,			type) */				\
    O(tcache,			tcache_t *)				\
    O(thread_allocated,		uint64_t)				\
    O(thread_deallocated,	uint64_t)				\
    O(prof_tdata,		prof_tdata_t *)				\
    O(arena,			arena_t *)				\
    O(arenas_cache,		arena_t **)				\
    O(narenas_cache,		unsigned)				\
    O(arenas_cache_bypass,	bool)					\
    O(tcache_enabled,		tcache_enabled_t)			\
    O(quarantine,		quarantine_t *)				\

struct tsd_s {
	tsd_state_t	state; // enum
#define	O(n, t)								\
	t		n;
MALLOC_TSD
#undef O
};