#ifndef CONTING_SETTINGS_H
#define CONTING_SETTINGS_H

#define CONTING_SETTINGS_DIR "newconting"

#define CONTING_SETTINGS_CONFIG_FILE "newconting.conf"

#include <glib.h>

const gchar *
conting_settings_config_dir(void);

const gchar *
conting_settings_data_dir(void);

GKeyFile *
conting_settings_get_config_file(void);

void
conting_settings_save(void);

#define conting_settings_get_value(g, k) \
	conting_settings_get_value_default(g, k, NULL)
const gchar *
conting_settings_get_value_default(const gchar *group, const gchar *key,
		const gchar *def);

#endif /* CONTING_SETTINGS_H */
