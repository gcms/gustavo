#include "contingdrawingline.h"

static GdkPoint line_points[2] = {
    { -10, 0 }, { 10, 0 }
};

static void conting_drawing_line_draw(ContingDrawing *self,
        GdkDrawable *drawable, GdkGC *gc, const GdkPoint *pos) {
	g_print("conting_drawing_line_draw()\n");
    gdk_draw_line(drawable, gc,
            pos->x + line_points[0].x, pos->y + line_points[0].y,
            pos->x + line_points[1].x, pos->y + line_points[1].y);
}


static void conting_drawing_line_get_points(ContingDrawing *drawing,
        GdkPoint **points, gint *npoints) {
    *points = line_points;
    *npoints = 2;
}

static void conting_drawing_line_get_rectangle(ContingDrawing *self,
        GdkRectangle *rect) {
    rect->width = line_points[1].x - line_points[0].x + 1;
    rect->height = line_points[1].y - line_points[0].y + 1;
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
