#ifndef CONTING_DRAWING_BUS_H
#define CONTING_DRAWING_BUS_H

#include "contingdrawing.h"
#include "contingdrawingline.h"
#include <gtk/gtkenums.h>

#define CONTING_TYPE_DRAWING_BUS        (conting_drawing_bus_get_type())
#define CONTING_DRAWING_BUS(o)          (G_TYPE_CHECK_INSTANCE_CAST((o), \
            CONTING_TYPE_DRAWING_BUS, ContingDrawingBus))
#define CONTING_DRAWING_BUS_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST((k), \
            CONTING_TYPE_DRAWING_BUS, ContingDrawingBusClass))
#define CONTING_IS_DRAWING_BUS(o)       (G_TYPE_CHECK_INSTANCE_TYPE((o), \
            CONTING_TYPE_DRAWING_BUS))
#define CONTING_IS_DRAWING_BUS_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE((o), \
            CONTING_TYPE_DRAWING_BUS))
#define CONTING_DRAWING_BUS_GET_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS((o), \
            CONTING_TYPE_DRAWING_BUS, ContingDrawingBusClass))


typedef struct ContingDrawingBus_ ContingDrawingBus;
struct ContingDrawingBus_ {
    ContingDrawing parent;
};

typedef struct ContingDrawingBusClass_ ContingDrawingBusClass;
struct ContingDrawingBusClass_ {
    ContingDrawingClass parent;
};

GType conting_drawing_bus_get_type(void);

ContingDrawing *conting_drawing_bus_new(void);
gboolean conting_drawing_bus_link_line(ContingDrawingBus *self,
		ContingDrawingLine *line, gint position, GtkSideType side);
gboolean conting_drawing_bus_link_line_coord(ContingDrawingBus *self,
		ContingDrawingLine *line, gint x, gint y);

#endif /* CONTING_DRAWING_BUS_H */
