#ifndef CONTING_TRANS3_H
#define CONTING_TRANS3_H

#include "contingbusbase.h"



#define CONTING_TYPE_TRANS3        (conting_trans3_get_type())
#define CONTING_TRANS3(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_TRANS3, ContingTrans3))
#define CONTING_TRANS3_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_TRANS3, ContingTrans3Class))
#define CONTING_IS_TRANS3(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_TRANS3))
#define CONTING_IS_TRANS3_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_TRANS3))
#define CONTING_TRANS3_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_TRANS3, ContingTrans3Class))

typedef struct ContingTrans3_ ContingTrans3;
struct ContingTrans3_ {
    ContingBusBase parent;
};

typedef struct ContingTrans3Class_ ContingTrans3Class;
struct ContingTrans3Class_ {
    ContingBusBaseClass parent;
};

GType conting_trans3_get_type(void);

void
conting_trans3_get_buses(ContingTrans3 *self,
		ContingBus **bus0, ContingBus **bus1, ContingBus **bus2);




#endif /* CONTING_TRANS3_H */
