#include "contingdrawingline.h"
#include <assert.h>

static gpointer parent_class = NULL;

#define CONTING_DRAWING_LINE_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
		CONTING_TYPE_DRAWING_LINE, ContingDrawingLinePrivate))

typedef struct ContingDrawingLinePrivate_ ContingDrawingLinePrivate;
struct ContingDrawingLinePrivate_ {
	GSList *points;
	ContingDrawing *start;
	ContingDrawing *end;

	gboolean placed;

	gboolean disposed;
};

static void conting_drawing_line_draw(ContingDrawing *self,
		GdkDrawable *drawable, GdkGC *gc,
		const GdkPoint *pos, const GdkPoint *mouse_pos) {
	ContingDrawingLinePrivate *priv;
	GSList *n;
	GdkPoint *last_point = NULL;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_LINE(self));

	priv = CONTING_DRAWING_LINE_GET_PRIVATE(self);

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
				mouse_pos->x, mouse_pos->y);
	}
}


/*
static void conting_drawing_line_get_rectangle(ContingDrawing *self,
        GdkRectangle *rect) {
	ContingDrawingLinePrivate *priv;
	GSList *n;
	gint minx = 0, miny = 0, maxx = 0, maxy = 0;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_LINE(self));
	
	priv = CONTING_DRAWING_LINE_GET_PRIVATE(self);
	
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
static void conting_drawing_line_start_place(ContingDrawing *self) {
	ContingDrawingLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_LINE(self));

	priv = CONTING_DRAWING_LINE_GET_PRIVATE(self);

	assert(priv->placed == FALSE);
}

static void conting_drawing_line_place(ContingDrawing *self,
		gint x, gint y, ContingDrawing *other) {
	ContingDrawingLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_LINE(self));

	priv = CONTING_DRAWING_LINE_GET_PRIVATE(self);

	assert(priv->placed == FALSE);

	if (priv->points == NULL) {
		if (other != NULL) {
			GdkPoint *new_point = g_new(GdkPoint, 1);
			new_point->x = x;
			new_point->y = y;

			priv->points = g_slist_append(priv->points, new_point);

			g_object_ref(other);
			priv->start = other;
		}
	} else {
		GdkPoint *new_point = g_new(GdkPoint, 1);
		new_point->x = x;
		new_point->y = y;

		priv->points = g_slist_append(priv->points, new_point);

		if (other != NULL) {
			g_object_ref(other);
			priv->end = other;
			priv->placed = TRUE;
		}
	}

	g_print("conting_drawing_line_place()\n");
}

static gboolean conting_drawing_line_is_placed(ContingDrawing *self) {
	ContingDrawingLinePrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING_LINE(self), FALSE);

	priv = CONTING_DRAWING_LINE_GET_PRIVATE(self);

	return priv->placed;
}

static gboolean conting_drawing_line_answer(ContingDrawing *self,
		gint x, gint y) {
	return FALSE;
}

static void conting_drawing_line_dispose(GObject *self) {
	ContingDrawingLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_LINE(self));

	priv = CONTING_DRAWING_LINE_GET_PRIVATE(self);

	if (priv->disposed)
		return;
	
	if (priv->start)
		g_object_unref(priv->start);
	if (priv->end)
		g_object_unref(priv->end);

	priv->disposed = TRUE;

	G_OBJECT_CLASS(parent_class)->dispose(self);
}

static void conting_drawing_line_finalize(GObject *self) {
	ContingDrawingLinePrivate *priv;
	GSList *n;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_LINE(self));

	priv = CONTING_DRAWING_LINE_GET_PRIVATE(self);

	for (n = priv->points; n != NULL; n = g_slist_next(n)) {
		g_free(n->data);
	}

	g_slist_free(priv->points);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void conting_drawing_line_class_init(gpointer g_class,
        gpointer class_data) {
    ContingDrawingClass *drawing_class;
	GObjectClass *object_class;

	g_type_class_add_private(g_class, sizeof(ContingDrawingLinePrivate));

	parent_class = g_type_class_peek_parent(g_class);
	
	g_print("drawing_line_class_init()\n");
    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_drawing_line_draw;
//	drawing_class->draw_selected = conting_drawing_line_draw;
	drawing_class->start_place = conting_drawing_line_start_place;
	drawing_class->place = conting_drawing_line_place;
	drawing_class->is_placed = conting_drawing_line_is_placed;
	drawing_class->answer = conting_drawing_line_answer;

	object_class = G_OBJECT_CLASS(g_class);
	object_class->finalize = conting_drawing_line_finalize;
	object_class->dispose = conting_drawing_line_dispose;

//    drawing_class->get_rectangle = conting_drawing_line_get_rectangle;
}

static void conting_drawing_line_instance_init(GTypeInstance *obj,
        gpointer g_class) {
	ContingDrawingLinePrivate *priv;

	g_return_if_fail(obj != NULL && CONTING_IS_DRAWING_LINE(obj));

	priv = CONTING_DRAWING_LINE_GET_PRIVATE(obj);

	priv->start = NULL;
	priv->end = NULL;

	priv->points = NULL;
	priv->placed = FALSE;

	priv->disposed = FALSE;
}

GType conting_drawing_line_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingDrawingLineClass),
            NULL,
            NULL,
            conting_drawing_line_class_init,
            NULL,
            NULL,
            sizeof(ContingDrawingLine),
            0,
            conting_drawing_line_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_DRAWING,
				"ContingDrawingLine",
                &type_info, 0);
    }

    return type;
}

ContingDrawing *conting_drawing_line_new(void) {
	g_print("conting_drawing_line_new()\n");
    return CONTING_DRAWING(g_object_new(CONTING_TYPE_DRAWING_LINE, NULL));
}
