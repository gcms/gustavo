#include "contingconnection.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>

static gpointer parent_class = NULL;

static GdkColor black_color;

#define CONTING_CONNECTION_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
		CONTING_TYPE_CONNECTION, ContingConnectionPrivate))

typedef struct ContingConnectionPrivate_ ContingConnectionPrivate;
struct ContingConnectionPrivate_ {
	GdkColor color;

	GSList *points;
	ContingComponent *start;
	ContingComponent *end;

	gboolean placed;

	gboolean disposed;
};

static void conting_connection_draw_selected(ContingDrawing *self,
		GdkDrawable *drawable, GdkGC *gc,
		const GdkPoint *pos) {
	ContingConnectionPrivate *priv;
	GSList *n;

	g_return_if_fail(self != NULL && CONTING_IS_CONNECTION(self));

	priv = CONTING_CONNECTION_GET_PRIVATE(self);

	assert(priv->placed == TRUE);

	gdk_gc_set_rgb_fg_color(gc, &black_color);

	for (n = g_slist_next(priv->points); g_slist_next(n) != NULL;
			n = g_slist_next(n)) {
		GdkPoint *cur_point = n->data;
		gdk_draw_rectangle(drawable, gc, TRUE,
				pos->x + cur_point->x - 3, pos->y + cur_point->y - 3,
				6, 6);
	}
}

static void conting_connection_draw(ContingDrawing *self,
		GdkDrawable *drawable, GdkGC *gc,
		const GdkPoint *pos, const GdkPoint *mouse_pos) {
	ContingConnectionPrivate *priv;
	GSList *n;
	GdkPoint *last_point = NULL;

	g_return_if_fail(self != NULL && CONTING_IS_CONNECTION(self));

	priv = CONTING_CONNECTION_GET_PRIVATE(self);

	gdk_gc_set_rgb_fg_color(gc, &priv->color);
	for (n = priv->points; n != NULL; n = g_slist_next(n)) {
		GdkPoint *cur_point = n->data;
		if (last_point) {
			gdk_draw_line(drawable, gc,
					pos->x + last_point->x, pos->y + last_point->y,
					pos->x + cur_point->x, pos->y + cur_point->y);
		}
		last_point = cur_point;
	}

	if (!priv->placed && last_point && mouse_pos) {
		gdk_draw_line(drawable, gc,
				pos->x + last_point->x, pos->y + last_point->y,
				pos->x + mouse_pos->x, pos->y + mouse_pos->y);
	}
}


/*
static void conting_connection_get_rectangle(ContingDrawing *self,
        GdkRectangle *rect) {
	ContingConnectionPrivate *priv;
	GSList *n;
	gint minx = 0, miny = 0, maxx = 0, maxy = 0;

	g_return_if_fail(self != NULL && CONTING_IS_CONNECTION(self));
	
	priv = CONTING_CONNECTION_GET_PRIVATE(self);
	
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
*/
static void conting_connection_start_place(ContingDrawing *self) {
	ContingConnectionPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_CONNECTION(self));

	priv = CONTING_CONNECTION_GET_PRIVATE(self);

	assert(priv->placed == FALSE);
}

static void conting_connection_place(ContingDrawing *self,
		gint x, gint y, ContingDrawing *other, gint ox, gint oy) {
	ContingConnectionPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_CONNECTION(self));

	priv = CONTING_CONNECTION_GET_PRIVATE(self);

	assert(priv->placed == FALSE);

	if (priv->points == NULL) {
		if (other != NULL && CONTING_IS_COMPONENT(other)) {
			GdkPoint shift;
			GdkPoint *new_point = g_new(GdkPoint, 1);

			new_point->x = x;
			new_point->y = y;

			priv->points = g_slist_append(priv->points, new_point);

			
			if (conting_component_connect(CONTING_COMPONENT(other),
					CONTING_CONNECTION(self), ox, oy, &shift)) {
				g_object_ref(other);
				priv->start = CONTING_COMPONENT(other);

				new_point->x += shift.x;
				new_point->y += shift.y;
			}
		}
	} else {
		GdkPoint *new_point = g_new(GdkPoint, 1);
		new_point->x = x;
		new_point->y = y;

		priv->points = g_slist_append(priv->points, new_point);

		if (other != NULL && CONTING_IS_COMPONENT(other)) {
			GdkPoint shift;

			if (conting_component_connect(CONTING_COMPONENT(other),
					CONTING_CONNECTION(self), ox, oy, &shift)) {
				g_object_ref(other);
				priv->end = CONTING_COMPONENT(other);
				priv->placed = TRUE;

				new_point->x += shift.x;
				new_point->y += shift.y;
			}
		}
	}

	g_print("conting_connection_place()\n");
}

static gboolean conting_connection_is_placed(ContingDrawing *self) {
	ContingConnectionPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_CONNECTION(self), FALSE);

	priv = CONTING_CONNECTION_GET_PRIVATE(self);

	return priv->placed;
}

static gboolean conting_connection_answer(ContingDrawing *self,
		gint x, gint y) {
	ContingConnectionPrivate *priv;
	GSList *n;
	GdkPoint *prev_point;

	g_return_val_if_fail(self != NULL && CONTING_IS_CONNECTION(self), FALSE);
	
	priv = CONTING_CONNECTION_GET_PRIVATE(self);
	
	assert(priv->placed == TRUE);

	prev_point = NULL;
	for (n = priv->points; n != NULL; n = g_slist_next(n)) {
		GdkPoint *cur_point = n->data;
		if (prev_point != NULL) {
			gint dx, dy;
			gdouble m, d;
			gint ix, iy;

			dx = cur_point->x - prev_point->x;
			dy = cur_point->y - prev_point->y;

			ix = x - prev_point->x;
			iy = prev_point->y - y;

			m = (gdouble) dy / (gdouble) dx;

			d = fabs(m * ix + iy) / sqrt(m * m + 1);

			if (d < 5 && abs(ix) < abs(dx) && abs(iy) < abs(dy)) {
				return TRUE;
			}
		}
		prev_point = cur_point;
	}

	return FALSE;
}

static void conting_connection_move_impl(ContingConnection *self,
		ContingComponent *comp, gint x, gint y) {
	ContingConnectionPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_CONNECTION(self));

	priv = CONTING_CONNECTION_GET_PRIVATE(self);

	assert(priv->placed == TRUE);

	if (comp == priv->start) {
		g_print("FIRST\n");
		GdkPoint *first = priv->points->data;
		first->x += x;
		first->y += y;
	} else if (comp == priv->end) {
		g_print("LAST\n");
		GdkPoint *last = g_slist_last(priv->points)->data;
		last->x += x;
		last->y += y;
	}
}

static void conting_connection_dispose(GObject *self) {
	ContingConnectionPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_CONNECTION(self));

	priv = CONTING_CONNECTION_GET_PRIVATE(self);

	if (priv->disposed)
		return;
	
	if (priv->start) {
		g_object_unref(priv->start);
	}
	if (priv->end) {
		g_object_unref(priv->end);
	}

	priv->disposed = TRUE;

	G_OBJECT_CLASS(parent_class)->dispose(self);
}

static void conting_connection_finalize(GObject *self) {
	ContingConnectionPrivate *priv;
	GSList *n;

	g_return_if_fail(self != NULL && CONTING_IS_CONNECTION(self));

	priv = CONTING_CONNECTION_GET_PRIVATE(self);

	for (n = priv->points; n != NULL; n = g_slist_next(n)) {
		g_free(n->data);
	}

	g_slist_free(priv->points);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void conting_connection_class_init(gpointer g_class,
        gpointer class_data) {
	ContingConnectionClass *connection_class;
    ContingDrawingClass *drawing_class;
	GObjectClass *object_class;

	g_type_class_add_private(g_class, sizeof(ContingConnectionPrivate));

	parent_class = g_type_class_peek_parent(g_class);

	connection_class = CONTING_CONNECTION_CLASS(g_class);
	connection_class->move = conting_connection_move_impl;
	
    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_connection_draw;
	drawing_class->draw_selected = conting_connection_draw_selected;
	drawing_class->start_place = conting_connection_start_place;
	drawing_class->place = conting_connection_place;
	drawing_class->is_placed = conting_connection_is_placed;
	drawing_class->answer = conting_connection_answer;

	object_class = G_OBJECT_CLASS(g_class);
	object_class->finalize = conting_connection_finalize;
	object_class->dispose = conting_connection_dispose;

	gdk_color_parse("black", &black_color);
}

static void conting_connection_instance_init(GTypeInstance *obj,
        gpointer g_class) {
	ContingConnectionPrivate *priv;

	g_return_if_fail(obj != NULL && CONTING_IS_CONNECTION(obj));

	priv = CONTING_CONNECTION_GET_PRIVATE(obj);

	gdk_color_parse("red", &priv->color);

	priv->start = NULL;
	priv->end = NULL;

	priv->points = NULL;
	priv->placed = FALSE;

	priv->disposed = FALSE;
}

GType conting_connection_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingConnectionClass),
            NULL,
            NULL,
            conting_connection_class_init,
            NULL,
            NULL,
            sizeof(ContingConnection),
            0,
            conting_connection_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_DRAWING,
				"ContingConnection",
                &type_info, 0);
    }

    return type;
}

ContingDrawing *conting_connection_new(void) {
	g_print("conting_connection_new()\n");
    return CONTING_DRAWING(g_object_new(CONTING_TYPE_CONNECTION, NULL));
}

void conting_connection_move(ContingConnection *self, ContingComponent *comp,
		gint x, gint y) {
	g_return_if_fail(self != NULL && CONTING_IS_CONNECTION(self));
	g_return_if_fail(comp != NULL && CONTING_IS_COMPONENT(comp));

	CONTING_CONNECTION_GET_CLASS(self)->move(self, comp, x, y);
}
