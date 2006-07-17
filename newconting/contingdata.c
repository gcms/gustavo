#include "contingdata.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

static gpointer parent_class = NULL;

#define CONTING_DATA_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_DATA, ContingDataPrivate))

typedef struct ContingDataPrivate_ ContingDataPrivate;
struct ContingDataPrivate_ {
	GList *bus_data;
	GList *branch_data;

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
	for (n = priv->bus_data; n != NULL; n = g_list_next(n)) {
		if (!g_hash_table_lookup(priv->data_drawing, n->data)) {
			result = g_list_append(result, n->data);
		}
	}

	return result;
}

data_t *
conting_data_get(ContingData *self, ContingDrawing *drawing)
{
	ContingDataPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), NULL);

	priv = CONTING_DATA_GET_PRIVATE(self);

	return g_hash_table_lookup(priv->drawing_data, drawing);
}

void
conting_data_assoc(ContingData *self, ContingDrawing *drawing, data_t *data)
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
	data_t *data;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	data = g_hash_table_lookup(priv->drawing_data, drawing);
	if (data) {
		g_hash_table_remove(priv->drawing_data, drawing);
		g_hash_table_remove(priv->data_drawing, data);
	}
}

void
conting_data_get_data_attr(data_t *data, const gchar *attr, ...)
{
	va_list ap;
	GValue *value;
	gpointer *pointer;

	va_start(ap, attr);

	while (attr) {
		pointer = va_arg(ap, gpointer);

		value = g_hash_table_lookup(data->attrs, attr);
		if (value == NULL) {
			pointer = NULL;
			continue;
		}

		switch (G_VALUE_TYPE(value)) {
			case G_TYPE_INT:
				*((gint *) pointer) = g_value_get_int(value);
				break;
			case G_TYPE_STRING:
				*((const gchar **) pointer) = g_value_get_string(value);
				break;
			case G_TYPE_DOUBLE:
				*((gdouble *) pointer) = g_value_get_double(value);
				break;
			case G_TYPE_POINTER:
			default:
				*pointer = g_value_get_pointer(value);
				break;
		}

		attr = va_arg(ap, const gchar *);
	}

	va_end(ap);
}

void
conting_data_set_data_attr(data_t *data, const gchar *attr, ...)
{
	GType type;
	GValue *value;
	va_list ap;

	va_start(ap, attr);

	while (attr) {
		type = va_arg(ap, GType);
		value = g_new(GValue, 1);
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
			default:
				g_value_set_pointer(value, va_arg(ap, gpointer));
				break;
		}
		g_hash_table_insert(data->attrs, g_strdup(attr), value);

		attr = va_arg(ap, const gchar *);
	}

	va_end(ap);
}

void
conting_data_load_file(ContingData *self, const gchar *filename)
{
	ContingDataPrivate *priv;
	FILE *fp;
	char buf[256];

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	fp = fopen(filename, "r");

	if (fp == NULL)
		return;

	while (fgets(buf, 256, fp)) {
		if (strncmp(buf, "BUS", 3) == 0) {
			while (fgets(buf, 256, fp)) {
				data_t *data = g_new(data_t, 1);
				data->attrs = g_hash_table_new_full(g_str_hash, g_str_equal,
						g_free, g_free);
				if (strncmp(buf, "-999", 4) == 0)
					break;

				data->type = BUS;
				data->used = NULL;

				conting_file_bus_data(&data->data.bus, buf);

				conting_data_set_data_attr(data,
						"name", G_TYPE_STRING, data->data.bus.name,
						"number", G_TYPE_INT, data->data.bus.number,
						"final_voltage",
						G_TYPE_DOUBLE, data->data.bus.final_voltage,
						NULL);

				priv->bus_data = g_list_append(priv->bus_data, data);
			}
		} else if (strncmp(buf, "BRANCH", 6) == 0) {
			while (fgets(buf, 256, fp)) {
				data_t *data = g_new(data_t, 1);
				data->attrs = g_hash_table_new_full(g_str_hash, g_str_equal,
						g_free, g_free);
				if (strncmp(buf, "-999", 4) == 0)
					break;

				data->type = BRANCH;
				data->used = NULL;

				conting_file_branch_data(&data->data.branch, buf);

				conting_data_set_data_attr(data,
						"tap_bus_number",
						G_TYPE_INT, data->data.branch.tap_bus_number,
						NULL);

				priv->branch_data = g_list_append(priv->branch_data, data);
			}
		}
	}
}

const GList *
conting_data_get_bus(ContingData *self)
{
	ContingDataPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), NULL);

	priv = CONTING_DATA_GET_PRIVATE(self);

	return priv->bus_data;
}

const GList *
conting_data_get_branch(ContingData *self)
{
	ContingDataPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_DATA(self), NULL);

	priv = CONTING_DATA_GET_PRIVATE(self);

	return priv->branch_data;
}

static void
conting_data_finalize(GObject *self)
{
	ContingDataPrivate *priv;
	GList *n;

	g_return_if_fail(self != NULL && CONTING_IS_DATA(self));

	priv = CONTING_DATA_GET_PRIVATE(self);

	for (n = priv->bus_data; n != NULL; n = g_list_next(n)) {
		g_free(n->data);
	}
	g_list_free(priv->bus_data);
	for (n = priv->branch_data; n != NULL; n = g_list_next(n)) {
		g_free(n->data);
	}
	g_list_free(priv->branch_data);

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

	priv->bus_data = NULL;
	priv->branch_data = NULL;

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
