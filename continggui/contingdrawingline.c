#include "contingdrawingline.h"


#define CONTING_DRAWING_LINE_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
		CONTING_TYPE_DRAWING_LINE, ContingDrawingLinePrivate))

typedef struct ContingDrawingLinePrivate_ ContingDrawingLinePrivate;
struct ContingDrawingLinePrivate_ {
	GSList *points;

	gboolean placed;
};

static GdkPoint line_points[2] = {
    { -10, 0 }, { 10, 0 }
};

static void conting_drawing_line_draw(ContingDrawing *self,
        GdkDrawable *drawable, GdkGC *gc, const GdkPoint *pos) {
	ContingDrawingLinePrivate *priv;
	GSList *n;
	GdkPoint *last_point = NULL;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_LINE(self));

	priv = CONTING_DRAWING_LINE_GET_PRIVATE(self);

	for (n = priv->points; n != NULL; n = g_slist_next(n)) {
		GdkPoint *cur_point = n->data;
		if (last_point) {
			gdk_draw_line(drawable, gc,
					last_point->x, last_point->y,
					cur_point->x, cur_point->y);
		}
		last_point = cur_point;
	}
}


static void conting_drawing_line_get_points(ContingDrawing *drawing,
        GdkPoint **points, gint *npoints) {
    *points = line_points;
    *npoints = 2;
}

static void conting_drawing_line_get_rectangle(ContingDrawing *self,
        GdkRectangle *rect) {
	ContingDrawingLinePrivate *priv;
	GSList *n;
	gint minx = 0, miny = 0, maxx = 0, maxy = 0;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_LINE(self));
	
	priv = CONTING_DRAWING_LINE_GET_PRIVATE(self);
	
	for (n = priv->points; n != NULL; n = g_slist_next(n)) {
		GdkPoint *p = n->data;

		minx = minx < p->x ? minx : p->x;
		miny = miny < p->y ? miny : p->y;
		maxx = maxx > p->x ? maxx : p->x;
		maxy = maxy > p->y ? maxy : p->y;
	}

	rect->x = minx;
	rect->y = miny;
	rect->width = maxx - minx;
	rect->height = maxy - miny;
}


static void conting_drawing_line_class_init(gpointer g_class,
        gpointer class_data) {
	g_print("drawing_line_class_init()\n");
    ContingDrawingClass *drawing_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_drawing_line_draw;
    drawing_class->get_rectangle = conting_drawing_line_get_rectangle;
}

static void conting_drawing_line_instance_init(GTypeInstance *obj,
        gpointer g_class) {
}

GType conting_drawing_line_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingDrawingLineClass),
            NULL,
            NULL,
            conting_drawing_line_class_init,
            NULL,
            NULL,
            sizeof(ContingDrawingLine),
            0,
            conting_drawing_line_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_DRAWING,
				"ContingDrawingLine",
                &type_info, 0);
    }

    return type;
}

ContingDrawing *conting_drawing_line_new(void) {
	g_print("conting_drawing_line_new()\n");
    return CONTING_DRAWING(g_object_new(CONTING_TYPE_DRAWING_LINE, NULL));
}
