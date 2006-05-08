#ifndef CONTING_COMPONENT_H
#define CONTING_COMPONENT_H

#include "contingdrawing.h"

G_BEGIN_DECLS

#define CONTING_TYPE_COMPONENT        (conting_component_get_type())
#define CONTING_COMPONENT(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_COMPONENT, ContingComponent))
#define CONTING_COMPONENT_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_COMPONENT, ContingComponentClass))
#define CONTING_IS_COMPONENT(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_COMPONENT))
#define CONTING_IS_COMPONENT_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_COMPONENT))
#define CONTING_COMPONENT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_COMPONENT, ContingComponentClass))

typedef struct ContingComponent_ ContingComponent;
struct ContingComponent_ {
    ContingDrawing parent;
};

typedef struct ContingComponentClass_ ContingComponentClass;
struct ContingComponentClass_ {
    ContingDrawingClass parent;
};

GType conting_component_get_type(void);

G_END_DECLS

#endif /* CONTING_COMPONENT_H */
