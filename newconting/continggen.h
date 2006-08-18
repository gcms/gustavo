#ifndef CONTING_GEN_H
#define CONTING_GEN_H

#include "contingcomponent.h"



#define CONTING_TYPE_GEN        (conting_gen_get_type())
#define CONTING_GEN(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_GEN, ContingGen))
#define CONTING_GEN_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_GEN, ContingGenClass))
#define CONTING_IS_GEN(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_GEN))
#define CONTING_IS_GEN_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_GEN))
#define CONTING_GEN_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_GEN, ContingGenClass))

typedef struct ContingGen_ ContingGen;
struct ContingGen_ {
    ContingComponent parent;
};

typedef struct ContingGenClass_ ContingGenClass;
struct ContingGenClass_ {
    ContingComponentClass parent;
};

GType conting_gen_get_type(void);




#endif /* CONTING_GEN_H */
