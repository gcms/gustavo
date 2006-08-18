#ifndef CONTING_ITEM_DATA_H
#define CONTING_ITEM_DATA_H

#include <glib-object.h>



#define CONTING_TYPE_ITEM_DATA        (conting_item_data_get_type())
#define CONTING_ITEM_DATA(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_ITEM_DATA, ContingItemData))
#define CONTING_ITEM_DATA_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_ITEM_DATA, ContingItemDataClass))
#define CONTING_IS_ITEM_DATA(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_ITEM_DATA))
#define CONTING_IS_ITEM_DATA_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_ITEM_DATA))
#define CONTING_ITEM_DATA_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_ITEM_DATA, ContingItemDataClass))

typedef struct ContingItemData_ ContingItemData;
struct ContingItemData_ {
    GObject parent;
};

typedef struct ContingItemDataClass_ ContingItemDataClass;
struct ContingItemDataClass_ {
    GObjectClass parent;
};

#define CONTING_TYPE_ITEM_TYPE	(conting_item_type_get_type())

typedef enum {
	CONTING_ITEM_TYPE_BUS,
	CONTING_ITEM_TYPE_BRANCH
} ContingItemType;

GType conting_item_type_get_type(void);

GType conting_item_data_get_type(void);

void conting_item_data_set_attr(ContingItemData *self,
		const gchar *first_attr, ...);

void conting_item_data_get_attr(ContingItemData *self,
		const gchar *first_attr, ...);

typedef void (*ContingItemDataAttrFunc)(const gchar *name, const GValue *value,
		gpointer user_data);

void conting_item_data_attr_foreach(ContingItemData *self,
		ContingItemDataAttrFunc func, gpointer user_data);

ContingItemType conting_item_data_get_item_type(ContingItemData *self);



#endif /* CONTING_ITEM_DATA_H */
