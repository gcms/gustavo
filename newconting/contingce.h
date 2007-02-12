#ifndef CONTING_CE_H
#define CONTING_CE_H

#include "contingsymbol.h"



#define CONTING_TYPE_CE        (conting_ce_get_type())
#define CONTING_CE(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_CE, ContingCE))
#define CONTING_CE_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_CE, ContingCEClass))
#define CONTING_IS_CE(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_CE))
#define CONTING_IS_CE_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_CE))
#define CONTING_CE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_CE, ContingCEClass))

typedef struct ContingCE_ ContingCE;
struct ContingCE_ {
    ContingSymbol parent;
};

typedef struct ContingCEClass_ ContingCEClass;
struct ContingCEClass_ {
    ContingSymbolClass parent;
};

GType conting_ce_get_type(void);




#endif /* CONTING_CE_H */
