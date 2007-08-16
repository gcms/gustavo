#ifndef CONTING_LINE_H
#define CONTING_LINE_H

#include "contingconnection.h"

G_BEGIN_DECLS

#define CONTING_TYPE_LINE        (conting_line_get_type())
#define CONTING_LINE(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_LINE, ContingLine))
#define CONTING_LINE_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_LINE, ContingLineClass))
#define CONTING_IS_LINE(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_LINE))
#define CONTING_IS_LINE_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_LINE))
#define CONTING_LINE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_LINE, ContingLineClass))

typedef struct ContingLine_ ContingLine;
struct ContingLine_ {
    ContingConnection parent;
};

typedef struct ContingLineClass_ ContingLineClass;
struct ContingLineClass_ {
    ContingConnectionClass parent;
};

GType conting_line_get_type(void);
ContingDrawing *conting_line_new(void);

G_END_DECLS

#endif /* CONTING_LINE_H */
