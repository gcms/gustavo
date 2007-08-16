#ifndef CONTING_C_SHUNT_H
#define CONTING_C_SHUNT_H

#include "contingcomponent.h"



#define CONTING_TYPE_C_SHUNT        (conting_c_shunt_get_type())
#define CONTING_C_SHUNT(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_C_SHUNT, ContingCShunt))
#define CONTING_C_SHUNT_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_C_SHUNT, ContingCShuntClass))
#define CONTING_IS_C_SHUNT(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_C_SHUNT))
#define CONTING_IS_C_SHUNT_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_C_SHUNT))
#define CONTING_C_SHUNT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_C_SHUNT, ContingCShuntClass))

typedef struct ContingCShunt_ ContingCShunt;
struct ContingCShunt_ {
    ContingComponent parent;
};

typedef struct ContingCShuntClass_ ContingCShuntClass;
struct ContingCShuntClass_ {
    ContingComponentClass parent;
};

GType conting_c_shunt_get_type(void);



#endif /* CONTING_C_SHUNT_H */
