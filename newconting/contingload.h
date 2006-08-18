#ifndef CONTING_LOAD_H
#define CONTING_LOAD_H

#include "contingcomponent.h"



#define CONTING_TYPE_LOAD        (conting_load_get_type())
#define CONTING_LOAD(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_LOAD, ContingLoad))
#define CONTING_LOAD_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_LOAD, ContingLoadClass))
#define CONTING_IS_LOAD(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_LOAD))
#define CONTING_IS_LOAD_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_LOAD))
#define CONTING_LOAD_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_LOAD, ContingLoadClass))

typedef struct ContingLoad_ ContingLoad;
struct ContingLoad_ {
    ContingComponent parent;
};

typedef struct ContingLoadClass_ ContingLoadClass;
struct ContingLoadClass_ {
    ContingComponentClass parent;
};

GType conting_load_get_type(void);




#endif /* CONTING_LOAD_H */
