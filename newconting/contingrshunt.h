#ifndef CONTING_R_SHUNT_H
#define CONTING_R_SHUNT_H

#include "contingcomponent.h"



#define CONTING_TYPE_R_SHUNT        (conting_r_shunt_get_type())
#define CONTING_R_SHUNT(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_R_SHUNT, ContingRShunt))
#define CONTING_R_SHUNT_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_R_SHUNT, ContingRShuntClass))
#define CONTING_IS_R_SHUNT(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_R_SHUNT))
#define CONTING_IS_R_SHUNT_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_R_SHUNT))
#define CONTING_R_SHUNT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_R_SHUNT, ContingRShuntClass))

typedef struct ContingRShunt_ ContingRShunt;
struct ContingRShunt_ {
    ContingComponent parent;
};

typedef struct ContingRShuntClass_ ContingRShuntClass;
struct ContingRShuntClass_ {
    ContingComponentClass parent;
};

GType conting_r_shunt_get_type(void);



#endif /* CONTING_R_SHUNT_H */
