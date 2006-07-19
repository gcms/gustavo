#include "contingdata.h"
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

ContingItemData *
conting_data_get(ContingData *self, ContingDrawing *drawing)
{
	ContingDataPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), NULL);

	priv = CONTING_DATA_GET_PRIVATE(self);

	return g_hash_table_lookup(priv->drawing_data, drawing);
}

void
conting_data_assoc(ContingData *self,
		ContingDrawing *drawing, ContingItemData *data)
{
	ContingDataPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

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
