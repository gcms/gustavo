#include "contingitemdata.h"
#include "contingserializable.h"
#include "contingxml.h"
#include "contingdata.h"
#include <string.h>
#include <assert.h>

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
	GTree *attrs;	/* <const gchar *><GValue *> */
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

	g_tree_foreach(priv->attrs, tree_traverse, parameters);
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
conting_item_data_read(ContingSerializable *self, xmlNodePtr node,
		GHashTable *id_drawing)
{
	ContingItemDataPrivate *priv;
	GEnumValue *enum_value;
	gchar *str;
	xmlNodePtr attr_node;

	g_return_if_fail(self != NULL && CONTING_IS_ITEM_DATA(self));

	priv = CONTING_ITEM_DATA_GET_PRIVATE(self);

	str = xmlGetProp(node, BAD_CAST "type");
	assert(str);

	enum_value = g_enum_get_value_by_name(
			g_type_class_peek(CONTING_TYPE_ITEM_TYPE), str);
	priv->type = enum_value->value;

	for (attr_node = node->children; attr_node; attr_node = attr_node->next) {
		gchar *name;
		GType type;
		gchar *value;

		if (!xmlStrEqual(attr_node->name, BAD_CAST "attribute"))
			continue;

		name = xmlGetProp(attr_node, BAD_CAST "name");
		
		str = xmlGetProp(attr_node, BAD_CAST "type");
		type = g_type_from_name(str);
		xmlFree(str);

		value = xmlNodeListGetString(attr_node->doc, attr_node->children,
				TRUE);

		switch (type) {
			case G_TYPE_INT:
				conting_item_data_set_attr(CONTING_ITEM_DATA(self),
						name, type, atoi(value), NULL);
				break;
			case G_TYPE_DOUBLE:
				conting_item_data_set_attr(CONTING_ITEM_DATA(self),
						name, type, strtod(value, NULL), NULL);
				break;
			case G_TYPE_STRING:
				conting_item_data_set_attr(CONTING_ITEM_DATA(self),
						name, type, value, NULL);
				break;
			case G_TYPE_POINTER:
			default:
				break;
		}

		xmlFree(name);
		xmlFree(value);
	}
}

static gboolean
tree_traverse_xml(gpointer key, gpointer value, gpointer user_data)
{
	xmlNodePtr attr_node;
	xmlNodePtr class_node = user_data;
	const GValue *gvalue = value;
	const gchar *name = key;
	gchar buff[256];

	attr_node = xmlNewNode(NULL, BAD_CAST "attribute");
	xmlNewProp(attr_node, BAD_CAST "name",
			BAD_CAST name);
	xmlNewProp(attr_node, BAD_CAST "type",
			BAD_CAST g_type_name(G_VALUE_TYPE(gvalue)));

	switch (G_VALUE_TYPE(gvalue)) {
		case G_TYPE_INT:
			sprintf(buff, "%d", g_value_get_int(gvalue));
			break;
		case G_TYPE_DOUBLE:
			sprintf(buff, "%lf", g_value_get_double(gvalue));
			break;
		case G_TYPE_STRING:
			sprintf(buff, "%s", g_value_get_string(gvalue));
			break;
		case G_TYPE_POINTER:
		default:
			break;
	}

	xmlAddChild(attr_node, xmlNewText(BAD_CAST buff));
	xmlAddChild(class_node, attr_node);

	return FALSE;
}
	
static void
conting_item_data_write(ContingSerializable *self, xmlNodePtr node,
		xmlNodePtr *result)
{
	ContingItemDataPrivate *priv;
	xmlNodePtr class_node;
	GEnumValue *enum_val;

	g_return_if_fail(self != NULL && CONTING_IS_ITEM_DATA(self));

	priv = CONTING_ITEM_DATA_GET_PRIVATE(self);

	class_node = xmlNewNode(NULL, BAD_CAST "class");
	xmlNewProp(class_node, BAD_CAST "name",
			BAD_CAST g_type_name(G_OBJECT_TYPE(self)));
	enum_val = g_enum_get_value(g_type_class_peek(CONTING_TYPE_ITEM_TYPE),
			priv->type);
	xmlNewProp(class_node, BAD_CAST "type",
			BAD_CAST enum_val->value_name);

	g_tree_foreach(priv->attrs, tree_traverse_xml, class_node);
	
	xmlAddChild(node, class_node);

	*result = class_node;
}

static void
conting_item_data_serializable_init(gpointer g_iface, gpointer iface_data)
{
	ContingSerializableClass *serial_class;

	serial_class = g_iface;
	serial_class->read = conting_item_data_read;
	serial_class->write = conting_item_data_write;
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

		
		static const GInterfaceInfo serial_info = {
			conting_item_data_serializable_init,
			NULL,
			NULL	
		};

		g_type_add_interface_static(type,
				CONTING_TYPE_SERIALIZABLE,
				&serial_info);
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
