

typedef struct rtree_node_elm_s rtree_node_elm_t;
typedef struct rtree_level_s rtree_level_t;
typedef struct rtree_s rtree_t;

struct rtree_node_elm_s {
	union {
		void			*pun;
		rtree_node_elm_t	*child;
		extent_node_t		*val;
	};
};

struct rtree_level_s {
	/*
	 * A non-NULL subtree points to a subtree rooted along the hypothetical
	 * path to the leaf node corresponding to key 0.  Depending on what keys
	 * have been used to store to the tree, an arbitrary combination of
	 * subtree pointers may remain NULL.
	 *
	 * Suppose keys comprise 48 bits, and LG_RTREE_BITS_PER_LEVEL is 4.
	 * This results in a 3-level tree, and the leftmost leaf can be directly
	 * accessed via subtrees[2], the subtree prefixed by 0x0000 (excluding
	 * 0x00000000) can be accessed via subtrees[1], and the remainder of the
	 * tree can be accessed via subtrees[0].
	 *
	 *   levels[0] : [<unused> | 0x0001******** | 0x0002******** | ...]
	 *
	 *   levels[1] : [<unused> | 0x00000001**** | 0x00000002**** | ... ]
	 *
	 *   levels[2] : [val(0x000000000000) | val(0x000000000001) | ...]
	 *
	 * This has practical implications on x64, which currently uses only the
	 * lower 47 bits of virtual address space in userland, thus leaving
	 * subtrees[0] unused and avoiding a level of tree traversal.
	 */
	union {
		void			*subtree_pun;
		rtree_node_elm_t	*subtree;
	};
	/* Number of key bits distinguished by this level. */
	unsigned		bits;
	/*
	 * Cumulative number of key bits distinguished by traversing to
	 * corresponding tree level.
	 */
	unsigned		cumbits;
};


struct rtree_s {
    // callback
	rtree_node_alloc_t	*alloc;
	rtree_node_dalloc_t	*dalloc;
	unsigned		height;
	/*
	 * Precomputed table used to convert from the number of leading 0 key
	 * bits to which subtree level to start at.
	 */
	unsigned		start_level[RTREE_HEIGHT_MAX]; //RTREE_HEIGHT_MAX = 2^6 / 2^4 = 2^2 = 4
	rtree_level_t		levels[RTREE_HEIGHT_MAX];
};