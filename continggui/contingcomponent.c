#include "contingcomponent.h"
#include <assert.h>

static gpointer parent_class = NULL;

#define DEFAULT_SIZE 60
#define DEFAULT_WIDTH 10

#define CONTING_COMPONENT_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE((o), \
            CONTING_TYPE_COMPONENT, ContingComponentPrivate))

typedef struct ContingComponentPrivate_ ContingComponentPrivate;
struct ContingComponentPrivate_ {
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

static void conting_component_get_rectangle(ContingDrawing *self,
		GdkRectangle *rect);
static void conting_component_draw(ContingDrawing *self, GdkDrawable *draw,
        GdkGC *gc, const GdkPoint *p, const GdkPoint *current) {
	g_print("conting_component_draw()\n");
	ContingComponentPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	if (!priv->placed)
		p = current;

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

static void conting_component_draw_selected(ContingDrawing *self,
		GdkDrawable *draw, GdkGC *gc, const GdkPoint *p) {
	ContingComponentPrivate *priv;
	GdkRectangle rect;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	conting_component_get_rectangle(self, &rect);

	rect.x += p->x;
	rect.y += p->y;

	gdk_draw_rectangle(draw, gc, TRUE,
			rect.x - 3, rect.y + rect.height / 2 - 3,
			6, 6);
	gdk_draw_rectangle(draw, gc, TRUE,
			rect.x + rect.width / 2 - 3, rect.y - 3,
			6, 6);
	gdk_draw_rectangle(draw, gc, TRUE,
			rect.x + rect.width - 3, rect.y + rect.height / 2 - 3,
			6, 6);
	gdk_draw_rectangle(draw, gc, TRUE,
			rect.x + rect.width / 2 - 3, rect.y + rect.height - 3,
			6, 6);
	
	return;
	switch (priv->orientation) {
		case GTK_ORIENTATION_HORIZONTAL:
			gdk_draw_rectangle(draw, gc, FALSE,
					p->x - (priv->size + 10) / 2,
					p->y - (DEFAULT_WIDTH + 10) / 2,
					priv->size + 10, DEFAULT_WIDTH + 10);
			break;
		case GTK_ORIENTATION_VERTICAL:
		default:
			gdk_draw_rectangle(draw, gc, TRUE,
					p->x - (DEFAULT_WIDTH + 10) / 2,
					p->y - (priv->size + 10) / 2,
					DEFAULT_WIDTH + 10, priv->size + 10);
			break;
	}
}

static void conting_component_get_rectangle(ContingDrawing *self,
		GdkRectangle *rect) {
	ContingComponentPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));
	g_return_if_fail(rect != NULL);

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	switch (priv->orientation) {
		case GTK_ORIENTATION_HORIZONTAL:
			rect->x = - priv->size / 2;
			rect->y = - DEFAULT_WIDTH / 2;
			rect->width = priv->size;
			rect->height = DEFAULT_WIDTH;
			break;
		case GTK_ORIENTATION_VERTICAL:
		default:
			rect->x = - DEFAULT_WIDTH / 2;
			rect->y = - priv->size / 2;
			rect->width = DEFAULT_WIDTH;
			rect->height = priv->size;
			break;
	}
}
/*
static gboolean conting_component_violates(ContingDrawing *self,
		ContingDrawing *other,
		gint x, gint y) {
	GdkRectangle rect, other_rect, dest_rect;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);
	
	conting_component_get_rectangle(self, &rect);

	if (other == NULL) {
		return x > rect.x && y > rect.y
			&& x - rect.x < rect.width && y - rect.y < rect.height;
	}

	conting_component_get_rectangle(other, &other_rect);
	other_rect.x += x;
	other_rect.y += y;

	return gdk_rectangle_intersect(&rect, &other_rect, &dest_rect);
}

static gboolean conting_component_can_link(ContingDrawing *self,
		ContingDrawing *link_drawing, gint x, gint y) {

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

	return conting_component_violates(self, link_drawing, x, y)
		&& CONTING_IS_DRAWING_LINE(link_drawing);
}
*/
static void conting_component_start_place(ContingDrawing *self) {
	ContingComponentPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	assert(priv->placed == FALSE);
}

static void conting_component_place(ContingDrawing *self,
		gint x, gint y, ContingDrawing *other) {
	ContingComponentPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	priv->placed = TRUE;
}

static gboolean conting_component_is_placed(ContingDrawing *self) {
	ContingComponentPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	return priv->placed;
}

static gboolean conting_component_answer(ContingDrawing *self,
		gint x, gint y) {
	ContingComponentPrivate *priv;
	GdkRectangle rect;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	conting_component_get_rectangle(self, &rect);

	return rect.x <= x && rect.y <= y
		&& x - rect.x < rect.width && y - rect.y < rect.height;
}

gboolean conting_component_link_line_coord(ContingComponent *self,
		ContingConnection *line, gint x, gint y) {
	ContingComponentPrivate *priv;
	GtkSideType side;
	gint position;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

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

	return conting_component_link_line(self, line, position, side);
}

gboolean conting_component_link_line(ContingComponent *self,
		ContingConnection *line, gint position, GtkSideType side) {
	ContingComponentPrivate *priv;
	BranchPosition *bp;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

	bp = g_new(BranchPosition, 1);
	bp->position = position;
	bp->side = side;

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	g_hash_table_insert(priv->branch_position, line, bp);

	return TRUE;
}

/*
static gboolean conting_component_place_linked(ContingDrawing *self,
		gint selfx, gint selfy,
		ContingDrawing *other, gint otherx, gint othery) {
	ContingComponentPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);
	g_return_val_if_fail(other != NULL && CONTING_IS_DRAWING_LINE(other),
			FALSE);

	if (!conting_component_place(self)) {
		return FALSE;
	}

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	return conting_component_link_line_coord(CONTING_COMPONENT(self),
		CONTING_DRAWING_LINE(other), selfx, selfy);
}
*/
static gboolean branch_position_foreach(gpointer key, gpointer value,
		gpointer user_data) {
	return TRUE;
}

static void conting_component_dispose(GObject *self) {
	ContingComponentPrivate *priv;
	
	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	if (priv->disposed)
		return;

	g_hash_table_foreach_remove(priv->branch_position,
			branch_position_foreach, NULL);

	priv->disposed = TRUE;

	G_OBJECT_CLASS(parent_class)->dispose(self);
}

static void conting_component_finalize(GObject *self) {
	ContingComponentPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	g_hash_table_destroy(priv->branch_position);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void conting_component_class_init(gpointer g_class,
        gpointer class_data) {
	g_print("component_class_init()\n");
    ContingDrawingClass *drawing_class;
	GObjectClass *object_class;

	g_type_class_add_private(g_class, sizeof(ContingComponentPrivate));

    parent_class = g_type_class_peek_parent(g_class);
    
    drawing_class = CONTING_DRAWING_CLASS(g_class);
//	drawing_class->get_rectangle = conting_component_get_rectangle;
    drawing_class->draw = conting_component_draw;
    drawing_class->draw_selected = conting_component_draw_selected;
//    drawing_class->violates = conting_component_violates;
//	drawing_class->can_link = conting_component_can_link;
	drawing_class->place = conting_component_place;
//	drawing_class->place_linked = conting_component_place_linked;

	drawing_class->start_place = conting_component_start_place;
	drawing_class->is_placed = conting_component_is_placed;
	drawing_class->answer = conting_component_answer;

	object_class = G_OBJECT_CLASS(g_class);
	object_class->dispose = conting_component_dispose;
	object_class->finalize = conting_component_finalize;
}

static void conting_component_instance_init(GTypeInstance *self,
		gpointer g_class) {
	ContingComponentPrivate *priv;

	priv = CONTING_COMPONENT_GET_PRIVATE(self);

	priv->orientation = GTK_ORIENTATION_VERTICAL;
	priv->size = DEFAULT_SIZE;
	priv->branch_position = g_hash_table_new_full(
			g_direct_hash, g_direct_equal,
			g_object_unref, g_free);

	priv->placed = FALSE;

	priv->disposed = FALSE;

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

ContingDrawing *conting_component_new(void) {
	return CONTING_DRAWING(g_object_new(CONTING_TYPE_COMPONENT, NULL));
}
