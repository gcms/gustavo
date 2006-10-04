#ifndef CONTING_COMPONENT_H
#define CONTING_COMPONENT_H




#define CONTING_TYPE_COMPONENT        (conting_component_get_type())
#define CONTING_COMPONENT(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_COMPONENT, ContingComponent))
#define CONTING_COMPONENT_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_COMPONENT, ContingComponentClass))
#define CONTING_IS_COMPONENT(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_COMPONENT))
#define CONTING_IS_COMPONENT_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_COMPONENT))
#define CONTING_COMPONENT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_COMPONENT, ContingComponentClass))

typedef struct ContingComponent_ ContingComponent;
typedef struct ContingComponentClass_ ContingComponentClass;

#ifndef CONTING_DRAWING_H
#include "contingdrawing.h"
#endif

typedef enum {
	CONTING_RESIZE_LEFT =	1 << 0,
	CONTING_RESIZE_RIGHT =	1 << 1,
	CONTING_RESIZE_DOWN =	1 << 2,
	CONTING_RESIZE_UP =		1 << 3
} ContingResizeOrientation;

struct ContingComponent_ {
    ContingDrawing parent;
    /* protected */
    ArtPoint p0, p1;
    gboolean placed;
	gboolean resize_horizontal, resize_vertical;

	/* private */
    gdouble rotate[6];
	GHashTable *points;
	GList *links;

	ArtPoint dragging_point;
	gboolean dragging;

	gboolean show;

	ContingResizeOrientation resizing;
	ArtDRect min_bounds;
};

struct ContingComponentClass_ {
    ContingDrawingClass parent;

    gboolean (*link)(ContingComponent *self, ContingDrawing *line,
            gdouble world_x, gdouble world_y, ArtPoint *pw);

    gboolean (*get_link_point)(ContingComponent *self, ContingDrawing *line,
            ArtPoint *p);

	/* protected */
	void (*link_deleted)(ContingComponent *self, ContingDrawing *drawing);
};

GType conting_component_get_type(void);

gboolean conting_component_link(ContingComponent *self,
                                ContingDrawing *line,
                                gdouble world_x, gdouble world_y,
                                ArtPoint *pw);

gboolean conting_component_get_link_point(ContingComponent *self,
                                          ContingDrawing *line,
                                          ArtPoint *p);
/* protected */

GType conting_component_get_type(void);

gboolean conting_component_link(ContingComponent *self,
                                ContingDrawing *line,
                                gdouble world_x, gdouble world_y,
                                ArtPoint *pw);

gboolean conting_component_get_link_point(ContingComponent *self,
                                          ContingDrawing *line,
                                          ArtPoint *p);

void conting_component_disconnect_link(ContingComponent *self,
		                               ContingDrawing *drawing);
void conting_component_connect_link(ContingComponent *self,
		                        ContingDrawing *link, ArtPoint *p);




#endif /* CONTING_COMPONENT_H */
