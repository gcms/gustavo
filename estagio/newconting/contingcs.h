#ifndef CONTING_CS_H
#define CONTING_CS_H

#include "contingsymbol.h"



#define CONTING_TYPE_CS        (conting_cs_get_type())
#define CONTING_CS(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_CS, ContingCS))
#define CONTING_CS_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_CS, ContingCSClass))
#define CONTING_IS_CS(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_CS))
#define CONTING_IS_CS_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_CS))
#define CONTING_CS_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_CS, ContingCSClass))

typedef struct ContingCS_ ContingCS;
struct ContingCS_ {
    ContingSymbol parent;
};

typedef struct ContingCSClass_ ContingCSClass;
struct ContingCSClass_ {
    ContingSymbolClass parent;
};

GType conting_cs_get_type(void);




#endif /* CONTING_CS_H */
