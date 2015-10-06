#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <strings.h>

#define	JEMALLOC_VERSION "4.0.0-14-g4771f24a12fa99145c437ed718edba536da3d57b"
#define	JEMALLOC_VERSION_MAJOR 4
#define	JEMALLOC_VERSION_MINOR 0
#define	JEMALLOC_VERSION_BUGFIX 0
#define	JEMALLOC_VERSION_NREV 14
#define	JEMALLOC_VERSION_GID "4771f24a12fa99145c437ed718edba536da3d57b"

#  define MALLOCX_LG_ALIGN(la)	(la)
#  if LG_SIZEOF_PTR == 2

#  else
#    define MALLOCX_ALIGN(a)						\
	 ((a < (size_t)INT_MAX) ? ffs(a)-1 : ffs(a>>32)+31)
#  endif
#  define MALLOCX_ZERO	((int)0x40)
/*
 * Bias tcache index bits so that 0 encodes "automatic tcache management", and 1
 * encodes MALLOCX_TCACHE_NONE.
 */
#  define MALLOCX_TCACHE(tc)	((int)(((tc)+2) << 8))
#  define MALLOCX_TCACHE_NONE	MALLOCX_TCACHE(-1)
/*
 * Bias arena index bits so that 0 encodes "use an automatically chosen arena".
 */
#  define MALLOCX_ARENA(a)	((int)(((a)+1) << 20))

#if defined(__cplusplus) && defined(JEMALLOC_USE_CXX_THROW)

#else
#  define JEMALLOC_CXX_THROW
#endif

#ifdef JEMALLOC_HAVE_ATTR
#  define JEMALLOC_ATTR(s) __attribute__((s))
#  define JEMALLOC_ALIGNED(s) JEMALLOC_ATTR(aligned(s))
#  ifdef JEMALLOC_HAVE_ATTR_ALLOC_SIZE
#    define JEMALLOC_ALLOC_SIZE(s) JEMALLOC_ATTR(alloc_size(s))
#    define JEMALLOC_ALLOC_SIZE2(s1, s2) JEMALLOC_ATTR(alloc_size(s1, s2))
#  else

#  endif
#  ifndef JEMALLOC_EXPORT

#  endif
#  ifdef JEMALLOC_HAVE_ATTR_FORMAT_GNU_PRINTF
#    define JEMALLOC_FORMAT_PRINTF(s, i) JEMALLOC_ATTR(format(gnu_printf, s, i))
#  elif defined(JEMALLOC_HAVE_ATTR_FORMAT_PRINTF)

#  else

#  endif
#  define JEMALLOC_NOINLINE JEMALLOC_ATTR(noinline)
#  define JEMALLOC_NOTHROW JEMALLOC_ATTR(nothrow)
#  define JEMALLOC_SECTION(s) JEMALLOC_ATTR(section(s))
#  define JEMALLOC_RESTRICT_RETURN
#  define JEMALLOC_ALLOCATOR
#elif _MSC_VER

#else

#endif
