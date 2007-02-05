#include "contingsettings.h"
#include "contingstringpool.h"

const gchar *
conting_settings_config_dir(void)
{
	static const gchar *result = NULL;

	if (result == NULL)
	 result = conting_string_pool_add(g_build_filename(g_get_user_config_dir(),
				 CONTING_SETTINGS_DIR, NULL));

	return result;
}

const gchar *
conting_settings_data_dir(void)
{
	static const gchar *result = NULL;

	if (result == NULL)
		result = conting_string_pool_add(g_build_filename(g_get_user_data_dir(),
				CONTING_SETTINGS_DIR, NULL));

	return result;
}

static GKeyFile *file = NULL;

GKeyFile *
conting_settings_get_config_file(void)
{
	if (file == NULL) {
		gchar *filename = g_build_filename(conting_settings_config_dir(),
				CONTING_SETTINGS_CONFIG_FILE, NULL);
		file = g_key_file_new();
		g_key_file_load_from_file(file, filename,
				G_KEY_FILE_KEEP_COMMENTS, NULL);
		g_free(filename);
	}

	return file;
}

void
conting_settings_save(void)
{
}

const gchar *
conting_settings_get_value_default(const gchar *group, const gchar *key,
		const gchar *def)
{
	GKeyFile *f = conting_settings_get_config_file();
	gchar *result = g_key_file_get_value(f, group, key, NULL);

	return result ? conting_string_pool_add(result) : def;
}
