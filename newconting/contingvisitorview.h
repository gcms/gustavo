#ifndef CONTING_VISITOR_VIEW_H
#define CONTING_VISITOR_VIEW_H

#define CONTING_TYPE_VISITOR_VIEW	(conting_visitor_view_get_type())
#define CONTING_VISITOR_VIEW(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_VISITOR_VIEW, ContingVisitorView))
#define CONTING_VISITOR_VIEW_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_VISITOR_VIEW, ContingVisitorViewClass))
#define CONTING_IS_VISITOR_VIEW(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_VISITOR_VIEW))
#define CONTING_IS_VISITOR_VIEW_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_VISITOR_VIEW))
#define CONTING_VISITOR_VIEW_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_VISITOR_VIEW, ContingVisitorViewClass))

typedef struct ContingVisitorView_ ContingVisitorView;
typedef struct ContingVisitorViewClass_ ContingVisitorViewClass;

struct ContingVisitorView_ {
	GObject parent;
};

struct ContingVisitorViewClass_ {
	GObjectClass parent;
};

GType conting_visitor_view_get_type(void);

#endif /* CONTING_VISITOR_VIEW_H */
