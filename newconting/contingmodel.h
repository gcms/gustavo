#ifndef CONTING_MODEL_H
#define CONTING_MODEL_H

#include <glib-object.h>

#define CONTING_TYPE_MODEL        (conting_model_get_type())
#define CONTING_MODEL(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_MODEL, ContingModel))
#define CONTING_MODEL_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_MODEL, ContingModelClass))
#define CONTING_IS_MODEL(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_MODEL))
#define CONTING_IS_MODEL_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_MODEL))
#define CONTING_MODEL_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_MODEL, ContingModelClass))

#include "contingfile.h"

typedef struct ContingModel_ ContingModel;
struct ContingModel_ {
    GObject parent;
};

typedef struct ContingModelClass_ ContingModelClass;
struct ContingModelClass_ {
    GObjectClass parent;
};

GType
conting_model_get_type(void);

ContingModel *
conting_model_new_default(void);

ContingModel *
conting_model_new_load_flow(void);

void
conting_model_load_list(ContingModel *self, const GList *list);
void
conting_model_load_file(ContingModel *self, ContingFile *file,
		const gchar *filename);
void
conting_model_clear(ContingModel *self);

const GList *
conting_model_get_items(ContingModel *self);
const GList *
conting_model_get_buses(ContingModel *self);
const GList *
conting_model_get_branches(ContingModel *self);

ContingItemData *
conting_model_get_item(ContingModel *self, ContingItemType type, gpointer key);
ContingItemData *
conting_model_get_bus(ContingModel *self, gint num);
ContingItemData *
conting_model_get_branch(ContingModel *self, gint x, gint y);

void
conting_model_add_item(ContingModel *self, ContingItemData *item);

gboolean
conting_model_loaded(ContingModel *self);

#define conting_item_data_bus_number(b) conting_item_data_get_int(b, "number")
typedef struct ContingItemBranchKey_ ContingItemBranchKey;
ContingItemBranchKey *
conting_item_branch_key_new(gint x, gint y);

#endif /* CONTING_MODEL_H */
