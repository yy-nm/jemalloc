#ifndef JEMALLOC_H_
#define	JEMALLOC_H_
#ifdef __cplusplus
extern "C" {
#endif

/* Defined if __attribute__((...)) syntax is supported. */
#define	JEMALLOC_HAVE_ATTR

/* Defined if alloc_size attribute is supported. */
#define	JEMALLOC_HAVE_ATTR_ALLOC_SIZE

/* Defined if format(gnu_printf, ...) attribute is supported. */
#define	JEMALLOC_HAVE_ATTR_FORMAT_GNU_PRINTF

/* Defined if format(printf, ...) attribute is supported. */
#define	JEMALLOC_HAVE_ATTR_FORMAT_PRINTF

/*
 * Define overrides for non-standard allocator-related functions if they are
 * present on the system.
 */
#define	JEMALLOC_OVERRIDE_MEMALIGN
#define	JEMALLOC_OVERRIDE_VALLOC

/*
 * At least Linux omits the "const" in:
 *
 *   size_t malloc_usable_size(const void *ptr);
 *
 * Match the operating system's prototype.
 */
#define	JEMALLOC_USABLE_SIZE_CONST

/*
 * If defined, specify throw() for the public function prototypes when compiling
 * with C++.  The only justification for this is to match the prototypes that
 * glibc defines.
 */
#define	JEMALLOC_USE_CXX_THROW

/* sizeof(void *) == 2^LG_SIZEOF_PTR. */
#define	LG_SIZEOF_PTR 3

/*
 * Name mangling for public symbols is controlled by --with-mangling and
 * --with-jemalloc-prefix.  With default settings the je_ prefix is stripped by
 * these macro definitions.
 */
#ifndef JEMALLOC_NO_RENAME

#endif

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

/*
 * The je_ prefix on the following public symbol declarations is an artifact
 * of namespace management, and should be omitted in application code unless
 * JEMALLOC_NO_DEMANGLE is defined (see jemalloc_mangle.h).
 */
extern JEMALLOC_EXPORT const char	*je_malloc_conf;
extern JEMALLOC_EXPORT void		(*je_malloc_message)(void *cbopaque,
    const char *s);

JEMALLOC_EXPORT JEMALLOC_ALLOCATOR JEMALLOC_RESTRICT_RETURN
    void JEMALLOC_NOTHROW	*je_malloc(size_t size)
    JEMALLOC_CXX_THROW JEMALLOC_ATTR(malloc) JEMALLOC_ALLOC_SIZE(1);
JEMALLOC_EXPORT JEMALLOC_ALLOCATOR JEMALLOC_RESTRICT_RETURN
    void JEMALLOC_NOTHROW	*je_calloc(size_t num, size_t size)
    JEMALLOC_CXX_THROW JEMALLOC_ATTR(malloc) JEMALLOC_ALLOC_SIZE2(1, 2);
JEMALLOC_EXPORT int JEMALLOC_NOTHROW	je_posix_memalign(void **memptr,
    size_t alignment, size_t size) JEMALLOC_CXX_THROW JEMALLOC_ATTR(nonnull(1));
JEMALLOC_EXPORT JEMALLOC_ALLOCATOR JEMALLOC_RESTRICT_RETURN
    void JEMALLOC_NOTHROW	*je_aligned_alloc(size_t alignment,
    size_t size) JEMALLOC_CXX_THROW JEMALLOC_ATTR(malloc)
    JEMALLOC_ALLOC_SIZE(2);
JEMALLOC_EXPORT JEMALLOC_ALLOCATOR JEMALLOC_RESTRICT_RETURN
    void JEMALLOC_NOTHROW	*je_realloc(void *ptr, size_t size)
    JEMALLOC_CXX_THROW JEMALLOC_ALLOC_SIZE(2);
JEMALLOC_EXPORT void JEMALLOC_NOTHROW	je_free(void *ptr)
    JEMALLOC_CXX_THROW;

JEMALLOC_EXPORT JEMALLOC_ALLOCATOR JEMALLOC_RESTRICT_RETURN
    void JEMALLOC_NOTHROW	*je_mallocx(size_t size, int flags)
    JEMALLOC_ATTR(malloc) JEMALLOC_ALLOC_SIZE(1);
JEMALLOC_EXPORT JEMALLOC_ALLOCATOR JEMALLOC_RESTRICT_RETURN
    void JEMALLOC_NOTHROW	*je_rallocx(void *ptr, size_t size,
    int flags) JEMALLOC_ALLOC_SIZE(2);
JEMALLOC_EXPORT size_t JEMALLOC_NOTHROW	je_xallocx(void *ptr, size_t size,
    size_t extra, int flags);
JEMALLOC_EXPORT size_t JEMALLOC_NOTHROW	je_sallocx(const void *ptr,
    int flags) JEMALLOC_ATTR(pure);
JEMALLOC_EXPORT void JEMALLOC_NOTHROW	je_dallocx(void *ptr, int flags);
JEMALLOC_EXPORT void JEMALLOC_NOTHROW	je_sdallocx(void *ptr, size_t size,
    int flags);
JEMALLOC_EXPORT size_t JEMALLOC_NOTHROW	je_nallocx(size_t size, int flags)
    JEMALLOC_ATTR(pure);

JEMALLOC_EXPORT int JEMALLOC_NOTHROW	je_mallctl(const char *name,
    void *oldp, size_t *oldlenp, void *newp, size_t newlen);
JEMALLOC_EXPORT int JEMALLOC_NOTHROW	je_mallctlnametomib(const char *name,
    size_t *mibp, size_t *miblenp);
JEMALLOC_EXPORT int JEMALLOC_NOTHROW	je_mallctlbymib(const size_t *mib,
    size_t miblen, void *oldp, size_t *oldlenp, void *newp, size_t newlen);
JEMALLOC_EXPORT void JEMALLOC_NOTHROW	je_malloc_stats_print(
    void (*write_cb)(void *, const char *), void *je_cbopaque,
    const char *opts);
JEMALLOC_EXPORT size_t JEMALLOC_NOTHROW	je_malloc_usable_size(
    JEMALLOC_USABLE_SIZE_CONST void *ptr) JEMALLOC_CXX_THROW;

#ifdef JEMALLOC_OVERRIDE_MEMALIGN
JEMALLOC_EXPORT JEMALLOC_ALLOCATOR JEMALLOC_RESTRICT_RETURN
    void JEMALLOC_NOTHROW	*je_memalign(size_t alignment, size_t size)
    JEMALLOC_CXX_THROW JEMALLOC_ATTR(malloc);
#endif

#ifdef JEMALLOC_OVERRIDE_VALLOC
JEMALLOC_EXPORT JEMALLOC_ALLOCATOR JEMALLOC_RESTRICT_RETURN
    void JEMALLOC_NOTHROW	*je_valloc(size_t size) JEMALLOC_CXX_THROW
    JEMALLOC_ATTR(malloc);
#endif

/*
 * void *
 * chunk_alloc(void *new_addr, size_t size, size_t alignment, bool *zero,
 *     bool *commit, unsigned arena_ind);
 */
typedef void *(chunk_alloc_t)(void *, size_t, size_t, bool *, bool *, unsigned);

/*
 * bool
 * chunk_dalloc(void *chunk, size_t size, bool committed, unsigned arena_ind);
 */
typedef bool (chunk_dalloc_t)(void *, size_t, bool, unsigned);

/*
 * bool
 * chunk_commit(void *chunk, size_t size, size_t offset, size_t length,
 *     unsigned arena_ind);
 */
typedef bool (chunk_commit_t)(void *, size_t, size_t, size_t, unsigned);

/*
 * bool
 * chunk_decommit(void *chunk, size_t size, size_t offset, size_t length,
 *     unsigned arena_ind);
 */
typedef bool (chunk_decommit_t)(void *, size_t, size_t, size_t, unsigned);

/*
 * bool
 * chunk_purge(void *chunk, size_t size, size_t offset, size_t length,
 *     unsigned arena_ind);
 */
typedef bool (chunk_purge_t)(void *, size_t, size_t, size_t, unsigned);

/*
 * bool
 * chunk_split(void *chunk, size_t size, size_t size_a, size_t size_b,
 *     bool committed, unsigned arena_ind);
 */
typedef bool (chunk_split_t)(void *, size_t, size_t, size_t, bool, unsigned);

/*
 * bool
 * chunk_merge(void *chunk_a, size_t size_a, void *chunk_b, size_t size_b,
 *     bool committed, unsigned arena_ind);
 */
typedef bool (chunk_merge_t)(void *, size_t, void *, size_t, bool, unsigned);

typedef struct {
	chunk_alloc_t		*alloc;
	chunk_dalloc_t		*dalloc;
	chunk_commit_t		*commit;
	chunk_decommit_t	*decommit;
	chunk_purge_t		*purge;
	chunk_split_t		*split;
	chunk_merge_t		*merge;
} chunk_hooks_t;

/*
 * By default application code must explicitly refer to mangled symbol names,
 * so that it is possible to use jemalloc in conjunction with another allocator
 * in the same application.  Define JEMALLOC_MANGLE in order to cause automatic
 * name mangling that matches the API prefixing that happened as a result of
 * --with-mangling and/or --with-jemalloc-prefix configuration settings.
 */
#ifdef JEMALLOC_MANGLE

#endif

/*
 * The je_* macros can be used as stable alternative names for the
 * public jemalloc API if JEMALLOC_NO_DEMANGLE is defined.  This is primarily
 * meant for use in jemalloc itself, but it can be used by application code to
 * provide isolation from the name mangling specified via --with-mangling
 * and/or --with-jemalloc-prefix.
 */
#ifndef JEMALLOC_NO_DEMANGLE
#  undef je_malloc_conf
#  undef je_malloc_message
#  undef je_malloc
#  undef je_calloc
#  undef je_posix_memalign
#  undef je_aligned_alloc
#  undef je_realloc
#  undef je_free
#  undef je_mallocx
#  undef je_rallocx
#  undef je_xallocx
#  undef je_sallocx
#  undef je_dallocx
#  undef je_sdallocx
#  undef je_nallocx
#  undef je_mallctl
#  undef je_mallctlnametomib
#  undef je_mallctlbymib
#  undef je_malloc_stats_print
#  undef je_malloc_usable_size
#  undef je_memalign
#  undef je_valloc
#endif

#ifdef __cplusplus
}
#endif
#endif /* JEMALLOC_H_ */
