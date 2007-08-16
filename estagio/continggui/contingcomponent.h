#ifndef CONTING_COMPONENT_H
#define CONTING_COMPONENT_H

#include "contingdrawing.h"
#include <gtk/gtkenums.h>

G_BEGIN_DECLS


#define CONTING_TYPE_COMPONENT        (conting_component_get_type())
#define CONTING_COMPONENT(o)          (G_TYPE_CHECK_INSTANCE_CAST((o), \
            CONTING_TYPE_COMPONENT, ContingComponent))
#define CONTING_COMPONENT_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST((k), \
            CONTING_TYPE_COMPONENT, ContingComponentClass))
#define CONTING_IS_COMPONENT(o)       (G_TYPE_CHECK_INSTANCE_TYPE((o), \
            CONTING_TYPE_COMPONENT))
#define CONTING_IS_COMPONENT_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE((k), \
            CONTING_TYPE_COMPONENT))
#define CONTING_COMPONENT_GET_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS((o), \
            CONTING_TYPE_COMPONENT, ContingComponentClass))

typedef enum {
	CONTING_RESIZE_HORIZONTAL = 1 << 1,
	CONTING_RESIZE_VERTICAL = 1 << 2
} ContingResizeOrientation;


typedef struct ContingComponent_ ContingComponent;
typedef struct ContingComponentClass_ ContingComponentClass;

#ifndef CONTING_CONNECTION_H
#include "contingconnection.h"
#endif

struct ContingComponent_ {
    ContingDrawing parent;
};


struct ContingComponentClass_ {
    ContingDrawingClass parent;

	void (*get_rectangle)(ContingComponent *self, GdkRectangle *rect);
	void (*move)(ContingComponent *self, gint x, gint y);
	void (*rotate)(ContingComponent *self, gdouble theta);
	gboolean (*connect)(ContingComponent *self, ContingConnection *conn,
			gint x, gint y, GdkPoint *shift);
	gboolean (*answer_resize)(ContingComponent *self, gint x, gint y,
			ContingResizeOrientation *orientation);
	void (*resizeable)(ContingComponent *self,
			ContingResizeOrientation *orientation);
	void (*resize)(ContingComponent *self,
		gint x, gint y, ContingResizeOrientation orient);
};

GType conting_component_get_type(void);

ContingDrawing *conting_component_new(void);
void conting_component_get_rectangle(ContingComponent *self, GdkRectangle *r);
void conting_component_move(ContingComponent *self, gint x, gint y);
void conting_component_rotate(ContingComponent *self, gdouble theta);

gboolean conting_component_connect(ContingComponent *self,
		ContingConnection *line, gint x, gint y, GdkPoint *shift);

void conting_component_resizeable(ContingComponent *self,
		ContingResizeOrientation *orientation);
gboolean conting_component_answer_resize(ContingComponent *self,
		gint x, gint y, ContingResizeOrientation *orientation);
void conting_component_resize(ContingComponent *self,
		gint x, gint y, ContingResizeOrientation orient);

G_END_DECLS

#endif /* CONTING_COMPONENT_H */
