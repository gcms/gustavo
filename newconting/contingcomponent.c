#include "contingcomponent.h"
#include <string.h>

#define CONTING_COMPONENT_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_COMPONENT, ContingComponentPrivate))

typedef struct ContingComponentPrivate_ ContingComponentPrivate;
struct ContingComponentPrivate_ {
	ArtPoint p0, p1;

    gboolean placed;
};

static void
conting_component_draw(ContingDrawing *self,
                  GdkDrawable *drawable,
                  const GdkRectangle *drawing_rect)
{
    ContingComponentPrivate *priv;
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

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	conting_drawing_get_affine(self, affine);

    art_affine_point(&pw0, &priv->p0, affine);

    art_affine_point(&pw1, &priv->p1, affine);

	conting_one_line_world_to_window(conting_drawing_get_one_line(self),
			pw0.x, pw0.y, &pw0.x, &pw0.y);
	conting_one_line_world_to_window(conting_drawing_get_one_line(self),
			pw1.x, pw1.y, &pw1.x, &pw1.y);


//	g_print("drawing: (%lf, %lf); (%lf, %lf)\n", pw0.x, pw0.y, pw1.x, pw1.y);
    gdk_draw_rectangle(drawable, gc, TRUE,
            drawing_rect->x + (gint) pw0.x, drawing_rect->y + (gint) pw0.y,
            (gint) (pw1.x - pw0.x),
            (gint) (pw1.y - pw0.y));
    
}

static void
conting_component_get_bounds(ContingDrawing *self,
		                ArtDRect *bounds)
{
	ContingComponentPrivate *priv;
	gdouble affine[6];
	ArtPoint pw;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	conting_drawing_get_affine(self, affine);

	art_affine_point(&pw, &priv->p0, affine);
	bounds->x0 = pw.x;
	bounds->y0 = pw.y;

	art_affine_point(&pw, &priv->p1, affine);
	bounds->x1 = pw.x;
	bounds->y1 = pw.y;
}

static void
conting_component_place(ContingDrawing *self)
{
	ContingComponentPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	priv->placed = TRUE;
}

static gboolean
conting_component_is_placed(ContingDrawing *self)
{
	ContingComponentPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	return priv->placed;
}

static gboolean
conting_component_answer(ContingDrawing *self,
		                 gdouble world_x, gdouble world_y)
{
	ContingComponentPrivate *priv;
	gdouble affine[6];
	ArtPoint pw0, pw1;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	conting_drawing_get_affine(self, affine);

	art_affine_point(&pw0, &priv->p0, affine);
	art_affine_point(&pw1, &priv->p1, affine);

//	g_print("checking:(%lf, %lf), (%lf, %lf); (%lf, %lf), (%lf, %lf);"
//			"(%lf, %lf)\n", priv->p0.x, priv->p0.y, priv->p1.x, priv->p1.y,
//			pw0.x, pw0.y, pw1.x, pw1.y, world_x, world_y);

	return world_x >= pw0.x && world_x <= pw1.x
		&& world_y >= pw0.y && world_y <= pw1.y;
}

static void
conting_component_instance_init(GTypeInstance *self,
		                   gpointer g_class)
{
	ContingComponentPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	priv->p0.x = -5;
	priv->p0.y = -20;
	priv->p1.x = 5;
	priv->p1.y = 20;

	priv->placed = FALSE;

}

static void
conting_component_class_init(gpointer g_class, gpointer class_data)
{
    ContingDrawingClass *drawing_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_component_draw;
	drawing_class->get_bounds = conting_component_get_bounds;
	drawing_class->place = conting_component_place;
	drawing_class->is_placed = conting_component_is_placed;
	drawing_class->answer = conting_component_answer;

	g_type_class_add_private(g_class, sizeof(ContingComponentPrivate));
}

GType conting_component_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingComponentClass),
            NULL,
            NULL,
            conting_component_class_init,
            NULL,
            NULL,
            sizeof(ContingComponent),
            0,
            conting_component_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_DRAWING,
                "ContingComponent",
                &type_info, 0);
    }

    return type;
}
