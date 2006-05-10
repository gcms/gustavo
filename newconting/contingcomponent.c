#include "contingcomponent.h"
#include <string.h>
#include <math.h>

#define CONTING_COMPONENT_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_COMPONENT, ContingComponentPrivate))

#define TOLERANCE 3
#define SIZE ((TOLERANCE * 2) - 1)

typedef struct ContingComponentPrivate_ ContingComponentPrivate;
struct ContingComponentPrivate_ {
	ArtPoint p0, p1;

    gboolean placed;

	gboolean dragging;
	ArtPoint dragging_point;
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

	if (conting_drawing_is_selected(self)) {
		gdk_draw_rectangle(drawable, gc, TRUE,
				(gint) (pw0.x - TOLERANCE), (gint) (pw0.y - TOLERANCE),
				SIZE, SIZE);
		gdk_draw_rectangle(drawable, gc, TRUE,
				(gint) (pw1.x - TOLERANCE), (gint) (pw0.y - TOLERANCE),
				SIZE, SIZE);
		gdk_draw_rectangle(drawable, gc, TRUE,
				(gint) (pw0.x - TOLERANCE), (gint) (pw1.y - TOLERANCE),
				SIZE, SIZE);
		gdk_draw_rectangle(drawable, gc, TRUE,
				(gint) (pw1.x - TOLERANCE), (gint) (pw1.y - TOLERANCE),
				SIZE, SIZE);
		
	}
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

	bounds->x0 -= TOLERANCE;
	bounds->x1 += TOLERANCE;
	bounds->y0 -= TOLERANCE;
	bounds->y1 += TOLERANCE;
}

static void
conting_component_place(ContingDrawing *self)
{
	ContingComponentPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	priv->placed = TRUE;

	conting_drawing_set_selected(self, TRUE);
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
	priv->dragging = FALSE;
}

static gboolean
conting_component_event(ContingDrawing *self,
		                GdkEvent *event)
{
	ContingComponentPrivate *priv;
	ArtPoint p;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	conting_one_line_window_to_world(conting_drawing_get_one_line(self),
			event->button.x, event->button.y,
			&p.x, &p.y);
	

	switch (event->type) {
		case GDK_BUTTON_PRESS:
			conting_drawing_set_selected(self, TRUE);
			conting_drawing_grab(self);
			priv->dragging_point = p;
			priv->dragging = TRUE;
			break;
		case GDK_MOTION_NOTIFY:
			if (priv->dragging) {
				gdouble affine[6];
				art_affine_translate(affine,
						p.x - priv->dragging_point.x,
						p.y - priv->dragging_point.y);
				conting_drawing_affine(self, affine);
				g_signal_emit_by_name(self, "move", affine);
				priv->dragging_point = p;
			}
			break;
		case GDK_BUTTON_RELEASE:
			if (priv->dragging) {
				priv->dragging = FALSE;
				conting_drawing_ungrab(self);
			}
			break;
		default:
			return FALSE;
	}

	return TRUE;
}

gboolean
conting_component_link(ContingComponent *self,
		               ContingDrawing *drawing,
					   gdouble world_x, gdouble world_y,
					   gdouble affine[6])
{
	ContingComponentPrivate *priv;
	ArtPoint pi;
	gdouble invert[6], my_affine[6];

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	conting_drawing_get_affine(CONTING_DRAWING(self), my_affine);
	art_affine_invert(invert, my_affine);
	pi.x = world_x;
	pi.y = world_y;
	art_affine_point(&pi, &pi, invert);
	
	g_print("link: (%lf, %lf); (%lf, %lf) : (%lf, %lf)\n",
			priv->p0.x, priv->p0.y, priv->p1.x, priv->p1.y,
			pi.x, pi.y);

	if (pi.y < priv->p0.y || pi.y > priv->p1.y
			|| pi.x < priv->p0.x || pi.x > priv->p1.x) {
		return FALSE;
	}

	if (fabs(pi.x - priv->p0.x) < fabs(pi.x - priv->p1.x)) {
		art_affine_translate(affine, priv->p0.x - pi.x, 0);
	} else {
		art_affine_translate(affine, priv->p1.x - pi.x, 0);
	}

	return TRUE;
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
	drawing_class->event = conting_component_event;

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
