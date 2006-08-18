#include "contingdata.h"
#include "contingbus.h"
#include "contingline.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

static gpointer parent_class = NULL;

#define CONTING_DATA_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_DATA, ContingDataPrivate))

typedef struct ContingDataPrivate_ ContingDataPrivate;
struct ContingDataPrivate_ {
	GList *item_data;

	GHashTable *drawing_data;
	GHashTable *data_drawing;
};

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

	for (n = priv->item_data; n != NULL; n = g_list_next(n)) {
		ContingItemData *item = n->data;
		gint z, tap;

		if (conting_item_data_get_item_type(item) != CONTING_ITEM_TYPE_BRANCH)
			continue;

		conting_item_data_get_attr(item,
				"tap bus number", &tap,
				"z bus number", &z,
				NULL);

		if ((n0 == tap && n1 == z) || (n0 == z && n1 == tap))
			return item;
	}

	return NULL;
}


ContingItemData *
conting_data_get(ContingData *self, ContingDrawing *drawing)
{
	ContingDataPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), NULL);

	priv = CONTING_DATA_GET_PRIVATE(self);

	if (CONTING_IS_LINE(drawing)) {
		ContingComponent *comp0, *comp1;

		conting_line_get_buses(CONTING_LINE(drawing), &comp0, &comp1);

		assert((comp0 == NULL || CONTING_IS_BUS(comp0))
				&& (comp1 == NULL || CONTING_IS_BUS(comp1)));
		if (comp0 && comp1) {
			ContingItemData *dcomp0, *dcomp1;

			dcomp0 = conting_data_get(self, CONTING_DRAWING(comp0));
			dcomp1 = conting_data_get(self, CONTING_DRAWING(comp1));

			if (dcomp0 && dcomp1) {
				return conting_data_get_branch(self, dcomp0, dcomp1);
			}
		}
	}

	return g_hash_table_lookup(priv->drawing_data, drawing);
}

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
}

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

static void
conting_data_clear(ContingData *self);

void
conting_data_load_file(ContingData *self, const gchar *filename)
{
	ContingDataPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	if (priv->item_data)
		conting_data_clear(self);

	assert(priv->item_data == NULL);

	priv->item_data = conting_file_get_item_data(filename);
}

static void
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
}

static void
conting_data_class_init(gpointer g_class, gpointer class_data)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS(g_class);
	object_class->finalize = conting_data_finalize;

	g_type_class_add_private(g_class, sizeof(ContingDataPrivate));

    parent_class = g_type_class_peek_parent(g_class);
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

        type = g_type_register_static(G_TYPE_OBJECT,
                "ContingData",
                &type_info, 0);
    }

    return type;
}
