#include "contingdata.h"
#include "contingbus.h"
#include "contingline.h"
#include "continggen.h"
#include "contingload.h"
#include "contingtrans2.h"
#include "contingserializable.h"
#include "contingmodel.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>


/* PARENT CLASS POINTER */
static gpointer parent_class = NULL;

/* CLASS PRIVATE DATA ACCESSOR MACRO */
#define CONTING_DATA_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_DATA, ContingDataPrivate))

/* CLASS PRIVATE DATA TYPE AND STRUCTURE */
typedef struct ContingDataPrivate_ ContingDataPrivate;
struct ContingDataPrivate_ {
	ContingModel *model;

	GHashTable *drawing_data;
	GHashTable *data_drawing;

	gchar *raw_data;
};

/* PUBLIC METHOD */
GList *
conting_data_get_unassoc(ContingData *self)
{
	ContingDataPrivate *priv;
	const GList *n;
	GList *result;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), NULL);

	priv = CONTING_DATA_GET_PRIVATE(self);

	result = NULL;
	for (n = conting_model_get_buses(priv->model); n != NULL;
			n = g_list_next(n)) {
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
	gint n0, n1;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), NULL);

	priv = CONTING_DATA_GET_PRIVATE(self);

	assert(conting_item_data_get_item_type(bus0) == CONTING_ITEM_TYPE_BUS);
	assert(conting_item_data_get_item_type(bus1) == CONTING_ITEM_TYPE_BUS);


	n0 = conting_item_data_bus_number(bus0);
	n1 = conting_item_data_bus_number(bus1);

	return conting_model_get_branch(priv->model, n0, n1);
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
/*
			g_print("dcomp0 = %p\tdcomp1 = %p\n", dcomp0, dcomp1);
			*/

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

/*
void
conting_data_load_list(ContingData *self, GList *list)
{
	ContingDataPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	conting_model_load_list(priv->model, list);
}
*/


const gchar *
conting_data_get_raw_data(ContingData *self)
{
	ContingDataPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), NULL);

	priv = CONTING_DATA_GET_PRIVATE(self);

	return priv->raw_data;
}

/* TODO: Change it to contingmodel, it's just not done yet because we need
 * to read/write from the data files. When the read/write code changes to model
 * it should go along. */
#include "contingstringfile.h"
static void
conting_data_load_raw_data(ContingData *self, const gchar *filename)
{
	ContingDataPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);
	
	priv->raw_data = conting_string_file_load(filename);
}

void
conting_data_load_file(ContingData *self, ContingFile *file,
		const gchar *filename)
{
	ContingDataPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	conting_model_load_file(priv->model, file, filename);

	conting_data_load_raw_data(self, filename);
}
gboolean
conting_data_is_loaded(ContingData *self)
{
	ContingDataPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), FALSE);

	priv = CONTING_DATA_GET_PRIVATE(self);

	return conting_model_loaded(priv->model);
}

void
conting_data_clear(ContingData *self)
{
	ContingDataPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	if (priv->raw_data) {
		g_free(priv->raw_data);
		priv->raw_data = NULL;
	}

	conting_model_clear(priv->model);

	g_hash_table_remove_all(priv->drawing_data);
	g_hash_table_remove_all(priv->data_drawing);
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
	const GList *n;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), FALSE);

	priv = CONTING_DATA_GET_PRIVATE(self);

	for (n = conting_model_get_buses(priv->model); n != NULL;
			n = g_list_next(n)) {
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

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);


	g_object_unref(priv->model);

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

	priv->model = conting_model_new_default();

	priv->data_drawing = g_hash_table_new(NULL, NULL);
	priv->drawing_data = g_hash_table_new(NULL, NULL);

	priv->raw_data = NULL;
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

		if (xmlStrEqual(item_node->name, BAD_CAST "raw")) {
			priv->raw_data = xmlNodeListGetString(item_node->doc,
					item_node->children, TRUE);
			g_print(priv->raw_data);
			continue;
		}
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

		/* This method should be removed from the public interface.
		 * The data from file should be loaded into the ContingModel
		 * instead of the ContingData. */
		conting_model_add_item(priv->model, obj);
	}
}

static void
conting_data_write(ContingSerializable *self, xmlNodePtr node,
		xmlNodePtr *result)
{
	ContingDataPrivate *priv;
	xmlNodePtr class_node, raw_node;
	const GList *n;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	class_node = xmlNewNode(NULL, BAD_CAST "data");
	xmlNewProp(class_node, BAD_CAST "class",
			BAD_CAST g_type_name(G_OBJECT_TYPE(self)));
	
	raw_node = xmlNewNode(NULL, BAD_CAST "raw");
	xmlAddChild(raw_node, xmlNewText(BAD_CAST priv->raw_data));
	xmlAddChild(class_node, raw_node);


	for (n = conting_model_get_items(priv->model); n != NULL;
			n = g_list_next(n)) {
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
