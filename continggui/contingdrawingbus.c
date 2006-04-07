#include "contingdrawingbus.h"

static gpointer parent_class = NULL;

#define DEFAULT_SIZE 60
#define DEFAULT_WIDTH 20

#define CONTING_DRAWING_BUS_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE((o), \
            CONTING_TYPE_DRAWING_BUS, ContingDrawingBusPrivate))

typedef struct ContingDrawingBusPrivate_ ContingDrawingBusPrivate;
struct ContingDrawingBusPrivate_ {
    GtkOrientation orientation;
    gint size;
	GHashTable *branch_position;

	gboolean placed;
	gboolean disposed;
};

typedef struct BranchPosition_ BranchPosition;
struct BranchPosition_ {
	gint position;
	GtkSideType side;
};

static void conting_drawing_bus_draw(ContingDrawing *self, GdkDrawable *draw,
        GdkGC *gc, const GdkPoint *p) {
	g_print("conting_drawing_bus_draw()\n");
	ContingDrawingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_BUS(self));

	priv = CONTING_DRAWING_BUS_GET_PRIVATE(self);

	switch (priv->orientation) {
		case GTK_ORIENTATION_HORIZONTAL:
			gdk_draw_rectangle(draw, gc, TRUE,
					p->x - priv->size / 2, p->y - DEFAULT_WIDTH / 2,
					priv->size, DEFAULT_WIDTH);
			break;
		case GTK_ORIENTATION_VERTICAL:
		default:
			gdk_draw_rectangle(draw, gc, TRUE,
					p->x - DEFAULT_WIDTH / 2, p->y - priv->size / 2,
					DEFAULT_WIDTH, priv->size);
			break;
	}
}

static void conting_drawing_bus_draw_selected(ContingDrawing *self,
		GdkDrawable *draw, GdkGC *gc, const GdkPoint *p) {
	ContingDrawingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_BUS(self));

	priv = CONTING_DRAWING_BUS_GET_PRIVATE(self);
	
	switch (priv->orientation) {
		case GTK_ORIENTATION_HORIZONTAL:
			gdk_draw_rectangle(draw, gc, FALSE,
					p->x - (priv->size + 5) / 2, p->y - (DEFAULT_WIDTH + 5) / 2,
					priv->size + 5, DEFAULT_WIDTH + 5);
			break;
		case GTK_ORIENTATION_VERTICAL:
		default:
			gdk_draw_rectangle(draw, gc, TRUE,
					p->x - (DEFAULT_WIDTH + 5) / 2, p->y - (priv->size + 5) / 2,
					DEFAULT_WIDTH + 5, priv->size + 5);
			break;
	}
}

static void conting_drawing_bus_get_rectangle(ContingDrawing *self,
		GdkRectangle *rect) {
	ContingDrawingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_BUS(self));
	g_return_if_fail(rect != NULL);

	priv = CONTING_DRAWING_BUS_GET_PRIVATE(self);

	switch (priv->orientation) {
		case GTK_ORIENTATION_HORIZONTAL:
			rect->x = priv->size / 2;
			rect->y = DEFAULT_WIDTH / 2;
			rect->width = priv->size;
			rect->height = DEFAULT_WIDTH;
			break;
		case GTK_ORIENTATION_VERTICAL:
		default:
			rect->x = DEFAULT_WIDTH / 2;
			rect->y = priv->size / 2;
			rect->width = DEFAULT_WIDTH;
			rect->height = priv->size;
			break;
	}
}

static gboolean conting_drawing_bus_violates(ContingDrawing *self,
		gint x, gint y) {
	GdkRectangle rect;

	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING_BUS(self), FALSE);

	conting_drawing_bus_get_rectangle(self, &rect);

	return x > rect.x && y > rect.y
		&& x - rect.x < rect.width && y - rect.y < rect.height;
}

static gboolean conting_drawing_bus_can_link(ContingDrawing *self,
		ContingDrawing *link_drawing, gint x, gint y) {

	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING_BUS(self), FALSE);

	return conting_drawing_bus_violates(self, x, y)
		&& CONTING_IS_DRAWING_LINE(link_drawing);
}

static gboolean conting_drawing_bus_place(ContingDrawing *self) {
	ContingDrawingBusPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING_BUS(self), FALSE);

	priv = CONTING_DRAWING_BUS_GET_PRIVATE(self);

	g_return_val_if_fail(!priv->placed, FALSE);

	priv->placed = TRUE;

	return TRUE;
}

gboolean conting_drawing_bus_link_line_coord(ContingDrawingBus *self,
		ContingDrawingLine *line, gint x, gint y) {
	ContingDrawingBusPrivate *priv;
	GtkSideType side;
	gint position;

	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING_BUS(self), FALSE);

	priv = CONTING_DRAWING_BUS_GET_PRIVATE(self);

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

	return conting_drawing_bus_link_line(self, line, position, side);
}

gboolean conting_drawing_bus_link_line(ContingDrawingBus *self,
		ContingDrawingLine *line, gint position, GtkSideType side) {
	ContingDrawingBusPrivate *priv;
	BranchPosition *bp;

	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING_BUS(self), FALSE);

	bp = g_new(BranchPosition, 1);
	bp->position = position;
	bp->side = side;

	priv = CONTING_DRAWING_BUS_GET_PRIVATE(self);

	g_hash_table_insert(priv->branch_position, line, bp);

	return TRUE;
}

static gboolean conting_drawing_bus_place_linked(ContingDrawing *self,
		gint selfx, gint selfy,
		ContingDrawing *other, gint otherx, gint othery) {
	ContingDrawingBusPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING_BUS(self), FALSE);
	g_return_val_if_fail(other != NULL && CONTING_IS_DRAWING_LINE(other),
			FALSE);

	if (!conting_drawing_bus_place(self)) {
		return FALSE;
	}

	priv = CONTING_DRAWING_BUS_GET_PRIVATE(self);

	return conting_drawing_bus_link_line_coord(CONTING_DRAWING_BUS(self),
		CONTING_DRAWING_LINE(other), selfx, selfy);
}

static gboolean branch_position_foreach(gpointer key, gpointer value,
		gpointer user_data) {
	return TRUE;
}

static void conting_drawing_bus_dispose(GObject *self) {
	ContingDrawingBusPrivate *priv;
	
	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_BUS(self));

	priv = CONTING_DRAWING_BUS_GET_PRIVATE(self);

	g_hash_table_foreach_remove(priv->branch_position,
			branch_position_foreach, NULL);

	priv->disposed = TRUE;

	G_OBJECT_CLASS(parent_class)->dispose(self);
}

static void conting_drawing_bus_finalize(GObject *self) {
	ContingDrawingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_BUS(self));

	priv = CONTING_DRAWING_BUS_GET_PRIVATE(self);

	g_hash_table_destroy(priv->branch_position);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void conting_drawing_bus_class_init(gpointer g_class,
        gpointer class_data) {
	g_print("drawing_bus_class_init()\n");
    ContingDrawingClass *drawing_class;
	GObjectClass *object_class;

	g_type_class_add_private(g_class, sizeof(ContingDrawingBusPrivate));

    parent_class = g_type_class_peek_parent(g_class);
    
    drawing_class = CONTING_DRAWING_CLASS(g_class);
	drawing_class->get_rectangle = conting_drawing_bus_get_rectangle;
    drawing_class->draw = conting_drawing_bus_draw;
    drawing_class->draw_selected = conting_drawing_bus_draw_selected;
    drawing_class->violates = conting_drawing_bus_violates;
	drawing_class->can_link = conting_drawing_bus_can_link;
	drawing_class->place = conting_drawing_bus_place;
	drawing_class->place_linked = conting_drawing_bus_place_linked;

	object_class = G_OBJECT_CLASS(g_class);
	object_class->dispose = conting_drawing_bus_dispose;
	object_class->finalize = conting_drawing_bus_finalize;
}

static void conting_drawing_bus_instance_init(GTypeInstance *self,
		gpointer g_class) {
	ContingDrawingBusPrivate *priv;

	priv = CONTING_DRAWING_BUS_GET_PRIVATE(self);

	priv->orientation = GTK_ORIENTATION_VERTICAL;
	priv->size = DEFAULT_SIZE;
	priv->branch_position = g_hash_table_new_full(
			g_direct_hash, g_direct_equal,
			g_object_unref, g_free);

	priv->placed = FALSE;

	priv->disposed = FALSE;

}

GType conting_drawing_bus_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingDrawingBusClass),
            NULL,
            NULL,
            conting_drawing_bus_class_init,
            NULL,
            NULL,
            sizeof(ContingDrawingBus),
            0,
            conting_drawing_bus_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_DRAWING,
				"ContingDrawingBus",
                &type_info, 0);
    }

    return type;
}

ContingDrawing *conting_drawing_bus_new(void) {
	return CONTING_DRAWING(g_object_new(CONTING_TYPE_DRAWING_BUS, NULL));
}
