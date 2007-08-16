#ifndef CONTING_GROUND_H
#define CONTING_GROUND_H

#include "contingsymbol.h"



#define CONTING_TYPE_GROUND        (conting_ground_get_type())
#define CONTING_GROUND(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_GROUND, ContingGround))
#define CONTING_GROUND_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_GROUND, ContingGroundClass))
#define CONTING_IS_GROUND(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_GROUND))
#define CONTING_IS_GROUND_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_GROUND))
#define CONTING_GROUND_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_GROUND, ContingGroundClass))

typedef struct ContingGround_ ContingGround;
struct ContingGround_ {
    ContingSymbol parent;
};

typedef struct ContingGroundClass_ ContingGroundClass;
struct ContingGroundClass_ {
    ContingSymbolClass parent;
};

GType conting_ground_get_type(void);




#endif /* CONTING_GROUND_H */
