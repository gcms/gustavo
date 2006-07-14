#ifndef __ASSERT_H__
#define __ASSERT_H__

#define assert(e) ((e) ? (void) 0 : _assert(__FILE__, __LINE__, #e))
void _assert(const char *file, int lineno, const char *e);

#endif /* __ASSERT_H__ */
