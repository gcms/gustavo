#ifndef CONTING_ATTRIBUTES_H
#define CONTING_ATTRIBUTES_H

#include <glib-object.h>

#define CONTING_TYPE_ATTRIBUTES        (conting_attributes_get_type())
#define CONTING_ATTRIBUTES(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_ATTRIBUTES, ContingAttributes))
#define CONTING_ATTRIBUTES_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_ATTRIBUTES, ContingAttributesClass))
#define CONTING_IS_ATTRIBUTES(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_ATTRIBUTES))
#define CONTING_IS_ATTRIBUTES_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_ATTRIBUTES))
#define CONTING_ATTRIBUTES_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_ATTRIBUTES, ContingAttributesClass))

typedef struct ContingAttributes_ ContingAttributes;
typedef struct ContingAttributesClass_ ContingAttributesClass;

struct ContingAttributes_ {
    GObject parent;
};

struct ContingAttributesClass_ {
    GObjectClass parent;
};

GType conting_attributes_get_type(void);

const gchar *conting_attributes_get_string(ContingAttributes *self,
        const gchar *name);
gint conting_attributes_get_int(ContingAttributes *self, const gchar *name);
gdouble conting_attributes_get_double(ContingAttributes *self,
        const gchar *name);

const GValue *conting_attributes_get_value(ContingAttributes *self,
        const gchar *name);
void conting_attributes_get(ContingAttributes *self, const gchar *first,
        ...);
void conting_attributes_get_valist(ContingAttributes *self, const gchar *first,
        va_list ap);

void conting_attributes_set_value(ContingAttributes *self,
        const gchar *name, const GValue *value);
void conting_attributes_set(ContingAttributes *self,
        const gchar *name, GType type, gpointer value);
void conting_attributes_set_string(ContingAttributes *self,
        const gchar *name, const gchar *value);
void conting_attributes_set_int(ContingAttributes *self,
        const gchar *name, gint value);
void conting_attributes_set_double(ContingAttributes *self,
        const gchar *name, gdouble value);
void conting_attributes_set(ContingAttributes *self, const gchar *first,
        ...);
void conting_attributes_set_valist(ContingAttributes *self, const gchar *first,
        va_list ap);


#endif /* CONTING_ATTRIBUTES_H */
