#include "contingline.h"
#include <string.h>

#define CONTING_LINE_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_LINE, ContingLinePrivate))

typedef struct ContingLinePrivate_ ContingLinePrivate;
struct ContingLinePrivate_ {
	ArtPoint p0, p1;

    gboolean placing;
    gboolean placed;
};

static void
conting_line_draw(ContingDrawing *self,
                  GdkDrawable *drawable,
                  const GdkRectangle *drawing_rect)
{
    ContingLinePrivate *priv;
	gdouble affine[6];
    ArtPoint pw0, pw1;

    static GdkGC *gc = NULL;
    if (gc == NULL) {
        static GdkColor color;
        gdk_color_parse("black", &color);
        gc = gdk_gc_new(drawable);
        gdk_gc_set_foreground(gc, &color);
        gdk_gc_set_background(gc, &color);
		gdk_gc_set_rgb_fg_color(gc, &color);
		gdk_gc_set_rgb_bg_color(gc, &color);
		gdk_gc_set_fill(gc, GDK_SOLID);
    }

    g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

    priv = CONTING_LINE_GET_PRIVATE(self);

	conting_drawing_get_affine(self, affine);

    art_affine_point(&pw0, &priv->p0, affine);

    art_affine_point(&pw1, &priv->p1, affine);

	conting_one_line_world_to_window(conting_drawing_get_one_line(self),
			pw0.x, pw0.y, &pw0.x, &pw0.y);
	conting_one_line_world_to_window(conting_drawing_get_one_line(self),
			pw1.x, pw1.y, &pw1.x, &pw1.y);


	g_print("drawing: (%lf, %lf); (%lf, %lf)\n", pw0.x, pw0.y, pw1.x, pw1.y);
    gdk_draw_rectangle(drawable, gc, TRUE,
            (gint) pw0.x, (gint) pw0.y,
            (gint) (pw1.x - pw0.x),
            (gint) (pw1.y - pw0.y));
    
}

static void
conting_line_get_bounds(ContingDrawing *self,
		                ArtDRect *bounds)
{
	ContingLinePrivate *priv;
	gdouble affine[6];
	ArtPoint pw;

	g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

    priv = CONTING_LINE_GET_PRIVATE(self);

	conting_drawing_get_affine(self, affine);

	art_affine_point(&pw, &priv->p0, affine);
	bounds->x0 = pw.x;
	bounds->y0 = pw.y;

	art_affine_point(&pw, &priv->p1, affine);
	bounds->x1 = pw.x;
	bounds->y1 = pw.y;
}

static void
conting_line_instance_init(GTypeInstance *self,
		                   gpointer g_class)
{
	ContingLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

    priv = CONTING_LINE_GET_PRIVATE(self);

	priv->p0.x = -5;
	priv->p0.y = -20;
	priv->p1.x = 5;
	priv->p1.y = 20;

	priv->placing = FALSE;
	priv->placed = FALSE;

}

static void conting_line_class_init(gpointer g_class, gpointer class_data) {
    ContingDrawingClass *drawing_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_line_draw;
	drawing_class->get_bounds = conting_line_get_bounds;

	g_type_class_add_private(g_class, sizeof(ContingLinePrivate));
}

GType conting_line_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingLineClass),
            NULL,
            NULL,
            conting_line_class_init,
            NULL,
            NULL,
            sizeof(ContingLine),
            0,
            conting_line_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_DRAWING,
                "ContingLine",
                &type_info, 0);
    }

    return type;
}
