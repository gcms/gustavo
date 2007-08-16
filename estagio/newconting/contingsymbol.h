#ifndef CONTING_SYMBOL_H
#define CONTING_SYMBOL_H

#include "contingcomponent.h"



#define CONTING_TYPE_SYMBOL        (conting_symbol_get_type())
#define CONTING_SYMBOL(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_SYMBOL, ContingSymbol))
#define CONTING_SYMBOL_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_SYMBOL, ContingSymbolClass))
#define CONTING_IS_SYMBOL(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_SYMBOL))
#define CONTING_IS_SYMBOL_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_SYMBOL))
#define CONTING_SYMBOL_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_SYMBOL, ContingSymbolClass))

typedef struct ContingSymbol_ ContingSymbol;
struct ContingSymbol_ {
    ContingComponent parent;

	/* protected */
	ContingDrawing *link0;
};

typedef struct ContingSymbolClass_ ContingSymbolClass;
struct ContingSymbolClass_ {
    ContingComponentClass parent;

	void (*get_link_point)(ContingSymbol *self, ArtPoint *p);
};

GType conting_symbol_get_type(void);

void
conting_symbol_get_link_point(ContingSymbol *self, ArtPoint *p);


#endif /* CONTING_SYMBOL_H */
