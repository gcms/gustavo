#include "contingattributes.h"

/* PRIVATE DATA MACRO ACCESSOR */
#define CONTING_ATTRIBUTES_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_ATTRIBUTES, ContingAttributesPrivate))

/* PRIVATE DATA STRUCTURE */
typedef struct ContingAttributesPrivate_ ContingAttributesPrivate;
struct ContingAttributesPrivate_ {
    GHashTable *name_value;
};

const GValue *
conting_attributes_get_value(ContingAttributes *self, const gchar *name)
{
    ContingAttributesPrivate *priv;

    g_return_val_if_fail(self != NULL && CONTING_IS_ATTRIBUTES(self), NULL);

    priv = CONTING_ATTRIBUTES_GET_PRIVATE(self);

    return g_hash_table_lookup(priv->name_value, name);
}

gint
conting_attributes_get_int(ContingAttributes *self, const gchar *name)
{
    const GValue *value;

    value = conting_attributes_get_value(self, name);

    g_return_val_if_fail(value != NULL && G_IS_VALUE(value), -1);

    return g_value_get_int(value);
}
gdouble
conting_attributes_get_double(ContingAttributes *self, const gchar *name)
{
    const GValue *value;

    value = conting_attributes_get_value(self, name);

    g_return_val_if_fail(value != NULL && G_IS_VALUE(value), -1.0);

    return g_value_get_double(value);
}
const gchar *
conting_attributes_get_string(ContingAttributes *self, const gchar *name)
{
    const GValue *value;

    value = conting_attributes_get_value(self, name);

    g_return_val_if_fail(value != NULL && G_IS_VALUE(value), NULL);

    return g_value_get_string(value);
}

void
conting_attributes_get_valist(ContingAttributes *self,
        const gchar *first, va_list ap)
{
    g_return_if_fail(self != NULL && CONTING_IS_ATTRIBUTES(self));

    while (first) {
        gpointer *pointer;
        const GValue *value;

        pointer = va_arg(ap, gpointer *);

        value = conting_attributes_get_value(self, first);

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

        first = va_arg(ap, const gchar *);
    }
}
void
conting_attributes_get(ContingAttributes *self, const gchar *first, ...)
{
    va_list ap;

    va_start(ap, first);

    conting_attributes_get_valist(self, first, ap);

    va_end(ap);
}
