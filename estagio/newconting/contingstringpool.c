#include "contingstringpool.h"

static GStringChunk *chunk = NULL;

gchar *
conting_string_pool_add_const(const gchar *str)
{
	if (chunk == NULL)
		chunk = g_string_chunk_new(10);

	return g_string_chunk_insert_const(chunk, str);
}

gchar *
conting_string_pool_add(gchar *str)
{
	gchar *result = conting_string_pool_add_const(str);

	g_free(str);

	return result;
}
