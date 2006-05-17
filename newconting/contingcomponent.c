#include "contingcomponent.h"
#include <string.h>
#include <math.h>

#define CONTING_COMPONENT_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_COMPONENT, ContingComponentPrivate))

#define TOLERANCE 3
#define SIZE ((TOLERANCE * 2) - 1)

static gpointer parent_class = NULL;

typedef struct ContingComponentPrivate_ ContingComponentPrivate;
struct ContingComponentPrivate_ {
	ArtPoint p0, p1;

    gboolean placed;

	gboolean dragging;
	ArtPoint dragging_point;

	GHashTable *points;
	GSList *links;

	ArtPoint *start_resize;

	gdouble rotate[6];
};

static void
conting_component_draw(ContingDrawing *self,
                  GdkDrawable *drawable,
                  const GdkRectangle *drawing_rect)
{
    ContingComponentPrivate *priv;
	gdouble affine[6];
    ArtPoint pw0, pw1;
	GdkRectangle rect;

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


	g_print("drawing: (%lf, %lf); (%lf, %lf)\n", pw0.x, pw0.y, pw1.x, pw1.y);
	
	rect.x = (gint) (pw0.x < pw1.x ? pw0.x : pw1.x);
	rect.y = (gint) (pw0.y < pw1.y ? pw0.y : pw1.y);
	rect.width = (gint) fabs(pw0.x - pw1.x);
	rect.height = (gint) fabs(pw0.y - pw1.y);

	gdk_draw_rectangle(drawable, gc, TRUE,
			rect.x, rect.y, rect.width, rect.height);

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
gboolean
conting_component_get_link_point(ContingComponent *self,
                                 ContingDrawing *line,
                                 ArtPoint *p)
{
	ContingComponentPrivate *priv;
	ArtPoint *point;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	point = g_hash_table_lookup(priv->points, line);

	if (point == NULL) {
		return FALSE;
	} else if (p != NULL) {
		gdouble affine[6];

		*p = *point;
		conting_drawing_get_affine(CONTING_DRAWING(self), affine);

		art_affine_point(p, p, affine);

		return TRUE;
	}

	return FALSE;
}

static void
conting_component_get_bounds(ContingDrawing *self,
		                ArtDRect *bounds)
{
	ContingComponentPrivate *priv;
	gdouble affine[6];
	ArtPoint pw0, pw1;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	conting_drawing_get_affine(self, affine);

	art_affine_point(&pw0, &priv->p0, affine);
	art_affine_point(&pw1, &priv->p1, affine);

	bounds->x0 = MIN(pw0.x, pw1.x);
	bounds->y0 = MIN(pw0.y, pw1.y);
	bounds->x1 = MAX(pw0.x, pw1.x);
	bounds->y1 = MAX(pw0.y, pw1.y);

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
	ArtDRect bounds;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

    priv = CONTING_COMPONENT_GET_PRIVATE(self);


	conting_drawing_get_bounds(self, &bounds);
	
	return world_x >= bounds.x0 && world_x <= bounds.x1
		&& world_y >= bounds.y0 && world_y <= bounds.y1;
}

static void
conting_component_instance_init(GTypeInstance *self,
		                   gpointer g_class)
{
	ContingComponentPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	priv->p0.x = -3;
	priv->p0.y = -20;
	priv->p1.x = 3;
	priv->p1.y = 20;

	priv->placed = FALSE;
	priv->dragging = FALSE;
	priv->start_resize = NULL;

	priv->points = g_hash_table_new(NULL, NULL);
	priv->links = NULL;

	art_affine_rotate(priv->rotate, 0.0);
}
static void
conting_component_center(ContingComponent *self)
{
	ContingComponentPrivate *priv;
	gdouble w, h;
	ArtPoint np0, np1;
	gdouble translate[6];

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	w = fabs(priv->p0.x - priv->p1.x);
	h = fabs(priv->p0.y - priv->p1.y);

	np0.x = - (w / 2);
	np0.y = - (h / 2);
	np1.x = np0.x + w;
	np1.y = np0.y + h;

	art_affine_translate(translate,
			priv->p0.x - np0.x,
			priv->p0.y - np0.y);
	
	priv->p0 = np0;
	priv->p1 = np1;

	conting_drawing_affine(CONTING_DRAWING(self), translate);
}
#include <gdk/gdkkeysyms.h>
static gboolean
conting_component_event_place(ContingDrawing *self,
		                GdkEvent *event)
{
	ContingComponentPrivate *priv;
	ArtPoint p;
	gdouble affine[6];

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	conting_one_line_window_to_world(conting_drawing_get_one_line(self),
			event->button.x, event->button.y,
			&p.x, &p.y);

	art_affine_translate(affine, p.x, p.y);
	conting_drawing_affine_absolute(self, affine);

	switch (event->type) {
		case GDK_KEY_PRESS:
			if (event->key.keyval == GDK_r) {
				gdouble rotate[6];
				art_affine_rotate(rotate, 90.0);
				art_affine_multiply(priv->rotate, priv->rotate, rotate);

//				g_signal_emit_by_name(self, "move");

				conting_drawing_update(self);
			}
			break;
		default:
			break;
	}
	return TRUE;
}

static gboolean
conting_component_event(ContingDrawing *self,
		                GdkEvent *event)
{
	ContingComponentPrivate *priv;
	ArtPoint p;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

	if (!priv->placed)
		return conting_component_event_place(self, event);

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
			if (priv->dragging && priv->start_resize != NULL) {
				ArtPoint pi;
				gdouble invert[6];

				CONTING_DRAWING_CLASS(parent_class)->get_affine(self, invert);
				art_affine_invert(invert, invert);
				art_affine_point(&pi, &p, invert);

				art_affine_invert(invert, priv->rotate);
				art_affine_point(&pi, &pi, invert);

				if ((priv->start_resize == &priv->p0
					   && fabs(pi.y - priv->p1.y) > 20)
						|| (priv->start_resize == &priv->p1
							&& fabs(pi.y - priv->p0.y) > 20)) {
					priv->start_resize->y = pi.y;
				}

				conting_drawing_update(self);
			} else if (priv->dragging) {
				gdouble affine[6];
				art_affine_translate(affine,
						p.x - priv->dragging_point.x,
						p.y - priv->dragging_point.y);
				conting_drawing_affine(self, affine);
				g_signal_emit_by_name(self, "move");
				priv->dragging_point = p;
			} else {
				ArtPoint pi;
				gdouble invert[6];

				CONTING_DRAWING_CLASS(parent_class)->get_affine(self, invert);
				art_affine_invert(invert, invert);
				art_affine_point(&pi, &p, invert);

				art_affine_invert(invert, priv->rotate);
				art_affine_point(&pi, &pi, invert);

				g_print("internal: (%lf, %lf)\n", pi.x, pi.y);

				if (fabs(pi.y - priv->p0.y) < TOLERANCE) {
					priv->start_resize = &priv->p0;
				} else if (fabs(pi.y - priv->p1.y) < TOLERANCE) {
					priv->start_resize = &priv->p1;
				} else {
					priv->start_resize = NULL;
				}
			}
			break;
		case GDK_BUTTON_RELEASE:
			if (priv->dragging) {
				priv->dragging = FALSE;
				conting_drawing_ungrab(self);
			}
			break;
		case GDK_KEY_PRESS:
			if (event->key.keyval == GDK_r) {
				gdouble rotate[6];
				art_affine_rotate(rotate, 90.0);
				art_affine_multiply(priv->rotate, priv->rotate, rotate);

				g_signal_emit_by_name(self, "move");

				conting_drawing_update(self);
			} else if (event->key.keyval == GDK_Delete) {
				g_signal_emit_by_name(self, "delete");
				conting_drawing_delete(self);
			}
			break;
		default:
			return FALSE;
	}

	return TRUE;
}

static void
conting_component_link_deleted(ContingDrawing *drawing,
		                       gpointer user_data)
{
	ContingComponentPrivate *priv;

	g_return_if_fail(user_data != NULL && CONTING_IS_COMPONENT(user_data));

	priv = CONTING_COMPONENT_GET_PRIVATE(user_data);

	priv->links = g_slist_remove(priv->links, drawing);
	g_hash_table_remove(priv->points, drawing);
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

	if (g_slist_find(priv->links, drawing))
		return FALSE;

	pi.x = world_x;
	pi.y = world_y;
	CONTING_DRAWING_CLASS(parent_class)->get_affine(CONTING_DRAWING(self),
			my_affine);
	art_affine_invert(invert, my_affine);
	art_affine_point(&pi, &pi, invert);

	art_affine_invert(invert, priv->rotate);
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
		pi.x = priv->p0.x;
	} else {
		art_affine_translate(affine, priv->p1.x - pi.x, 0);
		pi.x = priv->p1.x;
	}

	ArtPoint *p = g_new(ArtPoint, 1);
	*p = pi;

	priv->links = g_slist_append(priv->links, drawing);
	g_hash_table_insert(priv->points, drawing, p); 
	g_signal_connect(G_OBJECT(drawing), "delete",
			G_CALLBACK(conting_component_link_deleted), self);

	return TRUE;
}
static void
conting_component_get_affine(ContingDrawing *self,
		                     gdouble affine[6])
{
	ContingComponentPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	CONTING_DRAWING_CLASS(parent_class)->get_affine(self, affine);

	art_affine_multiply(affine, priv->rotate, affine);
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
	drawing_class->get_affine = conting_component_get_affine;

	g_type_class_add_private(g_class, sizeof(ContingComponentPrivate));

	parent_class = g_type_class_peek_parent(g_class);
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
