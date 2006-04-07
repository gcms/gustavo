#ifndef CONTING_DRAWING_LINE_H
#define CONTING_DRAWING_LINE_H

#include "contingdrawing.h"

#define CONTING_TYPE_DRAWING_LINE        (conting_drawing_line_get_type())
#define CONTING_DRAWING_LINE(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_DRAWING_LINE, ContingDrawingLine))
#define CONTING_DRAWING_LINE_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_DRAWING_LINE, ContingDrawingLineClass))
#define CONTING_IS_DRAWING_LINE(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_DRAWING_LINE))
#define CONTING_IS_DRAWING_LINE_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_DRAWING_LINE))
#define CONTING_DRAWING_LINE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_DRAWING_LINE, ContingDrawingLineClass))

typedef struct ContingDrawingLine_ ContingDrawingLine;
struct ContingDrawingLine_ {
    ContingDrawing parent;
};

typedef struct ContingDrawingLineClass_ ContingDrawingLineClass;
struct ContingDrawingLineClass_ {
    ContingDrawingClass parent;
};

GType conting_drawing_line_get_type(void);
ContingDrawing *conting_drawing_line_new(void);

#endif /* CONTING_DRAWING_LINE_H */
