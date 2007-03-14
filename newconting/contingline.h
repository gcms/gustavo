#ifndef CONTING_LINE_H
#define CONTING_LINE_H

#include "contingdrawing.h"



#define CONTING_TYPE_LINE        (conting_line_get_type())
#define CONTING_LINE(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_LINE, ContingLine))
#define CONTING_LINE_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_LINE, ContingLineClass))
#define CONTING_IS_LINE(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_LINE))
#define CONTING_IS_LINE_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_LINE))
#define CONTING_LINE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_LINE, ContingLineClass))

typedef struct ContingLine_ ContingLine;
struct ContingLine_ {
    ContingDrawing parent;
};

typedef struct ContingLineClass_ ContingLineClass;
struct ContingLineClass_ {
    ContingDrawingClass parent;
};

GType conting_line_get_type(void);
#include "contingcomponent.h"
void conting_line_get_links(ContingLine *self,
		ContingComponent **comp0, ContingComponent **comp1);

#include "contingbusbase.h"
void conting_line_get_buses(ContingLine *self,
		ContingBusBase **comp0, ContingBusBase **comp1);

void
conting_line_set_shift_mask(ContingLine *self, gboolean shift_mask);

void
conting_line_create_point(ContingLine *self, const ArtPoint *pi);

void conting_line_get_limits(ContingLine *self, ArtDRect *p1, ArtDRect *p2);
const GList *conting_line_get_points(ContingLine *self);


#endif /* CONTING_LINE_H */
