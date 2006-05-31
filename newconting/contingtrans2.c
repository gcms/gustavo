#include "contingtrans2.h"
#include "contingutil.h"
#include <string.h>
#include <math.h>

#define CONTING_TRANS2_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_TRANS2, ContingTrans2Private))

#define TOLERANCE 3
#define SIZE ((TOLERANCE * 2) - 1)

static gpointer parent_class = NULL;

typedef struct ContingTrans2Private_ ContingTrans2Private;
struct ContingTrans2Private_ {
    ArtPoint p0, p1;

    gboolean placed;

    gboolean dragging;
    ArtPoint dragging_point;

    ContingDrawing *link0, *link1;

    gdouble rotate[6];
};

static void
conting_trans2_draw(ContingDrawing *self,
                  GdkDrawable *drawable,
                  const GdkRectangle *drawing_rect)
{
    ContingTrans2Private *priv;
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

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);

    conting_drawing_get_affine(self, affine);

    pw0 = priv->p0;
    pw1 = priv->p1;
    pw1.y = pw1.y - (pw1.y - pw0.y) / 3;

    art_affine_point(&pw0, &pw0, affine);
    art_affine_point(&pw1, &pw1, affine);
    conting_one_line_world_to_window(conting_drawing_get_one_line(self),
            pw0.x, pw0.y, &pw0.x, &pw0.y);
    conting_one_line_world_to_window(conting_drawing_get_one_line(self),
            pw1.x, pw1.y, &pw1.x, &pw1.y);

    rect.x = (pw0.x < pw1.x ? pw0.x : pw1.x);
    rect.y = (pw0.y < pw1.y ? pw0.y : pw1.y);
    rect.width = fabs(pw1.x - pw0.x);
    rect.height = fabs(pw1.y - pw0.y);

    gdk_draw_arc(drawable, gc, FALSE,
            rect.x, rect.y, rect.width, rect.height, 0, 360 * 64);

    pw0 = priv->p0;
    pw1 = priv->p1;
    pw0.y = pw0.y + (pw1.y - pw0.y) / 3;

    art_affine_point(&pw0, &pw0, affine);
    art_affine_point(&pw1, &pw1, affine);
    conting_one_line_world_to_window(conting_drawing_get_one_line(self),
            pw0.x, pw0.y, &pw0.x, &pw0.y);
    conting_one_line_world_to_window(conting_drawing_get_one_line(self),
            pw1.x, pw1.y, &pw1.x, &pw1.y);

    rect.x = (pw0.x < pw1.x ? pw0.x : pw1.x);
    rect.y = (pw0.y < pw1.y ? pw0.y : pw1.y);
    rect.width = fabs(pw1.x - pw0.x);
    rect.height = fabs(pw1.y - pw0.y);

    gdk_draw_arc(drawable, gc, FALSE,
            rect.x, rect.y, rect.width, rect.height, 0, 360 * 64);

    art_affine_point(&pw0, &priv->p0, affine);
    art_affine_point(&pw1, &priv->p1, affine);
    conting_one_line_world_to_window(conting_drawing_get_one_line(self),
            pw0.x, pw0.y, &pw0.x, &pw0.y);
    conting_one_line_world_to_window(conting_drawing_get_one_line(self),
            pw1.x, pw1.y, &pw1.x, &pw1.y);

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
static gboolean
conting_trans2_get_link_point(ContingComponent *self,
                                 ContingDrawing *line,
                                 ArtPoint *p)
{
    ContingTrans2Private *priv;
    ArtPoint point;
    gdouble affine[6];

    g_return_val_if_fail(self != NULL && CONTING_IS_TRANS2(self), FALSE);

    priv = CONTING_TRANS2_GET_PRIVATE(self);

    point.x = 0;
    if (line == priv->link0) {
        point.y = priv->p0.y;
    } else if (line == priv->link1) {
        point.y = priv->p1.y;
    } else {
        return FALSE;
    }


    *p = point;
    conting_drawing_get_affine(CONTING_DRAWING(self), affine);

    art_affine_point(p, p, affine);

    return TRUE;
}

static void
conting_trans2_get_bounds(ContingDrawing *self,
                        ArtDRect *bounds)
{
    ContingTrans2Private *priv;
    gdouble affine[6];
    ArtPoint pw0, pw1;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);

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
conting_trans2_place(ContingDrawing *self)
{
    ContingTrans2Private *priv;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);

    priv->placed = TRUE;

    conting_drawing_set_selected(self, TRUE);
}

static gboolean
conting_trans2_is_placed(ContingDrawing *self)
{
    ContingTrans2Private *priv;

    g_return_val_if_fail(self != NULL && CONTING_IS_TRANS2(self), FALSE);

    priv = CONTING_TRANS2_GET_PRIVATE(self);

    return priv->placed;
}

static gboolean
conting_trans2_answer(ContingDrawing *self,
                         gdouble world_x, gdouble world_y)
{
    ContingTrans2Private *priv;
    ArtDRect bounds;

    g_return_val_if_fail(self != NULL && CONTING_IS_TRANS2(self), FALSE);

    priv = CONTING_TRANS2_GET_PRIVATE(self);

    conting_drawing_get_bounds(self, &bounds);
    
    return world_x >= bounds.x0 && world_x <= bounds.x1
        && world_y >= bounds.y0 && world_y <= bounds.y1;
}

static void
conting_trans2_finalize(GObject *self)
{
    ContingTrans2Private *priv;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void
conting_trans2_instance_init(GTypeInstance *self,
                           gpointer g_class)
{
    ContingTrans2Private *priv;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);

    priv->p0.x = -5;
    priv->p0.y = -8.5;
    priv->p1.x = 5;
    priv->p1.y = 8.5;

    priv->placed = FALSE;
    priv->dragging = FALSE;

    priv->link0 = priv->link1 = NULL;

    art_affine_rotate(priv->rotate, 0.0);
}
static void
conting_trans2_disconnect_link(ContingTrans2 *self,
                              ContingDrawing *drawing)
{
    ContingTrans2Private *priv;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);

    g_print("line %p disconnected from %p\n", drawing, self);

    g_signal_handlers_disconnect_matched(drawing,
            G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, self);
}
static void conting_trans2_delete(ContingDrawing *self)
{
    ContingTrans2Private *priv;

    g_print("DELETING\n");

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);

    if (priv->dragging) {
        priv->dragging = FALSE;
        conting_drawing_ungrab(self);
    }

	if (priv->link0) {
        conting_trans2_disconnect_link(CONTING_TRANS2(self), priv->link0);
	}
	if (priv->link1) {
        conting_trans2_disconnect_link(CONTING_TRANS2(self), priv->link1);
    }
    
	priv->link0 = priv->link1 = NULL;

    g_signal_emit_by_name(self, "delete");

	CONTING_DRAWING_CLASS(parent_class)->delete(self);
}
#include <gdk/gdkkeysyms.h>
static gboolean
conting_trans2_event_place(ContingDrawing *self,
                        GdkEvent *event)
{
    ContingTrans2Private *priv;
    ArtPoint p;
    gdouble affine[6];

    g_return_val_if_fail(self != NULL && CONTING_IS_TRANS2(self), FALSE);

    priv = CONTING_TRANS2_GET_PRIVATE(self);

    conting_one_line_window_to_world(conting_drawing_get_one_line(self),
            event->button.x, event->button.y,
            &p.x, &p.y);

    switch (event->type) {
        case GDK_MOTION_NOTIFY:
            art_affine_translate(affine, p.x, p.y);
            conting_drawing_affine_absolute(self, affine);
            break;
        case GDK_KEY_PRESS:
            if (event->key.keyval == GDK_r) {
                gdouble rotate[6];
                art_affine_rotate(rotate, 90.0);
                art_affine_multiply(priv->rotate, priv->rotate, rotate);

//                g_signal_emit_by_name(self, "move");

                conting_drawing_update(self);
            } else if (event->key.keyval == GDK_Escape) {
                conting_drawing_delete(self);
            }
            break;
        default:
            break;
    }
    return TRUE;
}

static gboolean
conting_trans2_event(ContingDrawing *self,
                        GdkEvent *event)
{
    ContingTrans2Private *priv;
    ArtPoint p;

    g_return_val_if_fail(self != NULL && CONTING_IS_TRANS2(self), FALSE);

    priv = CONTING_TRANS2_GET_PRIVATE(self);

    if (!priv->placed)
        return conting_trans2_event_place(self, event);

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
                g_signal_emit_by_name(self, "move");
                priv->dragging_point = p;
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
                conting_drawing_delete(self);
            }
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


static void
conting_trans2_link_deleted(ContingDrawing *drawing,
                               gpointer user_data)
{
    ContingTrans2Private *priv;

    g_return_if_fail(user_data != NULL && CONTING_IS_TRANS2(user_data));

    priv = CONTING_TRANS2_GET_PRIVATE(user_data);

    g_print("link %p deleted from %p\n", drawing, user_data);
	if (drawing == priv->link0) {
		priv->link0 = NULL;
	} else if (drawing == priv->link1) {
		priv->link1 = NULL;
	} else {
		return;
	}
    conting_trans2_disconnect_link(CONTING_TRANS2(user_data), drawing);
}

static gboolean
conting_trans2_link(ContingComponent *self,
                    ContingDrawing *drawing,
                    gdouble world_x, gdouble world_y,
                    ArtPoint *pw)
{
    ContingTrans2Private *priv;
    ArtPoint pi;
    gdouble invert[6], my_affine[6];

    g_return_val_if_fail(self != NULL && CONTING_IS_TRANS2(self), FALSE);

    priv = CONTING_TRANS2_GET_PRIVATE(self);

	if (drawing == priv->link0
			|| drawing == priv->link1) {
		return FALSE;
	}

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

    if (fabs(pi.y - priv->p0.y) < fabs(pi.y - priv->p1.y)) {
//        art_affine_translate(affine, priv->p0.x - pi.x, 0);
        pi.y = priv->p0.y;
		priv->link0 = drawing;
    } else {
//        art_affine_translate(affine, priv->p1.x - pi.x, 0);
        pi.y = priv->p1.y;
		priv->link1 = drawing;
    }
	pi.x = 0;

    conting_drawing_get_affine(CONTING_DRAWING(self), my_affine);
    art_affine_point(pw, &pi, my_affine);

    g_signal_connect(G_OBJECT(drawing), "delete",
            G_CALLBACK(conting_trans2_link_deleted), self);

    return TRUE;
}
static void
conting_trans2_get_affine(ContingDrawing *self,
                             gdouble affine[6])
{
    ContingTrans2Private *priv;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);

    CONTING_DRAWING_CLASS(parent_class)->get_affine(self, affine);

    art_affine_multiply(affine, priv->rotate, affine);
}
static void
conting_trans2_class_init(gpointer g_class, gpointer class_data)
{
    ContingDrawingClass *drawing_class;
    ContingComponentClass *component_class;
    GObjectClass *gobject_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_trans2_draw;
    drawing_class->get_bounds = conting_trans2_get_bounds;
    drawing_class->place = conting_trans2_place;
    drawing_class->is_placed = conting_trans2_is_placed;
    drawing_class->answer = conting_trans2_answer;
    drawing_class->event = conting_trans2_event;
    drawing_class->get_affine = conting_trans2_get_affine;
    drawing_class->delete = conting_trans2_delete;

    component_class = CONTING_COMPONENT_CLASS(g_class);
    component_class->link = conting_trans2_link;
    component_class->get_link_point = conting_trans2_get_link_point;

    gobject_class = G_OBJECT_CLASS(g_class);
    gobject_class->finalize = conting_trans2_finalize;

    g_type_class_add_private(g_class, sizeof(ContingTrans2Private));

    parent_class = g_type_class_peek_parent(g_class);
}

GType conting_trans2_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingTrans2Class),
            NULL,
            NULL,
            conting_trans2_class_init,
            NULL,
            NULL,
            sizeof(ContingTrans2),
            0,
            conting_trans2_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_COMPONENT,
                "ContingTrans2",
                &type_info, 0);
    }

    return type;
}
