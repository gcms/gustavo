#ifndef CONTING_BUS_H
#define CONTING_BUS_H




#define CONTING_TYPE_BUS        (conting_bus_get_type())
#define CONTING_BUS(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_BUS, ContingBus))
#define CONTING_BUS_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_BUS, ContingBusClass))
#define CONTING_IS_BUS(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_BUS))
#define CONTING_IS_BUS_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_BUS))
#define CONTING_BUS_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_BUS, ContingBusClass))

typedef struct ContingBus_ ContingBus;
typedef struct ContingBusClass_ ContingBusClass;


#include "contingcomponent.h"

struct ContingBus_ {
    ContingComponent parent;
};

struct ContingBusClass_ {
    ContingComponentClass parent;
};

GType conting_bus_get_type(void);



#endif /* CONTING_BUS_H */
