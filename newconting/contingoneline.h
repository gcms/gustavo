#ifndef CONTING_ONE_LINE_H
#define CONTING_ONE_LINE_H

#include <gtk/gtk.h>
#include <gtk/gtkenums.h>



#define CONTING_TYPE_ONE_LINE			(conting_one_line_get_type())
#define CONTING_ONE_LINE(o)				(G_TYPE_CHECK_INSTANCE_CAST((o), \
			CONTING_TYPE_ONE_LINE, ContingOneLine))
#define CONTING_ONE_LINE_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), \
			CONTING_TYPE_ONE_LINE, ContingOneLineClass))
#define CONTING_IS_ONE_LINE(o)			(G_TYPE_CHECK_INSTANCE_TYPE((o), \
			CONTING_TYPE_ONE_LINE))
#define CONTING_IS_ONE_LINE_CLASS		(G_TYPE_CHECK_CLASS_TYPE((k), \
			CONTING_TYPE_ONE_LINE))
#define CONTING_ONE_LINE_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS((o), \
			CONTING_TYPE_ONE_LINE, ContingOneLineClass))

typedef struct ContingOneLine_ ContingOneLine;
typedef struct ContingOneLineClass_ ContingOneLineClass;

#include "contingdrawing.h"

struct ContingOneLine_ {
	GObject parent;
};

struct ContingOneLineClass_ {
	GObjectClass parent;
};



GType conting_one_line_get_type(void);
void conting_one_line_window_to_world(ContingOneLine *self,
		                         gdouble win_x, gdouble win_y,
								 gdouble *world_x, gdouble *world_y);
void conting_one_line_world_to_window(ContingOneLine *self,
		                         gdouble world_x, gdouble world_y,
								 gdouble *win_x, gdouble *win_y);
void conting_one_line_world_to_window_affine(ContingOneLine *self,
		gdouble affine[6]);
void conting_one_line_grab(ContingOneLine *self,
		              ContingDrawing *grabbed_drawing);
void conting_one_line_ungrab(ContingOneLine *self,
		              ContingDrawing *grabbed_drawing);

void conting_one_line_update(ContingOneLine *self,
                        ArtDRect *bounds);
void conting_one_line_update_drawing(ContingOneLine *self,
		ContingDrawing *drawing);

GSList *conting_one_line_answer(ContingOneLine *self,
		                        gdouble world_x, gdouble world_y);

void conting_one_line_delete_drawing(ContingOneLine *self, ContingDrawing *);
void conting_one_line_set_widget(ContingOneLine *self, GtkWidget *widget);
void conting_one_line_create(ContingOneLine *self, ContingDrawing *drawing);

void conting_one_line_save(ContingOneLine *self, const char *filename);

void conting_one_line_load_data(ContingOneLine *self, const char *filename);
void conting_one_line_open(ContingOneLine *self, const char *filename);

void conting_one_line_edit(ContingOneLine *self, ContingDrawing *drawing);

cairo_t * conting_drawing_get_cairo(ContingDrawing *drawing);
cairo_t * conting_drawing_get_cairo_absolute(ContingDrawing *drawing);

typedef enum {
	CONTING_ONE_LINE_EDIT,
	CONTING_ONE_LINE_VIEW,
	CONTING_ONE_LINE_MODE_NUMBER
} ContingOneLineMode;

gboolean conting_one_line_set_mode(ContingOneLine *self,
        ContingOneLineMode mode);

#include "contingdrawingoperation.h"
void conting_one_line_add_operation(ContingOneLine *self,
        ContingDrawingOperation *opr);
void
conting_one_line_remove_operation(ContingOneLine *self,
        ContingDrawingOperation *opr);

ContingDrawingOperationDefault *
conting_one_line_get_default_operation(ContingOneLine *self);

#include "contingitemdata.h"
ContingItemData * conting_drawing_get_item_data(ContingDrawing *drawing);
#endif /* CONTING_ONE_LINE_H */
