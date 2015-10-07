/******************************************************************************/
#ifdef JEMALLOC_H_TYPES

/* Maximum bitmap bit count is 2^LG_BITMAP_MAXBITS. */
// LG_BITMAP_MAXBITS = 9
#define	LG_BITMAP_MAXBITS	LG_RUN_MAXREGS
// 2^9
#define	BITMAP_MAXBITS		(ZU(1) << LG_BITMAP_MAXBITS)

typedef struct bitmap_level_s bitmap_level_t;
typedef struct bitmap_info_s bitmap_info_t;
typedef unsigned long bitmap_t;
// LG_SIZEOF_BITMAP = LG_SIZEOF_LONG = 3
#define	LG_SIZEOF_BITMAP	LG_SIZEOF_LONG

/* Number of bits per group. */
// LG_BITMAP_GROUP_NBITS = 6
#define	LG_BITMAP_GROUP_NBITS		(LG_SIZEOF_BITMAP + 3)
// BITMAP_GROUP_NBITS = 64
#define	BITMAP_GROUP_NBITS		(ZU(1) << LG_BITMAP_GROUP_NBITS)
// BITMAP_GROUP_NBITS_MASK = 63
#define	BITMAP_GROUP_NBITS_MASK		(BITMAP_GROUP_NBITS-1)

/* Number of groups required to store a given number of bits. */
// 以 64 位为一组, 不足补 1
// 相当于 nbits / 64 + !!(nbits % 64)
#define	BITMAP_BITS2GROUPS(nbits)					\
    ((nbits + BITMAP_GROUP_NBITS_MASK) >> LG_BITMAP_GROUP_NBITS)

/*
 * Number of groups required at a particular level for a given number of bits.
 */
#define	BITMAP_GROUPS_L0(nbits)						\
    BITMAP_BITS2GROUPS(nbits)
#define	BITMAP_GROUPS_L1(nbits)						\
    BITMAP_BITS2GROUPS(BITMAP_BITS2GROUPS(nbits))
#define	BITMAP_GROUPS_L2(nbits)						\
    BITMAP_BITS2GROUPS(BITMAP_BITS2GROUPS(BITMAP_BITS2GROUPS((nbits))))
#define	BITMAP_GROUPS_L3(nbits)						\
    BITMAP_BITS2GROUPS(BITMAP_BITS2GROUPS(BITMAP_BITS2GROUPS(		\
	BITMAP_BITS2GROUPS((nbits)))))

/*
 * Assuming the number of levels, number of groups required for a given number
 * of bits.
 */
#define	BITMAP_GROUPS_1_LEVEL(nbits)					\
    BITMAP_GROUPS_L0(nbits)
#define	BITMAP_GROUPS_2_LEVEL(nbits)					\
    (BITMAP_GROUPS_1_LEVEL(nbits) + BITMAP_GROUPS_L1(nbits))
#define	BITMAP_GROUPS_3_LEVEL(nbits)					\
    (BITMAP_GROUPS_2_LEVEL(nbits) + BITMAP_GROUPS_L2(nbits))
#define	BITMAP_GROUPS_4_LEVEL(nbits)					\
    (BITMAP_GROUPS_3_LEVEL(nbits) + BITMAP_GROUPS_L3(nbits))

/*
 * Maximum number of groups required to support LG_BITMAP_MAXBITS.
 */
#if LG_BITMAP_MAXBITS <= LG_BITMAP_GROUP_NBITS

#elif LG_BITMAP_MAXBITS <= LG_BITMAP_GROUP_NBITS * 2
#  define BITMAP_GROUPS_MAX	BITMAP_GROUPS_2_LEVEL(BITMAP_MAXBITS)
#elif LG_BITMAP_MAXBITS <= LG_BITMAP_GROUP_NBITS * 3

#elif LG_BITMAP_MAXBITS <= LG_BITMAP_GROUP_NBITS * 4

#else
#  error "Unsupported bitmap size"
#endif

/* Maximum number of levels possible. */
// 9 / 3 + !! 0 = 3
#define	BITMAP_MAX_LEVELS						\
    (LG_BITMAP_MAXBITS / LG_SIZEOF_BITMAP)				\
    + !!(LG_BITMAP_MAXBITS % LG_SIZEOF_BITMAP)

#endif /* JEMALLOC_H_TYPES */
/******************************************************************************/
#ifdef JEMALLOC_H_STRUCTS

struct bitmap_level_s {
	/* Offset of this level's groups within the array of groups. */
	size_t group_offset;
};

struct bitmap_info_s {
	/* Logical number of bits in bitmap (stored at bottom level). */
	size_t nbits;

	/* Number of levels necessary for nbits. */
	unsigned nlevels;

	/*
	 * Only the first (nlevels+1) elements are used, and levels are ordered
	 * bottom to top (e.g. the bottom level is stored in levels[0]).
	 */
	 // BITMAP_MAX_LEVELS = 3
	bitmap_level_t levels[BITMAP_MAX_LEVELS+1];
};

#endif /* JEMALLOC_H_STRUCTS */
/******************************************************************************/
#ifdef JEMALLOC_H_EXTERNS

void	bitmap_info_init(bitmap_info_t *binfo, size_t nbits);
size_t	bitmap_info_ngroups(const bitmap_info_t *binfo);
size_t	bitmap_size(size_t nbits);
void	bitmap_init(bitmap_t *bitmap, const bitmap_info_t *binfo);

#endif /* JEMALLOC_H_EXTERNS */
/******************************************************************************/
#ifdef JEMALLOC_H_INLINES

#ifndef JEMALLOC_ENABLE_INLINE
bool	bitmap_full(bitmap_t *bitmap, const bitmap_info_t *binfo);
bool	bitmap_get(bitmap_t *bitmap, const bitmap_info_t *binfo, size_t bit);
void	bitmap_set(bitmap_t *bitmap, const bitmap_info_t *binfo, size_t bit);
size_t	bitmap_sfu(bitmap_t *bitmap, const bitmap_info_t *binfo);
void	bitmap_unset(bitmap_t *bitmap, const bitmap_info_t *binfo, size_t bit);
#endif

#if (defined(JEMALLOC_ENABLE_INLINE) || defined(JEMALLOC_BITMAP_C_))
JEMALLOC_INLINE bool
bitmap_full(bitmap_t *bitmap, const bitmap_info_t *binfo)
{
	unsigned rgoff = binfo->levels[binfo->nlevels].group_offset - 1;
	bitmap_t rg = bitmap[rgoff];
	/* The bitmap is full iff the root group is 0. */
	return (rg == 0);
}

JEMALLOC_INLINE bool
bitmap_get(bitmap_t *bitmap, const bitmap_info_t *binfo, size_t bit)
{
	size_t goff;
	bitmap_t g;

	assert(bit < binfo->nbits);
	goff = bit >> LG_BITMAP_GROUP_NBITS;
	g = bitmap[goff];
	return (!(g & (1LU << (bit & BITMAP_GROUP_NBITS_MASK))));
}

JEMALLOC_INLINE void
bitmap_set(bitmap_t *bitmap, const bitmap_info_t *binfo, size_t bit)
{
	size_t goff;
	bitmap_t *gp;
	bitmap_t g;

	assert(bit < binfo->nbits);
	assert(!bitmap_get(bitmap, binfo, bit));
	goff = bit >> LG_BITMAP_GROUP_NBITS;
	gp = &bitmap[goff];
	g = *gp;
	assert(g & (1LU << (bit & BITMAP_GROUP_NBITS_MASK)));
	g ^= 1LU << (bit & BITMAP_GROUP_NBITS_MASK);
	*gp = g;
	assert(bitmap_get(bitmap, binfo, bit));
	/* Propagate group state transitions up the tree. */
	if (g == 0) {
		unsigned i;
		for (i = 1; i < binfo->nlevels; i++) {
			bit = goff;
			goff = bit >> LG_BITMAP_GROUP_NBITS;
			gp = &bitmap[binfo->levels[i].group_offset + goff];
			g = *gp;
			assert(g & (1LU << (bit & BITMAP_GROUP_NBITS_MASK)));
			g ^= 1LU << (bit & BITMAP_GROUP_NBITS_MASK);
			*gp = g;
			if (g != 0)
				break;
		}
	}
}

/* sfu: set first unset. */
JEMALLOC_INLINE size_t
bitmap_sfu(bitmap_t *bitmap, const bitmap_info_t *binfo)
{
	size_t bit;
	bitmap_t g;
	unsigned i;

	assert(!bitmap_full(bitmap, binfo));

	i = binfo->nlevels - 1;
	g = bitmap[binfo->levels[i].group_offset];
	bit = jemalloc_ffsl(g) - 1;
	while (i > 0) {
		i--;
		g = bitmap[binfo->levels[i].group_offset + bit];
		bit = (bit << LG_BITMAP_GROUP_NBITS) + (jemalloc_ffsl(g) - 1);
	}

	bitmap_set(bitmap, binfo, bit);
	return (bit);
}

JEMALLOC_INLINE void
bitmap_unset(bitmap_t *bitmap, const bitmap_info_t *binfo, size_t bit)
{
	size_t goff;
	bitmap_t *gp;
	bitmap_t g;
	bool propagate;

	assert(bit < binfo->nbits);
	assert(bitmap_get(bitmap, binfo, bit));
	goff = bit >> LG_BITMAP_GROUP_NBITS;
	gp = &bitmap[goff];
	g = *gp;
	propagate = (g == 0);
	assert((g & (1LU << (bit & BITMAP_GROUP_NBITS_MASK))) == 0);
	g ^= 1LU << (bit & BITMAP_GROUP_NBITS_MASK);
	*gp = g;
	assert(!bitmap_get(bitmap, binfo, bit));
	/* Propagate group state transitions up the tree. */
	if (propagate) {
		unsigned i;
		for (i = 1; i < binfo->nlevels; i++) {
			bit = goff;
			goff = bit >> LG_BITMAP_GROUP_NBITS;
			gp = &bitmap[binfo->levels[i].group_offset + goff];
			g = *gp;
			propagate = (g == 0);
			assert((g & (1LU << (bit & BITMAP_GROUP_NBITS_MASK)))
			    == 0);
			g ^= 1LU << (bit & BITMAP_GROUP_NBITS_MASK);
			*gp = g;
			if (!propagate)
				break;
		}
	}
}

#endif

#endif /* JEMALLOC_H_INLINES */
/******************************************************************************/
