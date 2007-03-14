#include "contingdrawing.h"
#include "contingground.h"
#include "contingutil.h"
#include "contingxml.h"
#include <string.h>
#include <math.h>

static gpointer parent_class = NULL;

static void
conting_ground_draw(ContingDrawing *self, cairo_t *cr)
{
    ContingSymbol *symb;
    ContingComponent *comp;
	GdkColor *color;

    ArtPoint pw0, pw1;
	gdouble w, h;
    GdkRectangle rect;
	gdouble affine[6];

    g_return_if_fail(self != NULL && CONTING_IS_GROUND(self));

    symb = CONTING_SYMBOL(self);
    comp = CONTING_COMPONENT(self);

	g_object_get(self, "color", &color, NULL);

    pw0 = comp->p0;
    pw1 = comp->p1;

    rect.x = (pw0.x < pw1.x ? pw0.x : pw1.x);
    rect.y = (pw0.y < pw1.y ? pw0.y : pw1.y);
    rect.width = fabs(pw1.x - pw0.x);
    rect.height = fabs(pw1.y - pw0.y);

	cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);

	w = fabs(comp->p0.x - comp->p1.x);
	h = fabs(comp->p0.y - comp->p1.y);

	cairo_move_to(cr, comp->p0.x, comp->p0.y);
	cairo_line_to(cr, comp->p1.x, comp->p0.y);
	cairo_stroke(cr);

	cairo_move_to(cr, comp->p0.x + w * 0.2, comp->p0.y + h * 0.45);
	cairo_line_to(cr, comp->p1.x - w * 0.2, comp->p0.y + h * 0.45);
	cairo_stroke(cr);

	cairo_move_to(cr, comp->p0.x + w * 0.4, comp->p0.y + h * 0.9);
	cairo_line_to(cr, comp->p1.x - w * 0.4, comp->p0.y + h * 0.9);
	cairo_stroke(cr);


	CONTING_DRAWING_CLASS(parent_class)->draw(self, cr);
}

static void
conting_ground_accept(ContingDrawing *self, ContingVisitor *visitor)
{
	g_return_if_fail(self != NULL && CONTING_IS_GROUND(self));

	conting_visitor_visit_ground(visitor, CONTING_GROUND(self));
}

static void
conting_ground_get_link_point(ContingSymbol *self, ArtPoint *p)
{
	ContingComponent *comp;

	g_return_if_fail(self != NULL && CONTING_IS_GROUND(self));

	comp = CONTING_COMPONENT(self);

	p->x = comp->p0.x + (comp->p1.x - comp->p0.x) / 2.0;
	p->y = comp->p0.y;
}


static void
conting_ground_instance_init(GTypeInstance *self,
                           gpointer g_class)
{
    ContingSymbol *symb;
    ContingComponent *comp;

    g_return_if_fail(self != NULL && CONTING_IS_GROUND(self));

    symb = CONTING_SYMBOL(self);
    comp = CONTING_COMPONENT(self);

    comp->p0.x = -7.5;
    comp->p0.y = -3.5;
    comp->p1.x = 7.5;
    comp->p1.y = 3.5;

    symb->link0 = NULL;
}

static void
conting_ground_class_init(gpointer g_class, gpointer class_data)
{
    ContingDrawingClass *drawing_class;
	ContingSymbolClass *symbol_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_ground_draw;
	drawing_class->accept = conting_ground_accept;

	symbol_class = CONTING_SYMBOL_CLASS(g_class);
	symbol_class->get_link_point = conting_ground_get_link_point;

    parent_class = g_type_class_peek_parent(g_class);
}

GType conting_ground_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingGroundClass),
            NULL,
            NULL,
            conting_ground_class_init,
            NULL,
            NULL,
            sizeof(ContingGround),
            0,
            conting_ground_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_SYMBOL,
                "ContingGround",
                &type_info, 0);
    }

    return type;
}
