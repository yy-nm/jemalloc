#ifndef JEMALLOC_INTERNAL_DECLS_H
#define	JEMALLOC_INTERNAL_DECLS_H

#include <math.h>
#ifdef _WIN32


#else // __NR_write �����϶����� <bits/syscall.h> ��, ������ write �ĵײ�ʵ��
#  include <sys/param.h>
#  include <sys/mman.h>
#  if !defined(__pnacl__) && !defined(__native_client__)
#    include <sys/syscall.h>
#    if !defined(SYS_write) && defined(__NR_write)
#      define SYS_write __NR_write
#    endif
#    include <sys/uio.h>
#  endif
#  include <pthread.h>
#  include <errno.h>
#endif
#include <sys/types.h>

#include <limits.h>
#ifndef SIZE_T_MAX
#  define SIZE_T_MAX	SIZE_MAX
#endif
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#ifndef offsetof
#  define offsetof(type, member)	((size_t)&(((type *)NULL)->member))
#endif
#include <string.h>
#include <strings.h>
#include <ctype.h>
#ifdef _MSC_VER

#else
#  include <unistd.h>
#endif
#include <fcntl.h>

#endif /* JEMALLOC_INTERNAL_H */
