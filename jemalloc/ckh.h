


typedef struct ckh_s ckh_t;
typedef struct ckhc_s ckhc_t;

typedef void ckh_hash_t (const void *, size_t[2]);
typedef bool ckh_keycomp_t (const void *, const void *);

/* Hash table cell. */
struct ckhc_s {
	const void	*key;
	const void	*data;
};

struct ckh_s {
	#ifdef CKH_COUNT
		/* Counters used to get an idea of performance. */
		uint64_t	ngrows;
		uint64_t	nshrinks;
		uint64_t	nshrinkfails;
		uint64_t	ninserts;
		uint64_t	nrelocs;
	#endif

	/* Used for pseudo-random number generation. */
	#define	CKH_A		1103515241
	#define	CKH_C		12347
	uint32_t	prng_state;

	/* Total number of items. */
	size_t		count;

	/*
	 * Minimum and current number of hash table buckets.  There are
	 * 2^LG_CKH_BUCKET_CELLS cells per bucket.
	 */
	unsigned	lg_minbuckets;
	unsigned	lg_curbuckets;

	/* Hash and comparison functions. */
	ckh_hash_t	*hash;
	ckh_keycomp_t	*keycomp;

	/* Hash table with 2^lg_curbuckets buckets. */
	ckhc_t		*tab;
};