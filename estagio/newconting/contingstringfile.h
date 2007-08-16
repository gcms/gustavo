#ifndef CONTING_STRING_FILE_H
#define CONTING_STRING_FILE_H

#include <glib.h>

gchar *
conting_string_file_load(const gchar *filename);

gboolean
conting_string_file_store(const gchar *filename, const gchar *data);

#endif /* CONTING_STRING_FILE_H */
