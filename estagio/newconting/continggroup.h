#ifndef CONTING_GROUP_H
#define CONTING_GROUP_H

#include "contingdrawing.h"


#define CONTING_TYPE_GROUP        (conting_group_get_type())
#define CONTING_GROUP(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_GROUP, ContingGroup))
#define CONTING_GROUP_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_GROUP, ContingGroupClass))
#define CONTING_IS_GROUP(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_GROUP))
#define CONTING_IS_GROUP_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_GROUP))
#define CONTING_GROUP_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_GROUP, ContingGroupClass))

typedef struct ContingGroup_ ContingGroup;
struct ContingGroup_ {
    ContingDrawing parent;
};

typedef struct ContingGroupClass_ ContingGroupClass;
struct ContingGroupClass_ {
    ContingDrawingClass parent;
};

GType conting_group_get_type(void);
void conting_group_add_drawing(ContingGroup *group, ContingDrawing *drawing);
gboolean conting_group_contains(ContingGroup *group, ContingDrawing *drawing);
GSList *conting_group_get_children(ContingGroup *group);



#endif /* CONTING_GROUP_H */
