#ifndef CONTING_ONE_LINE_H
#define CONTING_ONE_LINE_H

#include <gtk/gtkenums.h>
#include "contingdrawing.h"

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
struct ContingOneLine_ {
	GObject parent;
};

typedef struct ContingOneLineClass_ ContingOneLineClass;
struct ContingOneLineClass_ {
	GObjectClass parent;

	void (*get_drawing_position)(ContingOneLine *self,
			ContingDrawing *drawing, GdkPoint *position);

	void (*get_relative_position)(ContingOneLine *self,
			ContingDrawing *draw1, ContingDrawing *draw2,
			GdkPoint *result);
};

G_END_DECLS

#endif /* CONTING_ONE_LINE_H */
