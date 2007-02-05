#include "contingconfig.h"

static gpointer parent_class = NULL;
#define CONTING_CONFIG_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE(\
			(o), CONTING_TYPE_CONFIG, ContingConfigPrivate))

typedef struct ContingConfigPrivate_ ContingConfigPrivate;
struct ContingConfigPrivate_ {
	ContingConfigGroup *current_group;
	GList *groups;

	GList *items;
	GHashTable *name_item;
};

static void
conting_config_item_destroy(gpointer data)
{
	ContingConfigItem *item;

	g_print("conting_config_item_destroy()\n");
	g_param_spec_unref(item->spec);
	g_free(item);
}

static void
conting_config_group_destroy(gpointer data)
{
	ContingConfigGroup *group = data;
	g_free(group->name);
	g_list_free(group->items);
	g_free(group);
}

const GParamSpec *
conting_config_get_item(ContingConfig *self, const gchar *name)
{
	ContingConfigPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_CONFIG(self), NULL);

	priv = CONTING_CONFIG_GET_PRIVATE(self);

	return g_hash_table_lookup(priv->name_item, name);
}

const GList *
conting_config_get_items(ContingConfig *self)
{
	ContingConfigPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_CONFIG(self), NULL);

	priv = CONTING_CONFIG_GET_PRIVATE(self);

	return priv->items;
}

const GList *
conting_config_get_groups(ContingConfig *self)
{
	ContingConfigPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_CONFIG(self), NULL);

	priv = CONTING_CONFIG_GET_PRIVATE(self);

	return priv->groups;
}

void
conting_config_new_group(ContingConfig *self, const gchar *name)
{
	ContingConfigPrivate *priv;
	ContingConfigGroup *group;

	g_return_if_fail(self != NULL && CONTING_IS_CONFIG(self));

	priv = CONTING_CONFIG_GET_PRIVATE(self);

	group = g_new(ContingConfigGroup, 1);
	group->name = g_strdup(name);
	group->items = NULL;

	priv->groups = g_list_append(priv->groups, group);
	priv->current_group = group;
}

void
conting_config_add_item_full(ContingConfig *self,
		GParamSpec *spec,
		ContingConfigMapFunc mapper, gpointer user_data)
{
	ContingConfigPrivate *priv;
	ContingConfigItem *item;

	g_return_if_fail(self != NULL && CONTING_IS_CONFIG(self));

	priv = CONTING_CONFIG_GET_PRIVATE(self);

	if (priv->current_group == NULL) {
		conting_config_new_group(self, CONTING_CONFIG_DEFAULT_GROUP);
	}

	item = g_new(ContingConfigItem, 1);
	item->spec = spec;
	item->mapper = mapper;
	item->user_data = user_data;

	priv->current_group->items = g_list_append(priv->current_group->items,
			item);

	priv->items = g_list_append(priv->items, item);
	g_hash_table_insert(priv->name_item,
			g_strdup(g_param_spec_get_name(spec)), item);
}


static void
conting_config_finalize(GObject *self)
{
	ContingConfigPrivate *priv;
	GList *n;

	g_print("conting_config_finalize()\n");
	g_return_if_fail(self != NULL && CONTING_IS_CONFIG(self));

	priv = CONTING_CONFIG_GET_PRIVATE(self);

	for (n = priv->groups; n != NULL; n = g_list_next(n)) {
		conting_config_group_destroy(n->data);
	}
	g_list_free(priv->groups);

	g_list_free(priv->items);
	g_hash_table_destroy(priv->name_item);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void
conting_config_instance_init(GTypeInstance *self, gpointer g_class)
{
	ContingConfigPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_CONFIG(self));

	priv = CONTING_CONFIG_GET_PRIVATE(self);

	priv->current_group = NULL;
	priv->groups = NULL;

	priv->items = NULL;
	priv->name_item = g_hash_table_new_full(g_str_hash, g_str_equal,
			g_free, conting_config_item_destroy);
}

static void
conting_config_class_init(gpointer g_class, gpointer class_data)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS(g_class);
	gobject_class->finalize = conting_config_finalize;

	parent_class = g_type_class_peek_parent(g_class);
	g_type_class_add_private(g_class, sizeof(ContingConfigPrivate));
}

GType
conting_config_get_type(void)
{
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingConfigClass),
            NULL,
            NULL,
            conting_config_class_init,
            NULL,
            NULL,
            sizeof(ContingConfig),
            0,
            conting_config_instance_init,
            NULL
        };

        type = g_type_register_static(G_TYPE_OBJECT,
				"ContingConfig",
                &type_info, 0);

    }

    return type;
}
