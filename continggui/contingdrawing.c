#include "contingdrawing.h"

/*
void conting_drawing_get_points(ContingDrawing *self,
        GdkPoint **links_points, gint *n_links) {
    ContingDrawingClass *klass;
    
    g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));
    
    klass = CONTING_DRAWING_GET_CLASS(self);
    klass->get_points(self, links_points, n_links);
}
*/

void conting_drawing_draw(ContingDrawing *self,
        GdkDrawable *drawable, GdkGC *gc,
		const GdkPoint *base_position, const GdkPoint *cur_position) {
	g_print("conting_drawing_draw()\n");
    ContingDrawingClass *klass;
   
    g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));
   
    klass = CONTING_DRAWING_GET_CLASS(self);

    g_return_if_fail(klass->draw != NULL);

    klass->draw(self, drawable, gc, base_position, cur_position);
}

void conting_drawing_draw_selected(ContingDrawing *self,
        GdkDrawable *drawable, GdkGC *gc, const GdkPoint *coord) {
    ContingDrawingClass *klass;
   
    g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));
   
    klass = CONTING_DRAWING_GET_CLASS(self);

    g_return_if_fail(klass->draw != NULL);

    klass->draw_selected(self, drawable, gc, coord);
}
/*
void conting_drawing_get_rectangle(ContingDrawing *self, GdkRectangle *rect) {
    g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self) && rect != NULL);

    CONTING_DRAWING_GET_CLASS(self)->get_rectangle(self, rect);
}


gboolean conting_drawing_violates(ContingDrawing *self, ContingDrawing *other,
		gint x, gint y) {
	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING(self), FALSE);

	return CONTING_DRAWING_GET_CLASS(self)->violates(self, other, x, y);
}

gboolean conting_drawing_can_link(ContingDrawing *self,
		ContingDrawing *link_drawing, gint x, gint y) {
	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING(self), FALSE);

	return CONTING_DRAWING_GET_CLASS(self)->can_link(self, link_drawing,
			x, y);
}
gboolean conting_drawing_place_linked(ContingDrawing *self,
		gint selfx, gint selfy,
		ContingDrawing *other, gint otherx, gint othery) {
	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING(self), FALSE);

	return CONTING_DRAWING_GET_CLASS(self)->place_linked(self, selfx, selfy,
			other, otherx, othery);
}
*/
void conting_drawing_place(ContingDrawing *self, gint x, gint y,
		ContingDrawing *other, gint ox, gint oy) {
	g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	CONTING_DRAWING_GET_CLASS(self)->place(self, x, y, other, ox, oy);
}

void conting_drawing_start_place(ContingDrawing *self) {
	g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	CONTING_DRAWING_GET_CLASS(self)->start_place(self);
}

gboolean conting_drawing_is_placed(ContingDrawing *self) {
	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING(self), FALSE);

	return CONTING_DRAWING_GET_CLASS(self)->is_placed(self);
}

gboolean conting_drawing_answer(ContingDrawing *self, gint x, gint y) {
	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING(self), FALSE);

	return CONTING_DRAWING_GET_CLASS(self)->answer(self, x, y);
}

static void conting_drawing_class_init(gpointer g_class,
        gpointer class_data) {
    ContingDrawingClass *klass = CONTING_DRAWING_CLASS(g_class);

    klass->draw = NULL;
	klass->draw_selected = NULL;
	klass->start_place = NULL;
	klass->place = NULL;
	klass->is_placed = NULL;
	klass->answer = NULL;
	
	/*
    klass->get_rectangle = NULL;
	klass->violates = NULL;
	klass->can_link = NULL;
	klass->place_linked = NULL;
	klass->place = NULL;
	*/
}

GType conting_drawing_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingDrawingClass),
            NULL,
            NULL,
            conting_drawing_class_init,
            NULL,
            NULL,
            sizeof(ContingDrawing),
            0,
            NULL,
            NULL
        };

        type = g_type_register_static(G_TYPE_OBJECT, "ContingDrawing",
                &type_info, 0);
    }

    return type;
}
