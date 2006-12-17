#include "contingdata.h"
#include "contingbus.h"
#include "contingline.h"
#include "continggen.h"
#include "contingload.h"
#include "contingtrans2.h"
#include "contingserializable.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* PROPERTIES ENUM */
enum {
    CONTING_DATA_PROP_0,
    CONTING_DATA_N_BUS,
    CONTING_DATA_N_BRANCH,
    CONTING_DATA_TITLE
};

/* PARENT CLASS POINTER */
static gpointer parent_class = NULL;

/* CLASS PRIVATE DATA ACCESSOR MACRO */
#define CONTING_DATA_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_DATA, ContingDataPrivate))

/* CLASS PRIVATE DATA TYPE AND STRUCTURE */
typedef struct ContingDataPrivate_ ContingDataPrivate;
struct ContingDataPrivate_ {
	GList *item_data;

	GHashTable *drawing_data;
	GHashTable *data_drawing;

    const gchar *title;
    guint n_bus, n_branch;

	gboolean loaded;
};

/* PUBLIC METHOD */
GList *
conting_data_get_unassoc(ContingData *self)
{
	ContingDataPrivate *priv;
	GList *n, *result;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), NULL);

	priv = CONTING_DATA_GET_PRIVATE(self);

	result = NULL;
	for (n = priv->item_data; n != NULL; n = g_list_next(n)) {
		if (!g_hash_table_lookup(priv->data_drawing, n->data)) {
			result = g_list_append(result, n->data);
		}
	}

	return result;
}

/* PRIVATE METHOD */
static ContingItemData *
conting_data_get_branch(ContingData *self,
		ContingItemData *bus0, ContingItemData *bus1)
{
	ContingDataPrivate *priv;
	GList *n;
	gint n0, n1;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), NULL);

	priv = CONTING_DATA_GET_PRIVATE(self);

	assert(conting_item_data_get_item_type(bus0) == CONTING_ITEM_TYPE_BUS);
	assert(conting_item_data_get_item_type(bus1) == CONTING_ITEM_TYPE_BUS);

	conting_item_data_get_attr(bus0,
			"number", &n0,
			NULL);
	conting_item_data_get_attr(bus1,
			"number", &n1,
			NULL);
/*
	g_print("find link from \"%d\" to \"%d\"\n", n0, n1);
    */

	for (n = priv->item_data; n != NULL; n = g_list_next(n)) {
		ContingItemData *item = n->data;
		gint z, tap;

		if (conting_item_data_get_item_type(item) != CONTING_ITEM_TYPE_BRANCH)
			continue;

		conting_item_data_get_attr(item,
				"tap bus number", &tap,
				"z bus number", &z,
				NULL);

        /*
		g_print("branch between \"%d\" and \"%d\"\n", tap, z);
        */

		if ((n0 == tap && n1 == z) || (n0 == z && n1 == tap)) {
            /*
			g_print("FOUNd\n");
            */
			return item;
		}
	}
    /*
	g_print("NULL\n");
    */

	return NULL;
}

static gboolean
conting_data_is_bus_pred(ContingDrawing *drawing, gpointer user_data)
{
	ContingDrawing **bus = user_data;

	if (*bus == NULL && CONTING_IS_BUS_BASE(drawing)) {
		*bus = drawing;

		return FALSE;
	}

	return *bus == NULL;
}

/* TODO: change everything in this method. It's all trash */
/* PUBLIC METHOD */
ContingItemData *
conting_data_get(ContingData *self, ContingDrawing *drawing)
{
	ContingDataPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), NULL);

	priv = CONTING_DATA_GET_PRIVATE(self);

	if (CONTING_IS_LINE(drawing)) {
		ContingBusBase *comp0, *comp1;

		conting_line_get_buses(CONTING_LINE(drawing), &comp0, &comp1);

		assert((comp0 == NULL || CONTING_IS_BUS_BASE(comp0))
				&& (comp1 == NULL || CONTING_IS_BUS_BASE(comp1)));
		if (comp0 && comp1) {
			ContingItemData *dcomp0, *dcomp1;

			dcomp0 = conting_data_get(self, CONTING_DRAWING(comp0));
			dcomp1 = conting_data_get(self, CONTING_DRAWING(comp1));

			g_print("dcomp0 = %p\tdcomp1 = %p\n", dcomp0, dcomp1);

			if (dcomp0 && dcomp1) {
				return conting_data_get_branch(self, dcomp0, dcomp1);
			}
		}
	} else if (CONTING_IS_GEN(drawing) || CONTING_IS_LOAD(drawing)) {
		ContingComponent *bus;
		ContingDrawing *linked;

		linked = CONTING_COMPONENT(drawing)->links ?
			CONTING_COMPONENT(drawing)->links->data : NULL;

		if (linked != NULL) {
			bus = NULL;
			conting_drawing_find_link(linked, conting_data_is_bus_pred, &bus);

			if (bus != NULL) {
				return g_hash_table_lookup(priv->drawing_data,
						CONTING_DRAWING(bus));
			}
		}
	} else if (CONTING_IS_TRANS2(drawing)) {
		GList *n;

		for (n = CONTING_COMPONENT(drawing)->links; n; n = g_list_next(n)) {
			return conting_data_get(self, CONTING_DRAWING(n->data));
		}
	} else if (CONTING_IS_TRANS3(drawing)) {
		if (!g_hash_table_lookup(priv->drawing_data, drawing)) {
		}
	}

	return g_hash_table_lookup(priv->drawing_data, drawing);
}

/* PUBLIC METHOD */
void
conting_data_assoc(ContingData *self,
		ContingDrawing *drawing, ContingItemData *data)
{
	ContingDataPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	assert(!CONTING_IS_BUS(self)
			|| (CONTING_IS_BUS(self)
				&& conting_item_data_get_item_type(data) == CONTING_ITEM_TYPE_BUS));

	/* FIXME: inneficient, but who cares? */
	conting_data_unassoc(self, drawing);

	if (data == NULL)
		return;

	g_hash_table_insert(priv->drawing_data, drawing, data);
	g_hash_table_insert(priv->data_drawing, data, drawing);

	g_signal_connect_swapped(G_OBJECT(drawing), "delete",
			G_CALLBACK(conting_data_unassoc), self);
}

/* PUBLIC METHOD */
void
conting_data_unassoc(ContingData *self, ContingDrawing *drawing)
{
	ContingDataPrivate *priv;
	ContingItemData *data;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	data = g_hash_table_lookup(priv->drawing_data, drawing);
	if (data) {
		g_hash_table_remove(priv->drawing_data, drawing);
		g_hash_table_remove(priv->data_drawing, data);
	}
}

void
conting_data_clear(ContingData *self);

void
conting_data_load_file(ContingData *self, ContingFile *file,
		const gchar *filename)
{
	ContingDataPrivate *priv;
	GList *n;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	if (priv->item_data)
		conting_data_clear(self);

	assert(priv->item_data == NULL);

	priv->item_data = conting_file_get_item_data(file, filename);
	for (n = priv->item_data; n != NULL; n = g_list_next(n)) {
        ContingItemType type;
		assert(CONTING_IS_ITEM_DATA(n->data));

        g_object_get(n->data, "type", &type, NULL);

        switch (type) {
            case CONTING_ITEM_TYPE_BUS:
                priv->n_bus++;
                break;
            case CONTING_ITEM_TYPE_BRANCH:
                priv->n_branch++;
                break;
            default:
                break;
        }
	}

	priv->loaded = TRUE;
}

gboolean
conting_data_is_loaded(ContingData *self)
{
	ContingDataPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), FALSE);

	priv = CONTING_DATA_GET_PRIVATE(self);

	return priv->loaded;
}

void
conting_data_clear(ContingData *self)
{
	ContingDataPrivate *priv;
	GList *n;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	for (n = priv->item_data; n != NULL; n = g_list_next(n)) {
		ContingDrawing *drawing = g_hash_table_lookup(priv->data_drawing,
				n->data);

		g_hash_table_steal(priv->drawing_data, drawing);
		g_hash_table_steal(priv->data_drawing, n->data);

		g_object_unref(n->data);
	}

	g_list_free(priv->item_data);
	priv->item_data = NULL;

	priv->loaded = FALSE;
}

ContingError *
conting_error_new(ContingDrawing *drawing, ContingItemData *item_data,
       const gchar *format, ...)
{
	ContingError *err = g_new(ContingError, 1);

	va_list ap;

	va_start(ap, format);

    err->item_data = item_data;
	err->drawing = drawing;
	err->message = g_strdup_vprintf(format, ap);

	va_end(ap);

	return err;
}

void
conting_error_free(ContingError *err)
{
	g_free(err->message);
	g_free(err);
}

gboolean
conting_data_check(ContingData *self, GList **error_list)
{
	ContingDataPrivate *priv;
	GList *n;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), FALSE);

	priv = CONTING_DATA_GET_PRIVATE(self);

	for (n = priv->item_data; n != NULL; n = g_list_next(n)) {
		ContingItemData *item_data = n->data;
		ContingDrawing *drawing = g_hash_table_lookup(priv->data_drawing,
				item_data);

		if (conting_item_data_get_item_type(item_data)
				== CONTING_ITEM_TYPE_BUS && drawing == NULL) {
			if (error_list) {
				*error_list = g_list_append(*error_list,
						conting_error_new(drawing, item_data,
                            "Bus not assoced"));
			} else {
				return FALSE;
			}
		}
	}

	return error_list == NULL || *error_list == NULL;
}

static void
conting_data_finalize(GObject *self)
{
	ContingDataPrivate *priv;
	GList *n;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	for (n = priv->item_data; n != NULL; n = g_list_next(n)) {
		g_object_unref(n->data);
	}
	g_list_free(priv->item_data);

	g_hash_table_destroy(priv->data_drawing);
	g_hash_table_destroy(priv->drawing_data);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void
conting_data_instance_init(GTypeInstance *self, gpointer g_class)
{
	ContingDataPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	priv->item_data = NULL;

	priv->data_drawing = g_hash_table_new(NULL, NULL);
	priv->drawing_data = g_hash_table_new(NULL, NULL);

    priv->n_bus = 0;
    priv->n_branch = 0;

    priv->loaded = FALSE;
}

static void
conting_data_read(ContingSerializable *self, xmlNodePtr node,
		GHashTable *id_drawing)
{
	ContingDataPrivate *priv;
	xmlNodePtr item_node;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	conting_data_clear(CONTING_DATA(self));

	for (item_node = node->children; item_node; item_node = item_node->next) {
		GObject *obj;
		GType type;
		gchar *str;
		ContingDrawing *drawing;

		if (!xmlStrEqual(item_node->name, BAD_CAST "class"))
			continue;

		str = xmlGetProp(item_node, BAD_CAST "name");

		if (str == NULL)
			continue;
		type = g_type_from_name(str);
		xmlFree(str);

		obj = g_object_new(type, NULL);

		if (!CONTING_IS_ITEM_DATA(obj)) {
			g_object_unref(obj);
			continue;
		}

		str = xmlGetProp(item_node, BAD_CAST "drawing");
		if (str) {
			drawing = g_hash_table_lookup(id_drawing,
					GUINT_TO_POINTER(strtoul(str, NULL, 10)));

			if (drawing) {
				conting_data_assoc(CONTING_DATA(self), drawing,
						CONTING_ITEM_DATA(obj));
			}
		}
		xmlFree(str);
		
		conting_serializable_read(CONTING_SERIALIZABLE(obj), item_node,
				id_drawing);

		priv->item_data = g_list_append(priv->item_data, obj);
	}
}

static void
conting_data_write(ContingSerializable *self, xmlNodePtr node,
		xmlNodePtr *result)
{
	ContingDataPrivate *priv;
	xmlNodePtr class_node;
	GList *n;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	class_node = xmlNewNode(NULL, BAD_CAST "data");
	xmlNewProp(class_node, BAD_CAST "class",
			BAD_CAST g_type_name(G_OBJECT_TYPE(self)));


	for (n = priv->item_data; n != NULL; n = g_list_next(n)) {
		xmlNodePtr item_node;
		ContingDrawing *drawing;

		conting_serializable_write(CONTING_SERIALIZABLE(n->data), class_node,
				&item_node);

		drawing = g_hash_table_lookup(priv->data_drawing, n->data);
		if (drawing != NULL) {
			gchar buff[256];
			gint id;

			g_object_get(G_OBJECT(drawing),
					"id", &id,
					NULL);
			sprintf(buff, "%d", id);
			xmlNewProp(item_node, BAD_CAST "drawing",
					BAD_CAST buff);
		}
	}
	
	xmlAddChild(node, class_node);
}

static void
conting_data_serializable_init(gpointer g_iface, gpointer iface_data)
{
	ContingSerializableClass *serial_class;

	serial_class = g_iface;
	serial_class->read = conting_data_read;
	serial_class->write = conting_data_write;
}

static void
conting_data_class_init(gpointer g_class, gpointer class_data)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS(g_class);
	object_class->finalize = conting_data_finalize;

	g_type_class_add_private(g_class, sizeof(ContingDataPrivate));

    parent_class = g_type_class_peek_parent(g_class);

	g_type_class_ref(CONTING_TYPE_ITEM_DATA);
}

GType conting_data_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingDataClass),
            NULL,
            NULL,
            conting_data_class_init,
            NULL,
            NULL,
            sizeof(ContingData),
            0,
            conting_data_instance_init,
            NULL
        };

		static const GInterfaceInfo serial_info = {
			conting_data_serializable_init,
			NULL,
			NULL	
		};

        type = g_type_register_static(G_TYPE_OBJECT,
                "ContingData",
                &type_info, 0);

		g_type_add_interface_static(type,
				CONTING_TYPE_SERIALIZABLE,
				&serial_info);

    }

    return type;
}
