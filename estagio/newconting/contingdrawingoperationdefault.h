#ifndef CONTING_DRAWING_OPERATION_DEFAULT_H
#define CONTING_DRAWING_OPERATION_DEFAULT_H

#define CONTING_TYPE_DRAWING_OPERATION_DEFAULT	(conting_drawing_operation_default_get_type())
#define CONTING_DRAWING_OPERATION_DEFAULT(o) (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_DRAWING_OPERATION_DEFAULT, ContingDrawingOperationDefault))
#define CONTING_DRAWING_OPERATION_DEFAULT_CLASS(k) (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_DRAWING_OPERATION_DEFAULT, ContingDrawingOperationDefaultClass))
#define CONTING_IS_DRAWING_OPERATION_DEFAULT(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_DRAWING_OPERATION_DEFAULT))
#define CONTING_IS_DRAWING_OPERATION_DEFAULT_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_DRAWING_OPERATION_DEFAULT))
#define CONTING_DRAWING_OPERATION_DEFAULT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_DRAWING_OPERATION_DEFAULT, ContingDrawingOperationDefaultClass))

typedef struct ContingDrawingOperationDefault_ ContingDrawingOperationDefault;
typedef struct ContingDrawingOperationDefaultClass_ ContingDrawingOperationDefaultClass;

#include "contingdrawingoperation.h"

struct ContingDrawingOperationDefault_ {
	GObject parent;

	/* public */
	gint mask;
	gboolean enable;
};

struct ContingDrawingOperationDefaultClass_ {
	GObjectClass parent;
};

enum {
	CONTING_DRAWING_OPERATION_DEFAULT_GEN	= 1 << 0,
	CONTING_DRAWING_OPERATION_DEFAULT_LOAD	= 1 << 1
};

GType conting_drawing_operation_default_get_type(void);

void conting_drawing_operation_default_set_mask(
		ContingDrawingOperationDefault *self, gint mask);

gint conting_drawing_operation_default_get_mask(
		ContingDrawingOperationDefault *self);

void conting_drawing_operation_default_add_mask(
		ContingDrawingOperationDefault *self, gint mask);

void conting_drawing_operation_default_remove_mask(
		ContingDrawingOperationDefault *self, gint mask);

void conting_drawing_operation_default_set_filter(
		ContingDrawingOperationDefault *self, gboolean filter);
#endif /* CONTING_DRAWING_OPERATION_DEFAULT_H */

