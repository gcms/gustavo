#include "contingbus.h"
#include <assert.h>

static gpointer parent_class = NULL;

static const GdkRectangle DEFAULT_RECT = { -5, -30, 10, 60 };

#define CONTING_BUS_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE((o), \
            CONTING_TYPE_BUS, ContingBusPrivate))

typedef struct ContingBusPrivate_ ContingBusPrivate;
struct ContingBusPrivate_ {
	GdkColor color;

	GdkRectangle rect;

	GHashTable *branch_position;

	gboolean placed;
	gboolean disposed;
};

static void conting_bus_draw(ContingDrawing *self, GdkDrawable *draw,
        GdkGC *gc, const GdkPoint *p, const GdkPoint *current) {
	g_print("conting_bus_draw()\n");
	ContingBusPrivate *priv;
	GdkRectangle rect;

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

	if (!priv->placed)
		p = current;

	gdk_gc_set_rgb_fg_color(gc, &priv->color);

	conting_component_get_rectangle(CONTING_COMPONENT(self), &rect);

	gdk_draw_rectangle(draw, gc, TRUE,
			rect.x + p->x, rect.y + p->y,
			rect.width, rect.height);
}

static void conting_bus_get_rectangle(ContingComponent *self,
		GdkRectangle *rect) {
	ContingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));
	g_return_if_fail(rect != NULL);

	priv = CONTING_BUS_GET_PRIVATE(self);

	*rect = priv->rect;
}
/*
static gboolean conting_bus_violates(ContingDrawing *self,
		ContingDrawing *other,
		gint x, gint y) {
	GdkRectangle rect, other_rect, dest_rect;

	g_return_val_if_fail(self != NULL && CONTING_IS_BUS(self), FALSE);
	
	conting_bus_get_rectangle(self, &rect);

	if (other == NULL) {
		return x > rect.x && y > rect.y
			&& x - rect.x < rect.width && y - rect.y < rect.height;
	}

	conting_bus_get_rectangle(other, &other_rect);
	other_rect.x += x;
	other_rect.y += y;

	return gdk_rectangle_intersect(&rect, &other_rect, &dest_rect);
}

static gboolean conting_bus_can_link(ContingDrawing *self,
		ContingDrawing *link_drawing, gint x, gint y) {

	g_return_val_if_fail(self != NULL && CONTING_IS_BUS(self), FALSE);

	return conting_bus_violates(self, link_drawing, x, y)
		&& CONTING_IS_DRAWING_LINE(link_drawing);
}
*/
static void conting_bus_start_place(ContingDrawing *self) {
	ContingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

	assert(priv->placed == FALSE);
}

static void conting_bus_place(ContingDrawing *self,
		gint x, gint y, ContingDrawing *other, gint ox, gint oy) {
	ContingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

	priv->placed = TRUE;
}

static gboolean conting_bus_is_placed(ContingDrawing *self) {
	ContingBusPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_BUS(self), FALSE);

	priv = CONTING_BUS_GET_PRIVATE(self);

	return priv->placed;
}

static void branch_position_move_foreach(gpointer key, gpointer value,
		gpointer user_data) {
	ContingConnection *conn;
	gpointer *data;
	ContingDrawing *bus;
	GdkPoint *movement;

	g_return_if_fail(key != NULL && CONTING_IS_CONNECTION(key));
	g_return_if_fail(user_data != NULL);

	conn = key;
	data = user_data;

	movement = data[0];
	bus = data[1];

	g_return_if_fail(bus != NULL && CONTING_IS_BUS(bus));
	g_return_if_fail(movement != NULL);

	conting_connection_move(conn, bus, movement->x, movement->y);
	
}

static void conting_bus_move(ContingComponent *self, gint x, gint y) {	
	ContingBusPrivate *priv;
	GdkPoint movement;
	gpointer data[2];

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	g_print("conting_bus_move()\n");

	priv = CONTING_BUS_GET_PRIVATE(self);

	movement.x = x;
	movement.y = y;

	data[0] = &movement;
	data[1] = self;

	g_hash_table_foreach(priv->branch_position,
			branch_position_move_foreach, data);
}

static gboolean conting_bus_connect(ContingComponent *self,
		ContingConnection *conn, gint x, gint y, GdkPoint *shift) {
	ContingBusPrivate *priv;
	GdkPoint *new_point;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);
	g_return_val_if_fail(conn != NULL && CONTING_IS_CONNECTION(conn), FALSE);

	priv = CONTING_BUS_GET_PRIVATE(self);

	new_point = g_new(GdkPoint, 1);
	new_point->x = DEFAULT_RECT.x + (x < 0 ? 0 : DEFAULT_RECT.width);
	new_point->y = y;

	if (shift != NULL) {
		shift->x = new_point->x - x;
		shift->y = new_point->y - y;
	}

	g_object_ref(conn);

	g_hash_table_insert(priv->branch_position,
			conn, new_point);

	return TRUE;
}

/*
gboolean conting_bus_link_line_coord(ContingBus *self,
		ContingConnection *line, gint x, gint y) {
	ContingBusPrivate *priv;
	GtkSideType side;
	gint position;

	g_return_val_if_fail(self != NULL && CONTING_IS_BUS(self), FALSE);

	priv = CONTING_BUS_GET_PRIVATE(self);

	switch (priv->orientation) {
		case GTK_ORIENTATION_HORIZONTAL:
			side = y < DEFAULT_WIDTH / 2 ? GTK_SIDE_TOP : GTK_SIDE_BOTTOM;
			position = x;
			break;
		case GTK_ORIENTATION_VERTICAL:
		default:
			side = x < DEFAULT_WIDTH / 2 ? GTK_SIDE_LEFT : GTK_SIDE_RIGHT;
			position = y;
			break;
	}

	return conting_bus_link_line(self, line, position, side);
}
gboolean conting_bus_link_line(ContingBus *self,
		ContingConnection *line, gint position, GtkSideType side) {
	ContingBusPrivate *priv;
	BranchPosition *bp;

	g_return_val_if_fail(self != NULL && CONTING_IS_BUS(self), FALSE);

	bp = g_new(BranchPosition, 1);
	bp->position = position;
	bp->side = side;

	priv = CONTING_BUS_GET_PRIVATE(self);

	g_hash_table_insert(priv->branch_position, line, bp);

	return TRUE;
}
static gboolean conting_bus_place_linked(ContingDrawing *self,
		gint selfx, gint selfy,
		ContingDrawing *other, gint otherx, gint othery) {
	ContingBusPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_BUS(self), FALSE);
	g_return_val_if_fail(other != NULL && CONTING_IS_DRAWING_LINE(other),
			FALSE);

	if (!conting_bus_place(self)) {
		return FALSE;
	}

	priv = CONTING_BUS_GET_PRIVATE(self);

	return conting_bus_link_line_coord(CONTING_BUS(self),
		CONTING_DRAWING_LINE(other), selfx, selfy);
}
*/
static gboolean branch_position_dispose_foreach(gpointer key, gpointer value,
		gpointer user_data) {
	return TRUE;
}

static void conting_bus_dispose(GObject *self) {
	ContingBusPrivate *priv;
	
	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

	if (priv->disposed)
		return;

	g_hash_table_foreach_remove(priv->branch_position,
			branch_position_dispose_foreach, NULL);

	priv->disposed = TRUE;

	G_OBJECT_CLASS(parent_class)->dispose(self);
}

static void conting_bus_finalize(GObject *self) {
	ContingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

	g_hash_table_destroy(priv->branch_position);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void conting_bus_class_init(gpointer g_class,
        gpointer class_data) {
    ContingDrawingClass *drawing_class;
	ContingComponentClass *component_class;
	GObjectClass *object_class;

	g_return_if_fail(g_class != NULL && CONTING_IS_BUS_CLASS(g_class));

	g_type_class_add_private(g_class, sizeof(ContingBusPrivate));

    parent_class = g_type_class_peek_parent(g_class);

	component_class = CONTING_COMPONENT_CLASS(g_class);
	component_class->get_rectangle = conting_bus_get_rectangle;
	component_class->move = conting_bus_move;
	component_class->connect = conting_bus_connect;
    
    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_bus_draw;
	drawing_class->start_place = conting_bus_start_place;
	drawing_class->place = conting_bus_place;
	drawing_class->is_placed = conting_bus_is_placed;

	object_class = G_OBJECT_CLASS(g_class);
	object_class->dispose = conting_bus_dispose;
	object_class->finalize = conting_bus_finalize;
}

static void conting_bus_instance_init(GTypeInstance *self,
		gpointer g_class) {
	ContingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

	gdk_color_parse("black", &priv->color);

	priv->rect = DEFAULT_RECT;

	priv->branch_position = g_hash_table_new_full(
			g_direct_hash, g_direct_equal,
			g_object_unref, g_free);

	priv->placed = FALSE;

	priv->disposed = FALSE;

}

GType conting_bus_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingBusClass),
            NULL,
            NULL,
            conting_bus_class_init,
            NULL,
            NULL,
            sizeof(ContingBus),
            0,
            conting_bus_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_COMPONENT,
				"ContingBus",
                &type_info, 0);
    }

    return type;
}

ContingDrawing *conting_bus_new(void) {
	return CONTING_DRAWING(g_object_new(CONTING_TYPE_BUS, NULL));
}
