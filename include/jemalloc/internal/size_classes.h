/* This file was automatically generated by size_classes.sh. */
/******************************************************************************/
#ifdef JEMALLOC_H_TYPES

/*
 * This header requires LG_SIZEOF_PTR, LG_TINY_MIN, LG_QUANTUM, and LG_PAGE to
 * be defined prior to inclusion, and it in turn defines:
 *
 *   LG_SIZE_CLASS_GROUP: Lg of size class count for each size doubling.
 *   SIZE_CLASSES: Complete table of
 *                 SC(index, lg_grp, lg_delta, ndelta, bin, lg_delta_lookup)
 *                 tuples.
 *     index: Size class index.
 *     lg_grp: Lg group base size (no deltas added).
 *     lg_delta: Lg delta to previous size class.
 *     ndelta: Delta multiplier.  size == 1<<lg_grp + ndelta<<lg_delta
 *     bin: 'yes' if a small bin size class, 'no' otherwise.
 *     lg_delta_lookup: Same as lg_delta if a lookup table size class, 'no'
 *                      otherwise.
 *   NTBINS: Number of tiny bins.
 *   NLBINS: Number of bins supported by the lookup table.
 *   NBINS: Number of small size class bins.
 *   NSIZES: Number of size classes.
 *   LG_TINY_MAXCLASS: Lg of maximum tiny size class.
 *   LOOKUP_MAXCLASS: Maximum size class included in lookup table.
 *   SMALL_MAXCLASS: Maximum small size class.
 *   LG_LARGE_MINCLASS: Lg of minimum large size class.
 */

#define	LG_SIZE_CLASS_GROUP	2

#if (LG_SIZEOF_PTR == 2 && LG_TINY_MIN == 3 && LG_QUANTUM == 3 && LG_PAGE == 12)

#endif

#if (LG_SIZEOF_PTR == 2 && LG_TINY_MIN == 3 && LG_QUANTUM == 4 && LG_PAGE == 12)

#endif

#if (LG_SIZEOF_PTR == 2 && LG_TINY_MIN == 4 && LG_QUANTUM == 4 && LG_PAGE == 12)

#endif

#if (LG_SIZEOF_PTR == 3 && LG_TINY_MIN == 3 && LG_QUANTUM == 3 && LG_PAGE == 12)

#endif

#if (LG_SIZEOF_PTR == 3 && LG_TINY_MIN == 3 && LG_QUANTUM == 4 && LG_PAGE == 12)
#define	SIZE_CLASSES \
  /* index, lg_grp, lg_delta, ndelta, bin, lg_delta_lookup */ \
    SC(  0,      3,        3,      0, yes,  3) \
                                               \
    SC(  1,      3,        3,      1, yes,  3) \
    SC(  2,      4,        4,      1, yes,  4) \
    SC(  3,      4,        4,      2, yes,  4) \
    SC(  4,      4,        4,      3, yes,  4) \
                                               \
    SC(  5,      6,        4,      1, yes,  4) \
    SC(  6,      6,        4,      2, yes,  4) \
    SC(  7,      6,        4,      3, yes,  4) \
    SC(  8,      6,        4,      4, yes,  4) \
                                               \
    SC(  9,      7,        5,      1, yes,  5) \
    SC( 10,      7,        5,      2, yes,  5) \
    SC( 11,      7,        5,      3, yes,  5) \
    SC( 12,      7,        5,      4, yes,  5) \
                                               \
    SC( 13,      8,        6,      1, yes,  6) \
    SC( 14,      8,        6,      2, yes,  6) \
    SC( 15,      8,        6,      3, yes,  6) \
    SC( 16,      8,        6,      4, yes,  6) \
                                               \
    SC( 17,      9,        7,      1, yes,  7) \
    SC( 18,      9,        7,      2, yes,  7) \
    SC( 19,      9,        7,      3, yes,  7) \
    SC( 20,      9,        7,      4, yes,  7) \
                                               \
    SC( 21,     10,        8,      1, yes,  8) \
    SC( 22,     10,        8,      2, yes,  8) \
    SC( 23,     10,        8,      3, yes,  8) \
    SC( 24,     10,        8,      4, yes,  8) \
                                               \
    SC( 25,     11,        9,      1, yes,  9) \
    SC( 26,     11,        9,      2, yes,  9) \
    SC( 27,     11,        9,      3, yes,  9) \
    SC( 28,     11,        9,      4, yes,  9) \
                                               \
    SC( 29,     12,       10,      1, yes, no) \
    SC( 30,     12,       10,      2, yes, no) \
    SC( 31,     12,       10,      3, yes, no) \
    SC( 32,     12,       10,      4, yes, no) \
                                               \
    SC( 33,     13,       11,      1, yes, no) \
    SC( 34,     13,       11,      2, yes, no) \
    SC( 35,     13,       11,      3, yes, no) \
    SC( 36,     13,       11,      4,  no, no) \
                                               \
    SC( 37,     14,       12,      1,  no, no) \
    SC( 38,     14,       12,      2,  no, no) \
    SC( 39,     14,       12,      3,  no, no) \
    SC( 40,     14,       12,      4,  no, no) \
                                               \
    SC( 41,     15,       13,      1,  no, no) \
    SC( 42,     15,       13,      2,  no, no) \
    SC( 43,     15,       13,      3,  no, no) \
    SC( 44,     15,       13,      4,  no, no) \
                                               \
    SC( 45,     16,       14,      1,  no, no) \
    SC( 46,     16,       14,      2,  no, no) \
    SC( 47,     16,       14,      3,  no, no) \
    SC( 48,     16,       14,      4,  no, no) \
                                               \
    SC( 49,     17,       15,      1,  no, no) \
    SC( 50,     17,       15,      2,  no, no) \
    SC( 51,     17,       15,      3,  no, no) \
    SC( 52,     17,       15,      4,  no, no) \
                                               \
    SC( 53,     18,       16,      1,  no, no) \
    SC( 54,     18,       16,      2,  no, no) \
    SC( 55,     18,       16,      3,  no, no) \
    SC( 56,     18,       16,      4,  no, no) \
                                               \
    SC( 57,     19,       17,      1,  no, no) \
    SC( 58,     19,       17,      2,  no, no) \
    SC( 59,     19,       17,      3,  no, no) \
    SC( 60,     19,       17,      4,  no, no) \
                                               \
    SC( 61,     20,       18,      1,  no, no) \
    SC( 62,     20,       18,      2,  no, no) \
    SC( 63,     20,       18,      3,  no, no) \
    SC( 64,     20,       18,      4,  no, no) \
                                               \
    SC( 65,     21,       19,      1,  no, no) \
    SC( 66,     21,       19,      2,  no, no) \
    SC( 67,     21,       19,      3,  no, no) \
    SC( 68,     21,       19,      4,  no, no) \
                                               \
    SC( 69,     22,       20,      1,  no, no) \
    SC( 70,     22,       20,      2,  no, no) \
    SC( 71,     22,       20,      3,  no, no) \
    SC( 72,     22,       20,      4,  no, no) \
                                               \
    SC( 73,     23,       21,      1,  no, no) \
    SC( 74,     23,       21,      2,  no, no) \
    SC( 75,     23,       21,      3,  no, no) \
    SC( 76,     23,       21,      4,  no, no) \
                                               \
    SC( 77,     24,       22,      1,  no, no) \
    SC( 78,     24,       22,      2,  no, no) \
    SC( 79,     24,       22,      3,  no, no) \
    SC( 80,     24,       22,      4,  no, no) \
                                               \
    SC( 81,     25,       23,      1,  no, no) \
    SC( 82,     25,       23,      2,  no, no) \
    SC( 83,     25,       23,      3,  no, no) \
    SC( 84,     25,       23,      4,  no, no) \
                                               \
    SC( 85,     26,       24,      1,  no, no) \
    SC( 86,     26,       24,      2,  no, no) \
    SC( 87,     26,       24,      3,  no, no) \
    SC( 88,     26,       24,      4,  no, no) \
                                               \
    SC( 89,     27,       25,      1,  no, no) \
    SC( 90,     27,       25,      2,  no, no) \
    SC( 91,     27,       25,      3,  no, no) \
    SC( 92,     27,       25,      4,  no, no) \
                                               \
    SC( 93,     28,       26,      1,  no, no) \
    SC( 94,     28,       26,      2,  no, no) \
    SC( 95,     28,       26,      3,  no, no) \
    SC( 96,     28,       26,      4,  no, no) \
                                               \
    SC( 97,     29,       27,      1,  no, no) \
    SC( 98,     29,       27,      2,  no, no) \
    SC( 99,     29,       27,      3,  no, no) \
    SC(100,     29,       27,      4,  no, no) \
                                               \
    SC(101,     30,       28,      1,  no, no) \
    SC(102,     30,       28,      2,  no, no) \
    SC(103,     30,       28,      3,  no, no) \
    SC(104,     30,       28,      4,  no, no) \
                                               \
    SC(105,     31,       29,      1,  no, no) \
    SC(106,     31,       29,      2,  no, no) \
    SC(107,     31,       29,      3,  no, no) \
    SC(108,     31,       29,      4,  no, no) \
                                               \
    SC(109,     32,       30,      1,  no, no) \
    SC(110,     32,       30,      2,  no, no) \
    SC(111,     32,       30,      3,  no, no) \
    SC(112,     32,       30,      4,  no, no) \
                                               \
    SC(113,     33,       31,      1,  no, no) \
    SC(114,     33,       31,      2,  no, no) \
    SC(115,     33,       31,      3,  no, no) \
    SC(116,     33,       31,      4,  no, no) \
                                               \
    SC(117,     34,       32,      1,  no, no) \
    SC(118,     34,       32,      2,  no, no) \
    SC(119,     34,       32,      3,  no, no) \
    SC(120,     34,       32,      4,  no, no) \
                                               \
    SC(121,     35,       33,      1,  no, no) \
    SC(122,     35,       33,      2,  no, no) \
    SC(123,     35,       33,      3,  no, no) \
    SC(124,     35,       33,      4,  no, no) \
                                               \
    SC(125,     36,       34,      1,  no, no) \
    SC(126,     36,       34,      2,  no, no) \
    SC(127,     36,       34,      3,  no, no) \
    SC(128,     36,       34,      4,  no, no) \
                                               \
    SC(129,     37,       35,      1,  no, no) \
    SC(130,     37,       35,      2,  no, no) \
    SC(131,     37,       35,      3,  no, no) \
    SC(132,     37,       35,      4,  no, no) \
                                               \
    SC(133,     38,       36,      1,  no, no) \
    SC(134,     38,       36,      2,  no, no) \
    SC(135,     38,       36,      3,  no, no) \
    SC(136,     38,       36,      4,  no, no) \
                                               \
    SC(137,     39,       37,      1,  no, no) \
    SC(138,     39,       37,      2,  no, no) \
    SC(139,     39,       37,      3,  no, no) \
    SC(140,     39,       37,      4,  no, no) \
                                               \
    SC(141,     40,       38,      1,  no, no) \
    SC(142,     40,       38,      2,  no, no) \
    SC(143,     40,       38,      3,  no, no) \
    SC(144,     40,       38,      4,  no, no) \
                                               \
    SC(145,     41,       39,      1,  no, no) \
    SC(146,     41,       39,      2,  no, no) \
    SC(147,     41,       39,      3,  no, no) \
    SC(148,     41,       39,      4,  no, no) \
                                               \
    SC(149,     42,       40,      1,  no, no) \
    SC(150,     42,       40,      2,  no, no) \
    SC(151,     42,       40,      3,  no, no) \
    SC(152,     42,       40,      4,  no, no) \
                                               \
    SC(153,     43,       41,      1,  no, no) \
    SC(154,     43,       41,      2,  no, no) \
    SC(155,     43,       41,      3,  no, no) \
    SC(156,     43,       41,      4,  no, no) \
                                               \
    SC(157,     44,       42,      1,  no, no) \
    SC(158,     44,       42,      2,  no, no) \
    SC(159,     44,       42,      3,  no, no) \
    SC(160,     44,       42,      4,  no, no) \
                                               \
    SC(161,     45,       43,      1,  no, no) \
    SC(162,     45,       43,      2,  no, no) \
    SC(163,     45,       43,      3,  no, no) \
    SC(164,     45,       43,      4,  no, no) \
                                               \
    SC(165,     46,       44,      1,  no, no) \
    SC(166,     46,       44,      2,  no, no) \
    SC(167,     46,       44,      3,  no, no) \
    SC(168,     46,       44,      4,  no, no) \
                                               \
    SC(169,     47,       45,      1,  no, no) \
    SC(170,     47,       45,      2,  no, no) \
    SC(171,     47,       45,      3,  no, no) \
    SC(172,     47,       45,      4,  no, no) \
                                               \
    SC(173,     48,       46,      1,  no, no) \
    SC(174,     48,       46,      2,  no, no) \
    SC(175,     48,       46,      3,  no, no) \
    SC(176,     48,       46,      4,  no, no) \
                                               \
    SC(177,     49,       47,      1,  no, no) \
    SC(178,     49,       47,      2,  no, no) \
    SC(179,     49,       47,      3,  no, no) \
    SC(180,     49,       47,      4,  no, no) \
                                               \
    SC(181,     50,       48,      1,  no, no) \
    SC(182,     50,       48,      2,  no, no) \
    SC(183,     50,       48,      3,  no, no) \
    SC(184,     50,       48,      4,  no, no) \
                                               \
    SC(185,     51,       49,      1,  no, no) \
    SC(186,     51,       49,      2,  no, no) \
    SC(187,     51,       49,      3,  no, no) \
    SC(188,     51,       49,      4,  no, no) \
                                               \
    SC(189,     52,       50,      1,  no, no) \
    SC(190,     52,       50,      2,  no, no) \
    SC(191,     52,       50,      3,  no, no) \
    SC(192,     52,       50,      4,  no, no) \
                                               \
    SC(193,     53,       51,      1,  no, no) \
    SC(194,     53,       51,      2,  no, no) \
    SC(195,     53,       51,      3,  no, no) \
    SC(196,     53,       51,      4,  no, no) \
                                               \
    SC(197,     54,       52,      1,  no, no) \
    SC(198,     54,       52,      2,  no, no) \
    SC(199,     54,       52,      3,  no, no) \
    SC(200,     54,       52,      4,  no, no) \
                                               \
    SC(201,     55,       53,      1,  no, no) \
    SC(202,     55,       53,      2,  no, no) \
    SC(203,     55,       53,      3,  no, no) \
    SC(204,     55,       53,      4,  no, no) \
                                               \
    SC(205,     56,       54,      1,  no, no) \
    SC(206,     56,       54,      2,  no, no) \
    SC(207,     56,       54,      3,  no, no) \
    SC(208,     56,       54,      4,  no, no) \
                                               \
    SC(209,     57,       55,      1,  no, no) \
    SC(210,     57,       55,      2,  no, no) \
    SC(211,     57,       55,      3,  no, no) \
    SC(212,     57,       55,      4,  no, no) \
                                               \
    SC(213,     58,       56,      1,  no, no) \
    SC(214,     58,       56,      2,  no, no) \
    SC(215,     58,       56,      3,  no, no) \
    SC(216,     58,       56,      4,  no, no) \
                                               \
    SC(217,     59,       57,      1,  no, no) \
    SC(218,     59,       57,      2,  no, no) \
    SC(219,     59,       57,      3,  no, no) \
    SC(220,     59,       57,      4,  no, no) \
                                               \
    SC(221,     60,       58,      1,  no, no) \
    SC(222,     60,       58,      2,  no, no) \
    SC(223,     60,       58,      3,  no, no) \
    SC(224,     60,       58,      4,  no, no) \
                                               \
    SC(225,     61,       59,      1,  no, no) \
    SC(226,     61,       59,      2,  no, no) \
    SC(227,     61,       59,      3,  no, no) \
    SC(228,     61,       59,      4,  no, no) \
                                               \
    SC(229,     62,       60,      1,  no, no) \
    SC(230,     62,       60,      2,  no, no) \
    SC(231,     62,       60,      3,  no, no) \
    SC(232,     62,       60,      4,  no, no) \
                                               \
    SC(233,     63,       61,      1,  no, no) \
    SC(234,     63,       61,      2,  no, no) \
    SC(235,     63,       61,      3,  no, no) \

#define	SIZE_CLASSES_DEFINED
#define	NTBINS			1
#define	NLBINS			29
#define	NBINS			36
#define	NSIZES			236
#define	LG_TINY_MAXCLASS	3
#define	LOOKUP_MAXCLASS		((((size_t)1) << 11) + (((size_t)4) << 9))
#define	SMALL_MAXCLASS		((((size_t)1) << 13) + (((size_t)3) << 11))
#define	LG_LARGE_MINCLASS	14
#endif

#if (LG_SIZEOF_PTR == 3 && LG_TINY_MIN == 4 && LG_QUANTUM == 4 && LG_PAGE == 12)

#endif

#ifndef SIZE_CLASSES_DEFINED
#  error "No size class definitions match configuration"
#endif
#undef SIZE_CLASSES_DEFINED
/*
 * The size2index_tab lookup table uses uint8_t to encode each bin index, so we
 * cannot support more than 256 small size classes.  Further constrain NBINS to
 * 255 since all small size classes, plus a "not small" size class must be
 * stored in 8 bits of arena_chunk_map_bits_t's bits field.
 */
#if (NBINS > 255)
#  error "Too many small size classes"
#endif

#endif /* JEMALLOC_H_TYPES */
/******************************************************************************/
#ifdef JEMALLOC_H_STRUCTS


#endif /* JEMALLOC_H_STRUCTS */
/******************************************************************************/
#ifdef JEMALLOC_H_EXTERNS


#endif /* JEMALLOC_H_EXTERNS */
/******************************************************************************/
#ifdef JEMALLOC_H_INLINES


#endif /* JEMALLOC_H_INLINES */
/******************************************************************************/
