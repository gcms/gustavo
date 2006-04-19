#include "contingcomponent.h"
#include <assert.h>
#include <stdlib.h>

static GdkColor color;

static void conting_component_draw_selected(ContingDrawing *self,
		GdkDrawable *draw, GdkGC *gc, const GdkPoint *p) {
	GdkRectangle rect;

	conting_component_get_rectangle(CONTING_COMPONENT(self), &rect);

	rect.x += p->x;
	rect.y += p->y;
	
	gdk_gc_set_rgb_fg_color(gc, &color);

	gdk_draw_rectangle(draw, gc, TRUE,
			rect.x - 7, rect.y + rect.height / 2 - 3,
			6, 6);
	gdk_draw_rectangle(draw, gc, TRUE,
			rect.x + rect.width / 2 - 3, rect.y - 7,
			6, 6);
	gdk_draw_rectangle(draw, gc, TRUE,
			rect.x + rect.width + 1, rect.y + rect.height / 2 - 3,
			6, 6);
	gdk_draw_rectangle(draw, gc, TRUE,
			rect.x + rect.width / 2 - 3, rect.y + rect.height + 1,
			6, 6);
	
	return;
}

static gboolean conting_component_answer(ContingDrawing *self,
		gint x, gint y) {
	GdkRectangle rect;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

	conting_component_get_rectangle(CONTING_COMPONENT(self), &rect);

	return rect.x <= x && rect.y <= y
		&& x - rect.x < rect.width && y - rect.y < rect.height;
}

static gboolean conting_component_answer_resize_impl(ContingComponent *self,
		gint x, gint y, ContingResizeOrientation *orientation) {
	ContingResizeOrientation resize;
	GdkRectangle rect;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);
	g_return_val_if_fail(orientation != NULL, FALSE);

	conting_component_get_rectangle(self, &rect);
	conting_component_resizeable(self, &resize);

	if ((resize & CONTING_RESIZE_HORIZONTAL)
			&& ((abs(x - (rect.x - 3)) < 5 && abs(y) < 5)
				|| (abs(x - (rect.x + rect.width + 3)) < 5 && abs(y) < 5))) {
		*orientation = CONTING_RESIZE_HORIZONTAL;

		return TRUE;
	}

	if ((resize & CONTING_RESIZE_VERTICAL)
			&& ((abs(y - (rect.y - 3)) < 5 && abs(x) < 5)
				|| (abs(y - (rect.y + rect.height + 3)) < 5 && abs(x) < 5))) {
		*orientation = CONTING_RESIZE_VERTICAL;

		return TRUE;
	}

	return FALSE;
}

static void conting_drawing_move_impl(ContingDrawing *self, gint x, gint y) {
	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

	conting_component_move(CONTING_COMPONENT(self), x, y);
}

static void conting_component_class_init(gpointer g_class,
        gpointer class_data) {
    ContingDrawingClass *drawing_class;
	ContingComponentClass *component_class;

	component_class = CONTING_COMPONENT_CLASS(g_class);
	component_class->get_rectangle = NULL;
	component_class->move = NULL;
	component_class->rotate = NULL;
	component_class->connect = NULL;
	component_class->answer_resize = conting_component_answer_resize_impl;
	component_class->resizeable = NULL;
	component_class->resize = NULL;
    
    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw_selected = conting_component_draw_selected;
	drawing_class->answer = conting_component_answer;
	drawing_class->answer_move = conting_component_answer;
	drawing_class->move = conting_drawing_move_impl;

	gdk_color_parse("black", &color);
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
            NULL,
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
void conting_component_get_rectangle(ContingComponent *self,
		GdkRectangle *rect) {
	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));
	g_return_if_fail(rect != NULL);

	CONTING_COMPONENT_GET_CLASS(self)->get_rectangle(self, rect);
}

void conting_component_move(ContingComponent *self, gint x, gint y) {
	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

	CONTING_COMPONENT_GET_CLASS(self)->move(self, x, y);
}

void conting_component_rotate(ContingComponent *self, gdouble theta) {
	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

	CONTING_COMPONENT_GET_CLASS(self)->rotate(self, theta);
}

void conting_component_resizeable(ContingComponent *self,
		ContingResizeOrientation *orientation) {
	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));
	g_return_if_fail(orientation != NULL);

	CONTING_COMPONENT_GET_CLASS(self)->resizeable(self, orientation);
}

gboolean conting_component_connect(ContingComponent *self,
		ContingConnection *conn,
		gint x, gint y, GdkPoint *shift) {
	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);
	g_return_val_if_fail(conn != NULL && CONTING_IS_CONNECTION(conn), FALSE);

	return CONTING_COMPONENT_GET_CLASS(self)->connect(self, conn, x, y, shift);
}
gboolean conting_component_answer_resize(ContingComponent *self,
		gint x, gint y, ContingResizeOrientation *orientation) {
	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);
	g_return_val_if_fail(orientation != NULL, FALSE);

	return CONTING_COMPONENT_GET_CLASS(self)->answer_resize(self, x, y,
			orientation);
}
void conting_component_resize(ContingComponent *self, gint x, gint y,
		ContingResizeOrientation orientation) {
	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));
	g_return_if_fail(orientation != NULL);

	CONTING_COMPONENT_GET_CLASS(self)->resize(self, x, y, orientation);
}
