#include "contingdrawing.h"
#include "continggroup.h"
#include "contingutil.h"
#include "contingline.h"
#include <assert.h>


#define CONTING_GROUP_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_GROUP, ContingGroupPrivate))

#define TOLERANCE 3

static gpointer parent_class = NULL;

typedef struct ContingGroupPrivate_ ContingGroupPrivate;
struct ContingGroupPrivate_ {
	GSList *drawings;

	ContingDrawing *grabbed_drawing;

	gboolean dragging;
	ArtPoint dragging_point;
};

static void
conting_group_draw(ContingDrawing *self,
                  GdkDrawable *drawable,
                  const GdkRectangle *drawing_rect)
{
    ContingGroupPrivate *priv;
	GSList *n;

    g_return_if_fail(self != NULL && CONTING_IS_GROUP(self));

    priv = CONTING_GROUP_GET_PRIVATE(self);

	for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
		ContingDrawing *drawing = n->data;

		conting_drawing_set_selected(drawing,
				conting_drawing_is_selected(self));

		conting_drawing_draw(drawing, drawable, drawing_rect);
	}
}
static void
conting_group_get_update_bounds(ContingDrawing *self,
	        	                ArtDRect *bounds)
{
	ContingGroupPrivate *priv;
	GSList *n;
	ContingDrawing *drawing;
	ArtDRect d_bounds;

	g_return_if_fail(self != NULL && CONTING_IS_GROUP(self));

    priv = CONTING_GROUP_GET_PRIVATE(self);

	if (!priv->drawings) {
		bounds->x0 = bounds->y0 = bounds->x1 = bounds->y1 = 0.0;
		return;
	}


	drawing = priv->drawings->data;
	conting_drawing_get_update_bounds(drawing, &d_bounds);
	*bounds = d_bounds;

	for (n = g_slist_next(priv->drawings); n != NULL; n = g_slist_next(n)) {
		drawing	= n->data;
		conting_drawing_get_update_bounds(drawing, &d_bounds);

		conting_util_union_bounds(&d_bounds, bounds, bounds);
	}

	bounds->x0 -= TOLERANCE;
	bounds->x1 += TOLERANCE;
	bounds->y0 -= TOLERANCE;
	bounds->y1 += TOLERANCE;
}

static void
conting_group_get_bounds(ContingDrawing *self,
		                ArtDRect *bounds)
{
	ContingGroupPrivate *priv;
	GSList *n;
	ContingDrawing *drawing;
	ArtDRect d_bounds;

	g_return_if_fail(self != NULL && CONTING_IS_GROUP(self));

    priv = CONTING_GROUP_GET_PRIVATE(self);

	if (!priv->drawings) {
		bounds->x0 = bounds->y0 = bounds->x1 = bounds->y1 = 0.0;
		return;
	}


	drawing = priv->drawings->data;
	conting_drawing_get_bounds(drawing, &d_bounds);
	*bounds = d_bounds;

	for (n = g_slist_next(priv->drawings); n != NULL; n = g_slist_next(n)) {
		drawing	= n->data;
		conting_drawing_get_bounds(drawing, &d_bounds);

		conting_util_union_bounds(&d_bounds, bounds, bounds);
	}
}

static void
conting_group_place(ContingDrawing *self)
{
	ContingGroupPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_GROUP(self));

    priv = CONTING_GROUP_GET_PRIVATE(self);
/*
	priv->placed = TRUE;
*/
	conting_drawing_set_selected(self, TRUE);
}

static gboolean
conting_group_is_placed(ContingDrawing *self)
{
	ContingGroupPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_GROUP(self), FALSE);

    priv = CONTING_GROUP_GET_PRIVATE(self);

	return TRUE;
}

static gboolean
conting_group_answer(ContingDrawing *self,
		                 gdouble world_x, gdouble world_y)
{
	ContingGroupPrivate *priv;
	GSList *n;

	g_return_val_if_fail(self != NULL && CONTING_IS_GROUP(self), FALSE);

    priv = CONTING_GROUP_GET_PRIVATE(self);

	for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
		ContingDrawing *drawing = n->data;
		if (conting_drawing_answer(drawing, world_x, world_y)) {
			return TRUE;
		}
	}

	return FALSE;
}

static void
conting_group_instance_init(GTypeInstance *self,
		                   gpointer g_class)
{
	ContingGroupPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_GROUP(self));

    priv = CONTING_GROUP_GET_PRIVATE(self);

	priv->drawings = NULL;
}

static void conting_group_delete(ContingDrawing *self)
{
	ContingGroupPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_GROUP(self));

	priv = CONTING_GROUP_GET_PRIVATE(self);

	CONTING_DRAWING_CLASS(parent_class)->delete(self);
}

static gboolean
conting_group_event(ContingDrawing *self,
		                GdkEvent *event)
{
	ContingGroupPrivate *priv;
	GSList *n;
	ArtPoint pw;

	g_return_val_if_fail(self != NULL && CONTING_IS_GROUP(self), FALSE);

    priv = CONTING_GROUP_GET_PRIVATE(self);

	conting_one_line_window_to_world(conting_drawing_get_one_line(self),
			event->button.x, event->button.y,
			&pw.x, &pw.y);

	switch (event->type) {
		case GDK_BUTTON_PRESS:
			conting_drawing_set_selected(self, TRUE);
			conting_drawing_grab(self);
			priv->dragging_point = pw;
			priv->dragging = TRUE;
			break;
		case GDK_MOTION_NOTIFY:
			g_print("DRAGGING\n");
			if (priv->dragging) {
				gdouble affine[6];
				art_affine_translate(affine,
						pw.x - priv->dragging_point.x,
						pw.y - priv->dragging_point.y);
				for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
					conting_drawing_affine(CONTING_DRAWING(n->data), affine);
					g_signal_emit_by_name(n->data, "move");
				}
				priv->dragging_point = pw;
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

static void
conting_group_class_init(gpointer g_class, gpointer class_data)
{
    ContingDrawingClass *drawing_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_group_draw;
	drawing_class->get_bounds = conting_group_get_bounds;
	drawing_class->get_update_bounds = conting_group_get_update_bounds;
	drawing_class->place = conting_group_place;
	drawing_class->is_placed = conting_group_is_placed;
	drawing_class->answer = conting_group_answer;
	drawing_class->event = conting_group_event;
	drawing_class->delete = conting_group_delete;

	g_type_class_add_private(g_class, sizeof(ContingGroupPrivate));

	parent_class = g_type_class_peek_parent(g_class);
}

GType conting_group_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingGroupClass),
            NULL,
            NULL,
            conting_group_class_init,
            NULL,
            NULL,
            sizeof(ContingGroup),
            0,
            conting_group_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_DRAWING,
                "ContingGroup",
                &type_info, 0);
    }

    return type;
}

void
conting_group_add_drawing(ContingGroup *self,
						  ContingDrawing *drawing)
{
	ContingGroupPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_GROUP(self));

	priv = CONTING_GROUP_GET_PRIVATE(self);

	g_object_set(G_OBJECT(drawing),
			"group", self,
			NULL);

	priv->drawings = g_slist_append(priv->drawings, drawing);
}
void
conting_group_grab(ContingGroup *group,
				   ContingDrawing *drawing)
{
	ContingGroupPrivate *priv;

	g_return_if_fail(group != NULL && CONTING_IS_GROUP(group));

	priv = CONTING_GROUP_GET_PRIVATE(group);

}
void
conting_group_ungrab(ContingGroup *group,
		             ContingDrawing *drawing)
{
	ContingGroupPrivate *priv;

	g_return_if_fail(group != NULL && CONTING_IS_GROUP(group));

	priv = CONTING_GROUP_GET_PRIVATE(group);
}

gboolean
conting_group_contains(ContingGroup *group,
		               ContingDrawing *drawing)
{
	ContingGroupPrivate *priv;

	g_return_val_if_fail(group != NULL && CONTING_IS_GROUP(group), FALSE);

	priv = CONTING_GROUP_GET_PRIVATE(group);

	return g_slist_find(priv->drawings, drawing) != NULL;
}
GSList *
conting_group_get_children(ContingGroup *group)
{
	ContingGroupPrivate *priv;

	g_return_val_if_fail(group != NULL && CONTING_IS_GROUP(group), FALSE);

	priv = CONTING_GROUP_GET_PRIVATE(group);

	return priv->drawings;
}
