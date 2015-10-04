/******************************************************************************/
#ifdef JEMALLOC_H_TYPES

#ifdef JEMALLOC_VALGRIND

#else
#define	RUNNING_ON_VALGRIND	((unsigned)0)
#define	JEMALLOC_VALGRIND_MAKE_MEM_NOACCESS(ptr, usize) do {} while (0)
#define	JEMALLOC_VALGRIND_MAKE_MEM_UNDEFINED(ptr, usize) do {} while (0)
#define	JEMALLOC_VALGRIND_MAKE_MEM_DEFINED(ptr, usize) do {} while (0)
#define	JEMALLOC_VALGRIND_MALLOC(cond, ptr, usize, zero) do {} while (0)
#define	JEMALLOC_VALGRIND_REALLOC(maybe_moved, ptr, usize,		\
    ptr_maybe_null, old_ptr, old_usize, old_rzsize, old_ptr_maybe_null,	\
    zero) do {} while (0)
#define	JEMALLOC_VALGRIND_FREE(ptr, rzsize) do {} while (0)
#endif

#endif /* JEMALLOC_H_TYPES */
/******************************************************************************/
#ifdef JEMALLOC_H_STRUCTS

#endif /* JEMALLOC_H_STRUCTS */
/******************************************************************************/
#ifdef JEMALLOC_H_EXTERNS

#ifdef JEMALLOC_VALGRIND
void	valgrind_make_mem_noaccess(void *ptr, size_t usize);
void	valgrind_make_mem_undefined(void *ptr, size_t usize);
void	valgrind_make_mem_defined(void *ptr, size_t usize);
void	valgrind_freelike_block(void *ptr, size_t usize);
#endif

#endif /* JEMALLOC_H_EXTERNS */
/******************************************************************************/
#ifdef JEMALLOC_H_INLINES

#endif /* JEMALLOC_H_INLINES */
/******************************************************************************/

