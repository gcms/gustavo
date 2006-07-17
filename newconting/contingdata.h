#ifndef CONTING_DATA_H
#define CONTING_DATA_H

#include <glib-object.h>

G_BEGIN_DECLS

#define CONTING_TYPE_DATA        (conting_data_get_type())
#define CONTING_DATA(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_DATA, ContingData))
#define CONTING_DATA_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_DATA, ContingDataClass))
#define CONTING_IS_DATA(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_DATA))
#define CONTING_IS_DATA_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_DATA))
#define CONTING_DATA_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_DATA, ContingDataClass))

#include "contingfile.h"

#define BUS 0
#define BRANCH 1


typedef struct data_ data_t;

#include "contingdrawing.h"
struct data_ {
	gint type;
	ContingDrawing *used;
	union {
		bus_data_t bus;
		branch_data_t branch;
	} data;
	GHashTable *attrs;
};

typedef struct ContingData_ ContingData;
struct ContingData_ {
    GObject parent;
};

typedef struct ContingDataClass_ ContingDataClass;
struct ContingDataClass_ {
    GObjectClass parent;
};

GType
conting_data_get_type(void);

void
conting_data_load_file(ContingData *self, const gchar *filename);

const GList *
conting_data_get_bus(ContingData *self);
const GList *
conting_data_get_branch(ContingData *self);


GList *
conting_data_get_unassoc(ContingData *self);

data_t *
conting_data_get(ContingData *self, ContingDrawing *drawing);

void
conting_data_assoc(ContingData *self, ContingDrawing *drawing, data_t *data);

void
conting_data_unassoc(ContingData *self, ContingDrawing *drawing);

G_END_DECLS

#endif /* CONTING_DATA_H */
