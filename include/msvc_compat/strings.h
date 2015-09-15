#ifndef strings_h
#define strings_h

/* MSVC doesn't define ffs/ffsl. This dummy strings.h header is provided
 * for both */
#ifdef _MSC_VER

#else
// 返回第一个不是 0 的 bit, 位置从低位开始, 从 1 开始
#  define ffsl(x) __builtin_ffsl(x)
#  define ffs(x) __builtin_ffs(x)
#endif

#endif /* strings_h */
