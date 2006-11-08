#ifndef CONTING_DRAWING_OPERATION_H
#define CONTING_DRAWING_OPERATION_H

#include <glib.h>
#include <glib-object.h>


#define CONTING_TYPE_DRAWING_OPERATION  (conting_drawing_operation_get_type())
#define CONTING_DRAWING_OPERATION(o)    (G_TYPE_CHECK_INSTANCE_CAST ((o), \
			CONTING_TYPE_DRAWING_OPERATION, ContingDrawingOperation))
#define CONTING_DRAWING_OPERATION_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST ((k), \
			CONTING_TYPE_DRAWING_OPERATION, ContingDrawingOperationClass))
#define CONTING_IS_DRAWING_OPERATION(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), \
			CONTING_TYPE_DRAWING_OPERATION))
#define CONTING_IS_DRAWING_OPERATION_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
			CONTING_TYPE_DRAWING_OPERATION))
#define CONTING_DRAWING_OPERATION_GET_CLASS(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), \
			CONTING_TYPE_DRAWING_OPERATION, ContingDrawingOperationClass))


typedef struct ContingDrawingOperation_ ContingDrawingOperation;
typedef struct ContingDrawingOperationClass_ ContingDrawingOperationClass;

#include "contingdrawing.h"

struct ContingDrawingOperationClass_ {
	GTypeInterface parent;
	
	void (*get_bounds)(ContingDrawingOperation *self, ContingDrawing *drawing,
			ArtDRect *bounds);
	void (*draw)(ContingDrawingOperation *self, ContingDrawing *drawing);
};

GType
conting_drawing_operation_get_type(void);


/* Drawing operation label */
#define CONTING_TYPE_DRAWING_OPERATION_LABEL (conting_drawing_operation_label_get_type())
GType conting_drawing_operation_label_get_type(void);

typedef const gchar *(*ContingLabelFunc)(ContingDrawingOperation *self,
		ContingDrawing *drawing, gpointer user_data);

ContingDrawingOperation *
conting_drawing_operation_label_new_with_func(ContingLabelFunc func,
        gpointer user_data);

typedef enum {
	CONTING_DRAWING_OPERATION_LABEL_TOP,
	CONTING_DRAWING_OPERATION_LABEL_BOTTOM,
} ContingDrawingOperationLabelPlace;

#include "contingdrawingoperationdefault.h"


void conting_drawing_operation_get_bounds(ContingDrawingOperation *self,
		ContingDrawing *drawing, ArtDRect *bounds);
void conting_drawing_operation_draw(ContingDrawingOperation *self,
		ContingDrawing *drawing);

#endif /* CONTING_DRAWING_OPERATION_H */
