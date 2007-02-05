#ifndef CONTING_CONFIG_H
#define CONTING_CONFIG_H


#define CONTING_TYPE_CONFIG        (conting_config_get_type())
#define CONTING_CONFIG(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_CONFIG, ContingConfig))
#define CONTING_CONFIG_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_CONFIG, ContingConfigClass))
#define CONTING_IS_CONFIG(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_CONFIG))
#define CONTING_IS_CONFIG_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_CONFIG))
#define CONTING_CONFIG_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_CONFIG, ContingConfigClass))

#define CONTING_CONFIG_DEFAULT_GROUP ""
typedef struct ContingConfig_ ContingConfig;
typedef struct ContingConfigClass_ ContingConfigClass;

#include <glib-object.h>

struct ContingConfig_ {
    GObject parent;
};

struct ContingConfigClass_ {
	GObjectClass parent;
};

GType
conting_config_get_type(void);

void
conting_config_new_group(ContingConfig *self, const gchar *name);

typedef gpointer (*ContingConfigMapFunc)(GParamSpec *spec, gpointer user_data);

#define conting_config_add_item(c, s) conting_config_add_item_full(c, s, \
		NULL, NULL)
void
conting_config_add_item_full(ContingConfig *self,
		GParamSpec *spec,
		ContingConfigMapFunc mapper, gpointer user_data);

typedef struct ContingConfigGroup_ {
	gchar *name;
	GList *items;
} ContingConfigGroup;

typedef struct ContingConfigItem_ {
	GParamSpec *spec;

	ContingConfigMapFunc mapper;
	gpointer user_data;
} ContingConfigItem;

const GParamSpec *
conting_config_get_item(ContingConfig *self, const gchar *name);
const GList *
conting_config_get_items(ContingConfig *self);
const GList *
conting_config_get_groups(ContingConfig *self);

#endif /* CONTING_CONFIG_H */
