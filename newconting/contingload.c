#include "contingload.h"
#include "contingutil.h"
#include "contingxml.h"
#include <string.h>
#include <math.h>

static gpointer parent_class = NULL;

static void
conting_load_draw(ContingDrawing *self,
                    GdkDrawable *drawable,
                    const GdkRectangle *drawing_rect)
{
    ContingComponent *comp;
	
    gdouble affine[6];

    ArtPoint pw0, pw1, pw2;

	cairo_t *cr;

    g_return_if_fail(self != NULL && CONTING_IS_LOAD(self));

    comp = CONTING_COMPONENT(self);

    conting_drawing_get_i2w_affine(self, affine);

    pw0 = comp->p0;
    pw1 = comp->p1;

	pw0.x = comp->p1.x;
	pw0.y = comp->p0.y;

	pw1.x = comp->p1.x;
	pw1.y = comp->p1.y;

	pw2.x = comp->p0.x;
	pw2.y = comp->p0.y + (comp->p1.y - comp->p0.y) / 2.0;

    art_affine_point(&pw0, &pw0, affine);
    art_affine_point(&pw1, &pw1, affine);
    art_affine_point(&pw2, &pw2, affine);

    conting_one_line_world_to_window(conting_drawing_get_one_line(self),
            pw0.x, pw0.y, &pw0.x, &pw0.y);
    conting_one_line_world_to_window(conting_drawing_get_one_line(self),
            pw1.x, pw1.y, &pw1.x, &pw1.y);
    conting_one_line_world_to_window(conting_drawing_get_one_line(self),
            pw2.x, pw2.y, &pw2.x, &pw2.y);

	cr = gdk_cairo_create(drawable);
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);

	cairo_move_to(cr, pw0.x, pw0.y);
	cairo_line_to(cr, pw1.x, pw1.y);
	cairo_line_to(cr, pw2.x, pw2.y);
	cairo_line_to(cr, pw0.x, pw0.y);
	cairo_fill_preserve(cr);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_stroke(cr);

	CONTING_DRAWING_CLASS(parent_class)->draw(self, drawable, drawing_rect);
}
static void
conting_load_instance_init(GTypeInstance *self,
                           gpointer g_class)
{
    ContingComponent *comp;

    g_return_if_fail(self != NULL && CONTING_IS_LOAD(self));

    comp = CONTING_COMPONENT(self);

    comp->p0.x = -2;
    comp->p0.y = -2;
    comp->p1.x = 2;
    comp->p1.y = 2;
}

static void
conting_load_class_init(gpointer g_class, gpointer class_data)
{
    ContingDrawingClass *drawing_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_load_draw;

    parent_class = g_type_class_peek_parent(g_class);
}

GType conting_load_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingLoadClass),
            NULL,
            NULL,
            conting_load_class_init,
            NULL,
            NULL,
            sizeof(ContingLoad),
            0,
            conting_load_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_SYMBOL,
                "ContingLoad",
                &type_info, 0);
    }

    return type;
}
