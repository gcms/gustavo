#ifndef CONTING_STRING_POOL_H
#define CONTING_STRING_POOL_H

#include <glib.h>

/**
 * Takes control over str. So, it's not safe to use str after calling it.
 */
gchar *
conting_string_pool_add(gchar *str);

/**
 * Makes a copy of str.
 */
gchar *
conting_string_pool_const(const gchar *str);

#endif /* CONTING_STRING_POOL_H */
