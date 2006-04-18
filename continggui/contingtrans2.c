#include "contingtrans2.h"
#include <assert.h>

static gpointer parent_class = NULL;

static GdkRectangle DEFAULT_RECT = { -10, -10, 30, 20 };

typedef struct ContingTrans2Private_ ContingTrans2Private;
struct ContingTrans2Private_ {
	GdkColor color;

	GdkRectangle rect;
	ContingConnection *input;
	ContingConnection *output;

	gboolean placed;
	gboolean disposed;
};

#define CONTING_TRANS2_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE((o), \
            CONTING_TYPE_TRANS2, ContingTrans2Private))

static void conting_trans2_get_rectangle(ContingComponent *self,
		GdkRectangle *rect) {
	ContingTrans2Private *priv;

	g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));
	g_return_if_fail(rect != NULL);

	priv = CONTING_TRANS2_GET_PRIVATE(self);

	*rect = priv->rect;
}

static void conting_trans2_start_place(ContingDrawing *self) {
	ContingTrans2Private *priv;

	g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

	priv = CONTING_TRANS2_GET_PRIVATE(self);

	assert(priv->placed == FALSE);
}

static void conting_trans2_place(ContingDrawing  *self, gint x, gint y,
		ContingDrawing *other, gint ox, gint oy) {
	ContingTrans2Private *priv;

	g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

	priv = CONTING_TRANS2_GET_PRIVATE(self);

	priv->placed = TRUE;
}

static gboolean conting_trans2_is_placed(ContingDrawing *self) {
	ContingTrans2Private *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_TRANS2(self), FALSE);

	priv = CONTING_TRANS2_GET_PRIVATE(self);

	return priv->placed;
}

static void conting_trans2_move(ContingComponent *self, gint x, gint y) {
	ContingTrans2Private *priv;

	g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

	priv = CONTING_TRANS2_GET_PRIVATE(self);;

	if (priv->input != NULL) {
		conting_connection_move(priv->input, self, x, y);
	}

	if (priv->output != NULL) {
		conting_connection_move(priv->output, self, x, y);
	}
}

static gboolean conting_trans2_connect(ContingComponent *self,
		ContingConnection *conn, gint x, gint y, GdkPoint *shift) {
	ContingTrans2Private *priv;
	GdkRectangle rect;

	g_return_val_if_fail(self != NULL && CONTING_IS_TRANS2(self), FALSE);
	g_return_val_if_fail(conn != NULL && CONTING_IS_CONNECTION(conn), FALSE);

	priv = CONTING_TRANS2_GET_PRIVATE(self);

	conting_component_get_rectangle(CONTING_COMPONENT(self), &rect);

	gdouble h_tick = (gdouble) rect.width / 6.0;
	gdouble v_tick = (gdouble) rect.height / 5.0;

	if (y < 0 && priv->input == NULL) {
		priv->input = conn;
		g_object_ref(conn);

		if (shift != NULL) {
			shift->x = rect.x + (gint) (3 * h_tick) - x;
			shift->y = rect.y - y;
		}

		return TRUE;
	} else if (priv->output == NULL) {
		priv->output = conn;
		g_object_ref(conn);

		if (shift != NULL) {
			shift->x = rect.x + (gint) (3 * h_tick) - x;
			shift->y = rect.y + (gint) (5 * v_tick) - y;
		}

		return TRUE;
	}

	return FALSE;
}
static gboolean conting_trans2_disconnect(ContingComponent *self,
		ContingConnection *conn) {
	ContingTrans2Private *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_TRANS2(self), FALSE);

	priv = CONTING_TRANS2_GET_PRIVATE(self);

	if (conn == priv->input) {
		g_object_unref(conn);
		priv->input = NULL;
		
		return TRUE;
	} else if (conn == priv->output) {
		g_object_unref(conn);
		priv->output = NULL;

		return TRUE;
	}

	return FALSE;
}

static void conting_trans2_draw(ContingDrawing *self, GdkDrawable *draw,
		GdkGC *gc, const GdkPoint *base_point, const GdkPoint *cur_point) {
	g_print("conting_trans2_draw()\n");
	ContingTrans2Private *priv;
	GdkRectangle rect;

	g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

	priv = CONTING_TRANS2_GET_PRIVATE(self);

	if (!priv->placed) {
		base_point = cur_point;
	}

	conting_component_get_rectangle(CONTING_COMPONENT(self), &rect);

	rect.x += base_point->x;
	rect.y += base_point->y;

	gdouble h_tick = (gdouble) rect.width / 6.0;
	gdouble v_tick = (gdouble) rect.height / 5.0;

	gdk_gc_set_rgb_fg_color(gc, &priv->color);

	gdk_draw_line(draw, gc,
			rect.x, rect.y + (gint) (2 * v_tick),
			rect.x + (gint) h_tick, rect.y);
	gdk_draw_line(draw, gc,
			rect.x + (gint) h_tick, rect.y,
			rect.x + (gint) (2 * h_tick), rect.y + (gint) (2 * v_tick));
	gdk_draw_line(draw, gc,
			rect.x + (gint) (2 * h_tick), rect.y + (gint) (2 * v_tick),
			rect.x + (gint) (3 * h_tick), rect.y);
	gdk_draw_line(draw, gc,
			rect.x + (gint) (3 * h_tick), rect.y,
			rect.x + (gint) (4 * h_tick), rect.y + (gint) (2 * v_tick));
	gdk_draw_line(draw, gc,
			rect.x + (gint) (4 * h_tick), rect.y + (gint) (2 * v_tick),
			rect.x + (gint) (5 * h_tick), rect.y);
	gdk_draw_line(draw, gc,
			rect.x + (gint) (5 * h_tick), rect.y,
			rect.x + (gint) (6 * h_tick), rect.y + (gint) (2 * v_tick));

	gdk_draw_line(draw, gc,
			rect.x, rect.y + (gint) (3 * v_tick),
			rect.x + (gint) h_tick, rect.y + (gint) (5 * v_tick));
	gdk_draw_line(draw, gc,
			rect.x + (gint) h_tick, rect.y + (gint) (5 * v_tick),
			rect.x + (gint) (2 * h_tick), rect.y + (3 * v_tick));
	gdk_draw_line(draw, gc,
			rect.x + (gint) (2 * h_tick), rect.y + (3 * v_tick),
			rect.x + (gint) (3 * h_tick), rect.y + (5 * v_tick));
	gdk_draw_line(draw, gc,
			rect.x + (gint) (3 * h_tick), rect.y + (5 * v_tick),
			rect.x + (gint) (4 * h_tick), rect.y + (3 * v_tick));
	gdk_draw_line(draw, gc,
			rect.x + (gint) (4 * h_tick), rect.y + (3 * v_tick),
			rect.x + (gint) (5 * h_tick), rect.y + (5 * v_tick));
	gdk_draw_line(draw, gc,
			rect.x + (gint) (5 * h_tick), rect.y + (5 * v_tick),
			rect.x + (gint) (6 * h_tick), rect.y + (3 * v_tick));
}

static void conting_trans2_class_init(gpointer g_class, gpointer class_data) {
	ContingDrawingClass *drawing_class;
	ContingComponentClass *component_class;
	GObjectClass *object_class;

	g_return_if_fail(g_class != NULL && CONTING_IS_TRANS2_CLASS(g_class));

	g_type_class_add_private(g_class, sizeof(ContingTrans2Private));

	parent_class = g_type_class_peek_parent(g_class);

	drawing_class = CONTING_DRAWING_CLASS(g_class);
	drawing_class->draw = conting_trans2_draw;
	drawing_class->start_place = conting_trans2_start_place;
	drawing_class->place = conting_trans2_place;
	drawing_class->is_placed = conting_trans2_is_placed;

	component_class = CONTING_COMPONENT_CLASS(g_class);
	component_class->get_rectangle = conting_trans2_get_rectangle;
	component_class->move = conting_trans2_move;
	component_class->connect = conting_trans2_connect;
	component_class->disconnect = conting_trans2_disconnect;

	object_class = G_OBJECT_CLASS(g_class);
}

static void conting_trans2_instance_init(GTypeInstance *self,
		gpointer g_class) {
	ContingTrans2Private *priv;

	g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

	priv = CONTING_TRANS2_GET_PRIVATE(self);

	gdk_color_parse("black", &priv->color);

	priv->rect = DEFAULT_RECT;

	priv->input = NULL;
	priv->output = NULL;

	priv->placed = FALSE;

	priv->disposed = FALSE;
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

ContingDrawing *conting_trans2_new(void) {
	return CONTING_DRAWING(g_object_new(CONTING_TYPE_TRANS2, NULL));
}
