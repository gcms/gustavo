#ifndef CONTING_CONNECTION_H
#define CONTING_CONNECTION_H

#include "contingdrawing.h"

G_BEGIN_DECLS

#define CONTING_TYPE_CONNECTION        (conting_connection_get_type())
#define CONTING_CONNECTION(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_CONNECTION, ContingConnection))
#define CONTING_CONNECTION_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_CONNECTION, ContingConnectionClass))
#define CONTING_IS_CONNECTION(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_CONNECTION))
#define CONTING_IS_CONNECTION_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_CONNECTION))
#define CONTING_CONNECTION_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_CONNECTION, ContingConnectionClass))

typedef struct ContingConnection_ ContingConnection;
typedef struct ContingConnectionClass_ ContingConnectionClass;

#ifndef CONTING_COMPONENT_H
#include "contingcomponent.h"
#endif

struct ContingConnection_ {
    ContingDrawing parent;
};

struct ContingConnectionClass_ {
    ContingDrawingClass parent;

	void (*move)(ContingConnection *self, ContingComponent *comp,
			gint x, gint y);
};

GType conting_connection_get_type(void);
ContingDrawing *conting_connection_new(void);
void conting_connection_move(ContingConnection *self, ContingComponent *comp,
		gint x, gint y);

G_END_DECLS

#endif /* CONTING_CONNECTION_H */
