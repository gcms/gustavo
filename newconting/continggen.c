#include "continggen.h"
#include "contingutil.h"
#include "contingxml.h"
#include <string.h>
#include <math.h>

static gpointer parent_class = NULL;

static void
conting_gen_draw(ContingDrawing *self,
                    GdkDrawable *drawable,
                    const GdkRectangle *drawing_rect)
{
    ContingSymbol *symb;
    ContingComponent *comp;
	
    gdouble affine[6];

    ArtPoint pw0, pw1;
    GdkRectangle rect;

	cairo_t *cr;

    g_return_if_fail(self != NULL && CONTING_IS_GEN(self));

    symb = CONTING_SYMBOL(self);
    comp = CONTING_COMPONENT(self);

    conting_drawing_get_i2w_affine(self, affine);

    pw0 = comp->p0;
    pw1 = comp->p1;

    art_affine_point(&pw0, &pw0, affine);
    art_affine_point(&pw1, &pw1, affine);

    conting_one_line_world_to_window(conting_drawing_get_one_line(self),
            pw0.x, pw0.y, &pw0.x, &pw0.y);
    conting_one_line_world_to_window(conting_drawing_get_one_line(self),
            pw1.x, pw1.y, &pw1.x, &pw1.y);

    rect.x = (pw0.x < pw1.x ? pw0.x : pw1.x);
    rect.y = (pw0.y < pw1.y ? pw0.y : pw1.y);
    rect.width = fabs(pw1.x - pw0.x);
    rect.height = fabs(pw1.y - pw0.y);

	cr = gdk_cairo_create(drawable);
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);

	cairo_arc(cr,
			rect.x + ((gdouble) rect.width / 2.0),
			rect.y + ((gdouble) rect.height / 2.0),
			(gdouble) rect.width / 2.0, 0, 2 * M_PI);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_stroke(cr);

	cairo_arc(cr,
			rect.x + ((gdouble) rect.width / 3.0),
			rect.y + ((gdouble) rect.height / 2.0),
			(gdouble) rect.width / 8.0, 0, M_PI);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_stroke(cr);

	cairo_arc(cr,
			rect.x + (2 * (gdouble) rect.width / 3.0),
			rect.y + ((gdouble) rect.height / 2.0),
			(gdouble) rect.width / 8.0, M_PI, 2 * M_PI);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_stroke(cr);

	CONTING_DRAWING_CLASS(parent_class)->draw(self, drawable, drawing_rect);
}

static void
conting_gen_instance_init(GTypeInstance *self,
                           gpointer g_class)
{
    ContingSymbol *symb;
    ContingComponent *comp;

    g_return_if_fail(self != NULL && CONTING_IS_GEN(self));

    symb = CONTING_SYMBOL(self);
    comp = CONTING_COMPONENT(self);

    comp->p0.x = -5;
    comp->p0.y = -5;
    comp->p1.x = 5;
    comp->p1.y = 5;

    symb->link0 = NULL;
}

static void
conting_gen_class_init(gpointer g_class, gpointer class_data)
{
    ContingDrawingClass *drawing_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_gen_draw;

    parent_class = g_type_class_peek_parent(g_class);
}

GType conting_gen_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingGenClass),
            NULL,
            NULL,
            conting_gen_class_init,
            NULL,
            NULL,
            sizeof(ContingGen),
            0,
            conting_gen_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_SYMBOL,
                "ContingGen",
                &type_info, 0);
    }

    return type;
}
