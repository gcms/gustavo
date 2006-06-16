#ifndef CONTING_ONE_LINE_H
#define CONTING_ONE_LINE_H

#include <gtk/gtk.h>
#include <gtk/gtkenums.h>

G_BEGIN_DECLS

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

G_END_DECLS

GType conting_one_line_get_type(void);
void conting_one_line_window_to_world(ContingOneLine *self,
		                         gdouble win_x, gdouble win_y,
								 gdouble *world_x, gdouble *world_y);
void conting_one_line_world_to_window(ContingOneLine *self,
		                         gdouble world_x, gdouble world_y,
								 gdouble *win_x, gdouble *win_y);
void conting_one_line_grab(ContingOneLine *self,
		              ContingDrawing *grabbed_drawing);
void conting_one_line_ungrab(ContingOneLine *self,
		              ContingDrawing *grabbed_drawing);

void conting_one_line_update(ContingOneLine *self, ArtDRect *bounds);

GSList *conting_one_line_answer(ContingOneLine *self,
		                        gdouble world_x, gdouble world_y);

void conting_one_line_delete_drawing(ContingOneLine *self, ContingDrawing *);
void conting_one_line_set_widget(ContingOneLine *self, GtkWidget *widget);
void conting_one_line_create(ContingOneLine *self, ContingDrawing *drawing);

void conting_one_line_save(ContingOneLine *self, const char *filename);

void conting_one_line_open(ContingOneLine *self, const char *filename);
#endif /* CONTING_ONE_LINE_H */
