#define	JEMALLOC_PAGES_C_
#include "jemalloc/internal/jemalloc_internal.h"

/******************************************************************************/

void *
pages_map(void *addr, size_t size)
{
	void *ret;

	assert(size != 0);

#ifdef _WIN32

#else
	/*
	 * We don't use MAP_FIXED here, because it can cause the *replacement*
	 * of existing mappings, and we only want to create new mappings.
	 */
	ret = mmap(addr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON,
	    -1, 0);
	assert(ret != NULL);

	if (ret == MAP_FAILED)
		ret = NULL;
	else if (addr != NULL && ret != addr) {
		/*
		 * We succeeded in mapping memory, but not in the right place.
		 */
		pages_unmap(ret, size);
		ret = NULL;
	}
#endif
	assert(ret == NULL || (addr == NULL && ret != addr)
	    || (addr != NULL && ret == addr));
	return (ret);
}

void
pages_unmap(void *addr, size_t size)
{

#ifdef _WIN32

#else
	if (munmap(addr, size) == -1)
#endif
	{
		char buf[BUFERROR_BUF];

		buferror(get_errno(), buf, sizeof(buf));
		malloc_printf("<jemalloc>: Error in "
#ifdef _WIN32

#else
		              "munmap"
#endif
		              "(): %s\n", buf);
		if (opt_abort)
			abort();
	}
}

void *
pages_trim(void *addr, size_t alloc_size, size_t leadsize, size_t size)
{
	void *ret = (void *)((uintptr_t)addr + leadsize);

	assert(alloc_size >= leadsize + size);
#ifdef _WIN32

#else
	{
		size_t trailsize = alloc_size - leadsize - size;

		if (leadsize != 0)
			pages_unmap(addr, leadsize);
		if (trailsize != 0)
			pages_unmap((void *)((uintptr_t)ret + size), trailsize);
		return (ret);
	}
#endif
}

static bool
pages_commit_impl(void *addr, size_t size, bool commit)
{

#ifndef _WIN32
	/*
	 * The following decommit/commit implementation is functional, but
	 * always disabled because it doesn't add value beyong improved
	 * debugging (at the cost of extra system calls) on systems that
	 * overcommit.
	 */
	if (false) {
		int prot = commit ? (PROT_READ | PROT_WRITE) : PROT_NONE;
		void *result = mmap(addr, size, prot, MAP_PRIVATE | MAP_ANON |
		    MAP_FIXED, -1, 0);
		if (result == MAP_FAILED)
			return (true);
		if (result != addr) {
			/*
			 * We succeeded in mapping memory, but not in the right
			 * place.
			 */
			pages_unmap(result, size);
			return (true);
		}
		return (false);
	}
#endif
	return (true);
}

bool
pages_commit(void *addr, size_t size)
{

	return (pages_commit_impl(addr, size, true));
}

bool
pages_decommit(void *addr, size_t size)
{

	return (pages_commit_impl(addr, size, false));
}

bool
pages_purge(void *addr, size_t size)
{
	bool unzeroed;

#ifdef _WIN32

#elif defined(JEMALLOC_HAVE_MADVISE)
#  ifdef JEMALLOC_PURGE_MADVISE_DONTNEED
#    define JEMALLOC_MADV_PURGE MADV_DONTNEED
#    define JEMALLOC_MADV_ZEROS true
#  elif defined(JEMALLOC_PURGE_MADVISE_FREE)

#  else
#    error "No madvise(2) flag defined for purging unused dirty pages."
#  endif
	int err = madvise(addr, size, JEMALLOC_MADV_PURGE);
	unzeroed = (!JEMALLOC_MADV_ZEROS || err != 0);
#  undef JEMALLOC_MADV_PURGE
#  undef JEMALLOC_MADV_ZEROS
#else
	/* Last resort no-op. */
	unzeroed = true;
#endif
	return (unzeroed);
}

