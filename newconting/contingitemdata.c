#include "contingitemdata.h"
#include <string.h>

static gpointer parent_class = NULL;

enum {
	CONTING_ITEM_DATA_PROP_0,
	CONTING_ITEM_DATA_PROP_TYPE
};

#define CONTING_ITEM_DATA_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE((o), \
            CONTING_TYPE_ITEM_DATA, ContingItemDataPrivate))

typedef struct ContingItemDataPrivate_ ContingItemDataPrivate;
struct ContingItemDataPrivate_ {
	ContingItemType type;
	GTree *attrs;
};

void
conting_item_data_set_attr(ContingItemData *self,
		const gchar *attr, ...)
{
	ContingItemDataPrivate *priv;
	GType type;
	GValue *value;
	va_list ap;

	g_return_if_fail(self != NULL && CONTING_IS_ITEM_DATA(self));

	priv = CONTING_ITEM_DATA_GET_PRIVATE(self);

	va_start(ap, attr);

	while (attr) {
		type = va_arg(ap, GType);
		value = g_new(GValue, 1);
		memset(value, 0, sizeof(GValue));
		g_value_init(value, type);

		switch (type) {
			case G_TYPE_INT:
				g_value_set_int(value, va_arg(ap, gint));
				break;
			case G_TYPE_DOUBLE:
				g_value_set_double(value, va_arg(ap, gdouble));
				break;
			case G_TYPE_STRING:
				g_value_set_string(value, va_arg(ap, const gchar *));
				break;
			case G_TYPE_POINTER:
			default:
				g_value_set_pointer(value, va_arg(ap, gpointer));
				break;
		}

		g_tree_insert(priv->attrs, g_strdup(attr), value);

		attr = va_arg(ap, const gchar *);
	}

	va_end(ap);
}

void
conting_item_data_get_attr(ContingItemData *self,
		const gchar *attr, ...)
{
	ContingItemDataPrivate *priv;
	GValue *value;
	gpointer *pointer;
	va_list ap;

	g_return_if_fail(self != NULL && CONTING_IS_ITEM_DATA(self));

	priv = CONTING_ITEM_DATA_GET_PRIVATE(self);

	va_start(ap, attr);

	while (attr) {
		pointer = va_arg(ap, gpointer *);

		value = g_tree_lookup(priv->attrs, attr);

		if (value == NULL) {
			*pointer = NULL;
		} else {
			switch (G_VALUE_TYPE(value)) {
				case G_TYPE_INT:
					*((gint *) pointer) = g_value_get_int(value);
					break;
				case G_TYPE_DOUBLE:
					*((gdouble *) pointer) = g_value_get_double(value);
					break;
				case G_TYPE_STRING:
					*((const gchar **) pointer) = g_value_get_string(value);
					break;
				case G_TYPE_POINTER:
				default:
					*pointer = g_value_get_pointer(value);
					break;
			}
		}

		attr = va_arg(ap, const gchar *);
	}

	va_end(ap);
}


static gboolean
tree_traverse(gpointer key, gpointer value, gpointer user_data)
{
	gpointer *parameters = user_data;
	ContingItemDataAttrFunc func = parameters[0];
	gpointer data = parameters[1];

	func(key, value, data);

	return FALSE;
}

void
conting_item_data_attr_foreach(ContingItemData *self,
		ContingItemDataAttrFunc func, gpointer user_data)
{
	ContingItemDataPrivate *priv;
	gpointer parameters[2];

	g_return_if_fail(self != NULL && CONTING_IS_ITEM_DATA(self));

	priv = CONTING_ITEM_DATA_GET_PRIVATE(self);

	parameters[0] = func;
	parameters[1] = user_data;

	g_tree_traverse(priv->attrs, tree_traverse, G_IN_ORDER, parameters);
}

static void
conting_item_data_get_property(GObject *self,
                               guint prop_id,
                               GValue *value,
                               GParamSpec *pspec)
{
    ContingItemDataPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ITEM_DATA(self));

    priv = CONTING_ITEM_DATA_GET_PRIVATE(self);

    switch (prop_id) {
		case CONTING_ITEM_DATA_PROP_TYPE:
            g_value_set_enum(value, priv->type);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

static void
conting_item_data_set_property(GObject *self,
                               guint prop_id,
                               const GValue *value,
                               GParamSpec *pspec)
{
    ContingItemDataPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ITEM_DATA(self));

    priv = CONTING_ITEM_DATA_GET_PRIVATE(self);

    switch (prop_id) {
		case CONTING_ITEM_DATA_PROP_TYPE:
			priv->type = g_value_get_enum(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

static gint
key_compare_string(gconstpointer a, gconstpointer b, gpointer user_data)
{
	return strcmp(a, b);
}

static void
conting_item_data_instance_init(GTypeInstance *self, gpointer g_class)
{
	ContingItemDataPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_ITEM_DATA(self));

	priv = CONTING_ITEM_DATA_GET_PRIVATE(self);
	
	priv->attrs = g_tree_new_full(key_compare_string, NULL,
			g_free, g_free);
}

static void
conting_item_data_finalize(GObject *self)
{
	ContingItemDataPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_ITEM_DATA(self));

	priv = CONTING_ITEM_DATA_GET_PRIVATE(self);

	g_tree_destroy(priv->attrs);
	
	G_OBJECT_CLASS(parent_class)->finalize(self);
}
ContingItemType
conting_item_data_get_item_type(ContingItemData *self)
{
	ContingItemType type;

	g_object_get(self,
			"type", &type,
			NULL);

	return type;
}

static void
conting_item_data_class_init(gpointer g_class, gpointer class_data)
{
	GObjectClass *gobject_class;

	gobject_class = G_OBJECT_CLASS(g_class);
	gobject_class->set_property = conting_item_data_set_property;
	gobject_class->get_property = conting_item_data_get_property;
	gobject_class->finalize = conting_item_data_finalize;

	g_object_class_install_property(gobject_class,
			CONTING_ITEM_DATA_PROP_TYPE,
			g_param_spec_enum("type",
				              "type",
							  "Item type",
							  CONTING_TYPE_ITEM_TYPE,
							  CONTING_ITEM_TYPE_BUS,
							  G_PARAM_READABLE | G_PARAM_WRITABLE
							  |G_PARAM_CONSTRUCT_ONLY));
			
	g_type_class_add_private(g_class, sizeof(ContingItemDataPrivate));
	parent_class = g_type_class_peek_parent(g_class);
}

GType
conting_item_data_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static GTypeInfo type_info = {
			sizeof(ContingItemDataClass),
			NULL,
			NULL,
			conting_item_data_class_init,
			NULL,
			NULL,
			sizeof(ContingItemData),
			0,
			conting_item_data_instance_init,
			NULL
		};

		type = g_type_register_static(G_TYPE_OBJECT,
				"ContingItemData",
				&type_info, 0);
	}

	return type;
}

GType
conting_item_type_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static GEnumValue conting_item_type_enum[] = {
			{ CONTING_ITEM_TYPE_BUS, "CONTING_ITEM_TYPE_BUS", "BUS" },
			{ CONTING_ITEM_TYPE_BRANCH, "CONTING_ITEM_TYPE_BRANCH", "BRANCH" }
		};
		type = g_enum_register_static("ContingItemType",
				conting_item_type_enum);
	}

	return type;
}
