


typedef struct bitmap_info_s bitmap_info_t;
typedef struct bitmap_level_s bitmap_level_t;
typedef unsigned long bitmap_t;


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
	bitmap_level_t levels[4];
};