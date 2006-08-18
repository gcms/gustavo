#ifndef CONTING_TRANS2_H
#define CONTING_TRANS2_H

#include "contingcomponent.h"



#define CONTING_TYPE_TRANS2        (conting_trans2_get_type())
#define CONTING_TRANS2(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_TRANS2, ContingTrans2))
#define CONTING_TRANS2_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_TRANS2, ContingTrans2Class))
#define CONTING_IS_TRANS2(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_TRANS2))
#define CONTING_IS_TRANS2_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_TRANS2))
#define CONTING_TRANS2_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_TRANS2, ContingTrans2Class))

typedef struct ContingTrans2_ ContingTrans2;
struct ContingTrans2_ {
    ContingComponent parent;
};

typedef struct ContingTrans2Class_ ContingTrans2Class;
struct ContingTrans2Class_ {
    ContingComponentClass parent;
};

GType conting_trans2_get_type(void);




#endif /* CONTING_TRANS2_H */
