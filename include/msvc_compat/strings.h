#ifndef strings_h
#define strings_h

/* MSVC doesn't define ffs/ffsl. This dummy strings.h header is provided
 * for both */
#ifdef _MSC_VER

#else
// ���ص�һ������ 0 �� bit, λ�ôӵ�λ��ʼ, �� 1 ��ʼ
#  define ffsl(x) __builtin_ffsl(x)
#  define ffs(x) __builtin_ffs(x)
#endif

#endif /* strings_h */
