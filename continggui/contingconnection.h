#ifndef CONTING_CONNECTION_H
#define CONTING_CONNECTION_H

#include "contingdrawing.h"

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
struct ContingConnection_ {
    ContingDrawing parent;
};

typedef struct ContingConnectionClass_ ContingConnectionClass;
struct ContingConnectionClass_ {
    ContingDrawingClass parent;
};

GType conting_connection_get_type(void);
ContingDrawing *conting_connection_new(void);

#endif /* CONTING_CONNECTION_H */
