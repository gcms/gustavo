#ifndef CONTING_BUS_BASE_H
#define CONTING_BUS_BASE_H




#define CONTING_TYPE_BUS_BASE        (conting_bus_base_get_type())
#define CONTING_BUS_BASE(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_BUS_BASE, ContingBusBase))
#define CONTING_BUS_BASE_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_BUS_BASE, ContingBusBaseClass))
#define CONTING_IS_BUS_BASE(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_BUS_BASE))
#define CONTING_IS_BUS_BASE_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_BUS_BASE))
#define CONTING_BUS_BASE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_BUS_BASE, ContingBusBaseClass))

typedef struct ContingBusBase_ ContingBusBase;
typedef struct ContingBusBaseClass_ ContingBusBaseClass;


#include "contingcomponent.h"

struct ContingBusBase_ {
    ContingComponent parent;
};

struct ContingBusBaseClass_ {
    ContingComponentClass parent;
};

GType conting_bus_base_get_type(void);



#endif /* CONTING_BUS_BASE_H */
