#include "contingline.h"
#include <string.h>

#define CONTING_LINE_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_LINE, ContingLinePrivate))

typedef struct ContingLinePrivate_ ContingLinePrivate;
struct ContingLinePrivate_ {
	GSList *points;
	gint n_points;

	gdouble affine[6];

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
	GSList *n;
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

	if (!priv->placing && !priv->placed)
		return;

	pw0 = *((ArtPoint *) priv->points->data);
	art_affine_point(&pw0, &pw0, priv->affine);
	conting_one_line_world_to_window(conting_drawing_get_one_line(self),
			pw0.x, pw0.y, &pw0.x, &pw0.y);

	for (n = g_slist_next(priv->points); n != NULL; n = g_slist_next(n)) {
		pw1 = *((ArtPoint *) n->data);
		art_affine_point(&pw1, &pw1, priv->affine);
		conting_one_line_world_to_window(conting_drawing_get_one_line(self),
				pw1.x, pw1.y, &pw1.x, &pw1.y);

		g_print("drawing: (%lf, %lf); (%lf, %lf)\n",
				pw0.x, pw0.y, pw1.x, pw1.y);
		gdk_draw_line(drawable, gc,
				(gint) pw0.x, (gint) pw0.y,
				(gint) pw1.x, (gint) pw1.y);

		pw0 = pw1;
	}

	if (priv->placing) {
		pw1.x = pw1.y = 0.0;
		art_affine_point(&pw1, &pw1, affine);
		conting_one_line_world_to_window(conting_drawing_get_one_line(self),
				pw1.x, pw1.y, &pw1.x, &pw1.y);

		gdk_draw_line(drawable, gc,
				(gint) pw0.x, (gint) pw0.y,
				(gint) pw1.x, (gint) pw1.y);
	}
}

static void
conting_line_get_bounds(ContingDrawing *self,
		                ArtDRect *bounds)
{
	ContingLinePrivate *priv;
	gdouble affine[6];
	ArtPoint pw;
	GSList *n;

	g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

    priv = CONTING_LINE_GET_PRIVATE(self);

	conting_drawing_get_affine(self, affine);

	pw.x = pw.y = 0;
	art_affine_point(&pw, &pw, affine);
	bounds->x0 = bounds->x1 = pw.x;
	bounds->y0 = bounds->y1 = pw.y;

	for (n = priv->points; n != NULL; n = g_slist_next(n)) {
		art_affine_point(&pw, (ArtPoint *) n->data, affine);

		if (pw.x < bounds->x0) {
			bounds->x0 = pw.x;
		} else if (pw.x > bounds->x1) {
			bounds->x1 = pw.x;
		}

		if (pw.y < bounds->y0) {
			bounds->y0 = pw.y;
		} else if (pw.y > bounds->y1) {
			bounds->y1 = pw.y;
		}
	}
}

static void
conting_line_instance_init(GTypeInstance *self,
		                   gpointer g_class)
{
	ContingLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

    priv = CONTING_LINE_GET_PRIVATE(self);

	priv->points = NULL;

	priv->placing = FALSE;
	priv->placed = FALSE;

}

static void
conting_line_place(ContingDrawing *self)
{
	ContingLinePrivate *priv;
	gdouble affine[6];
	ArtPoint *cur_p;

	g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

    priv = CONTING_LINE_GET_PRIVATE(self);

	conting_drawing_get_affine(self, affine);

	cur_p = g_new(ArtPoint, 1);
	cur_p->x = cur_p->y = 0.0;		

	if (!priv->placing) {
		memcpy(priv->affine, affine, 6 * sizeof(gdouble));

		priv->points = g_slist_append(priv->points, cur_p);

		priv->placing = TRUE;
	} else {
		gdouble tmp[6];

		art_affine_invert(tmp, priv->affine);
		art_affine_multiply(tmp, tmp, affine);

		art_affine_point(cur_p, cur_p, tmp);
		
		priv->points = g_slist_append(priv->points, cur_p);
	}

	priv->n_points++;
	if (priv->n_points >= 5) {
		priv->placed = TRUE;
		priv->placing = FALSE;
	}
}

static gboolean
conting_line_is_placed(ContingDrawing *self)
{
	ContingLinePrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_LINE(self), FALSE);

    priv = CONTING_LINE_GET_PRIVATE(self);

	return priv->placed;
}

static void conting_line_class_init(gpointer g_class, gpointer class_data) {
    ContingDrawingClass *drawing_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_line_draw;
	drawing_class->get_bounds = conting_line_get_bounds;
	drawing_class->place = conting_line_place;
	drawing_class->is_placed = conting_line_is_placed;

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
