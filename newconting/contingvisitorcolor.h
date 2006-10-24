#ifndef CONTING_VISITOR_COLOR_H
#define CONTING_VISITOR_COLOR_H

#define CONTING_TYPE_VISITOR_COLOR	(conting_visitor_color_get_type())
#define CONTING_VISITOR_COLOR(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_VISITOR_COLOR, ContingVisitorColor))
#define CONTING_VISITOR_COLOR_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_VISITOR_COLOR, ContingVisitorColorClass))
#define CONTING_IS_VISITOR_COLOR(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_VISITOR_COLOR))
#define CONTING_IS_VISITOR_COLOR_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_VISITOR_COLOR))
#define CONTING_VISITOR_COLOR_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_VISITOR_COLOR, ContingVisitorColorClass))

typedef struct ContingVisitorColor_ ContingVisitorColor;
typedef struct ContingVisitorColorClass_ ContingVisitorColorClass;

struct ContingVisitorColor_ {
	GObject parent;
};

struct ContingVisitorColorClass_ {
	GObjectClass parent;
};

void
conting_visitor_color_set_default_color(ContingVisitorColor *self,
        GdkColor *color);

void
conting_visitor_color_use_default(ContingVisitorColor *self, gboolean use);

void
conting_visitor_color_add_interval(ContingVisitorColor *self,
        gdouble start, gdouble end, GdkColor *color);

GType conting_visitor_color_get_type(void);

#endif /* CONTING_VISITOR_COLOR_H */
