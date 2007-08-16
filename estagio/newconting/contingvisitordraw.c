#include "contingvisitor.h"

typedef struct ContingVisitorDraw_ ContingVisitorDraw;
typedef struct ContingVisitorDrawClass_ ContingVisitorDrawClass;

struct ContingVisitorDraw_ {
	GObject parent;
};

struct ContingVisitorDrawClass_
{
	GObjectClass parent;
};

static void
conting_visit_simple(ContingVisitor *self, ContingDrawing *drawing)
{
	cairo_t *cr;

	cr = conting_drawing_get_cairo(drawing);
	
	conting_drawing_draw(drawing, cr);

	cairo_destroy(cr);
}

static void
conting_visitor_draw_visitor_init(gpointer g_iface,
		gpointer iface_data)
{
	ContingVisitorClass *visitor_class;

	visitor_class = g_iface;

	visitor_class->visit_line = conting_visit_simple;
	visitor_class->visit_bus = conting_visit_simple;
	visitor_class->visit_load = conting_visit_simple;
	visitor_class->visit_gen = conting_visit_simple
	visitor_class->visit_ce = conting_visit_simple
	visitor_class->visit_cs = conting_visit_simple
	
	visitor_class->visit_trans2 = conting_visit_simple;
	visitor_class->visit_trans3 = conting_visit_simple;
}

GType
conting_visitor_draw_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo type_info = {
			sizeof(ContingVisitorDrawClass),
			NULL,
			NULL,
			NULL, /* conting_visitor_draw_class_init, */
			NULL,
			NULL,
			sizeof(ContingVisitorDraw),
			0,
			NULL, /* conting_visitor_draw_instance_init, */
			NULL,
		};

		static const GInterfaceInfo iface_info = {
			conting_visitor_draw_visitor_init,
			NULL,
			NULL
		};

		type = g_type_register_static(G_TYPE_OBJECT,
				"ContingVisitorDraw",
				&type_info, 0);

		g_type_add_interface_static(type,
				CONTING_TYPE_VISITOR,
				&iface_info);
	}

	return type;
}
