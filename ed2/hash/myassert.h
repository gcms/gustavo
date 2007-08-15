#ifndef MY_ASSERT_H
#define MY_ASSERT_H


#ifdef NDEBUG
#define	my_assert(e, m)	((void)0)
#else
#define my_assert(e, m) ((e) ? (void)0 : _my_assert(__FILE__, __LINE__, #e, m))
#endif

void _my_assert(const char *file, int lineno, const char *e, const char *msg);

#endif /* MY_ASSERT_H */
