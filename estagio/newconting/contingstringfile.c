#include  "contingstringfile.h"
#include <stdio.h>
gchar *
conting_string_file_load(const gchar *filename)
{
	FILE *fp;
	GString *string;
	size_t read;
	gchar buff[256];

	fp = fopen(filename, "r");
	if (fp == NULL)
		return NULL;

	string = g_string_sized_new(4096);
	while ((read = fread(buff, sizeof(char), 256, fp)) > 0) {
		g_string_append_len(string, buff, read);
	}

	return g_string_free(string, FALSE);
}

gboolean
conting_string_file_store(const gchar *filename, const gchar *data)
{
	FILE *fp;

	fp = fopen(filename, "w");
	if (fp == NULL)
		return FALSE;

	while (*data) {
		if (fputc(*data++, fp) == EOF) {
			fclose(fp);
			return FALSE;
		}
	}


	fclose(fp);

	return TRUE;
}
