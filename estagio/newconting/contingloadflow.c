#include "contingloadflow.h"

static gpointer parent_class = NULL;
#define CONTING_LOAD_FLOW_CONFIG_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE(\
			(o), CONTING_TYPE_LOAD_FLOW_CONFIG, ContingLoadFlowConfigPrivate))

typedef struct ContingLoadFlowConfigPrivate_ ContingLoadFlowConfigPrivate;
struct ContingLoadFlowConfigPrivate_ {
	GString *string;

	int count;
	gboolean valid;
};

void
conting_load_flow_config_end_section(ContingLoadFlowConfig *self)
{
	ContingLoadFlowConfigPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_LOAD_FLOW_CONFIG(self));

	priv = CONTING_LOAD_FLOW_CONFIG_GET_PRIVATE(self);

	g_string_append(priv->string, "9999\n");

	priv->count = 1;
	priv->valid = TRUE;
}

void
conting_load_flow_config_add_int(ContingLoadFlowConfig *self, gint v)
{
	ContingLoadFlowConfigPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_LOAD_FLOW_CONFIG(self));

	priv = CONTING_LOAD_FLOW_CONFIG_GET_PRIVATE(self);

	g_string_append_printf(priv->string, "   %d %d\n", priv->count, v);

	priv->count++;
	priv->valid = FALSE;
}
void
conting_load_flow_config_add_double(ContingLoadFlowConfig *self, gdouble v)
{
	ContingLoadFlowConfigPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_LOAD_FLOW_CONFIG(self));

	priv = CONTING_LOAD_FLOW_CONFIG_GET_PRIVATE(self);

	g_string_append_printf(priv->string, "   %d %lf\n", priv->count, v);

	priv->count++;
	priv->valid = FALSE;
}

const gchar *
conting_load_flow_config_get_text(ContingLoadFlowConfig *self)
{
	ContingLoadFlowConfigPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_LOAD_FLOW_CONFIG(self),
			NULL);

	priv = CONTING_LOAD_FLOW_CONFIG_GET_PRIVATE(self);

	if (!priv->valid)
		return NULL;

	return priv->string->str;
}

static void
conting_load_flow_config_finalize(GObject *self)
{
	ContingLoadFlowConfigPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_LOAD_FLOW_CONFIG(self));

	priv = CONTING_LOAD_FLOW_CONFIG_GET_PRIVATE(self);

	g_string_free(priv->string, TRUE);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void
conting_load_flow_config_instance_init(GTypeInstance *self, gpointer g_class)
{
	ContingLoadFlowConfigPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_LOAD_FLOW_CONFIG(self));

	priv = CONTING_LOAD_FLOW_CONFIG_GET_PRIVATE(self);

	priv->string = g_string_sized_new(1024);
	priv->count = 1;
	priv->valid = FALSE;
}

static void
conting_load_flow_config_class_init(gpointer g_class, gpointer class_data)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS(g_class);
	gobject_class->finalize = conting_load_flow_config_finalize;

	parent_class = g_type_class_peek_parent(g_class);
	g_type_class_add_private(g_class, sizeof(ContingLoadFlowConfigPrivate));
}

GType
conting_load_flow_config_get_type(void)
{
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingLoadFlowConfigClass),
            NULL,
            NULL,
            conting_load_flow_config_class_init,
            NULL,
            NULL,
            sizeof(ContingLoadFlowConfig),
            0,
            conting_load_flow_config_instance_init,
            NULL
        };

        type = g_type_register_static(G_TYPE_OBJECT,
				"ContingLoadFlowConfig",
                &type_info, 0);

    }

    return type;
}
