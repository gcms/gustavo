#include "contingvisitor.h"
#include <assert.h>

typedef struct ContingVisitorColor_ ContingVisitorColor;
typedef struct ContingVisitorColorClass_ ContingVisitorColorClass;

struct ContingVisitorColor_ {
	GObject parent;
};

struct ContingVisitorColorClass_
{
	GObjectClass parent;
};
static void
color_by_voltage(GdkColor *color, gdouble voltage)
{
	if (voltage < 25) {
		g_print("blue\n");
		gdk_color_parse("blue", color);
	} else if (voltage < 50) {
		g_print("orange\n");
		gdk_color_parse("orange", color);
	} else if (voltage < 100) {
		g_print("red\n");
		gdk_color_parse("red", color);
	} else if (voltage < 200) {
		g_print("yellow\n");
		gdk_color_parse("yellow", color);
	} else {
		g_print("green\n");
		gdk_color_parse("green", color);
	}
}

static void
conting_visitor_visit_line(ContingVisitor *self, ContingLine *line)
{
	ContingComponent *comp0, *comp1;

	GdkColor color;

	ContingItemData *item_data;
	gdouble voltage;

	conting_line_get_links(line, &comp0, &comp1);
	assert(comp0 && comp1);

	if (CONTING_IS_BUS(comp0)) {
		item_data = conting_drawing_get_item_data(CONTING_DRAWING(comp0));
	} else {
		assert(CONTING_IS_BUS(comp1));
		item_data = conting_drawing_get_item_data(CONTING_DRAWING(comp1));
	}

	g_return_if_fail(item_data != NULL && CONTING_IS_ITEM_DATA(item_data));
	conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);

	color_by_voltage(&color, voltage);

	g_object_set(line, "color", &color, NULL);
}

static void
conting_visitor_visit_bus(ContingVisitor *self, ContingBus *bus)
{
	GdkColor color;
	
	ContingItemData *item_data;
	gdouble voltage;

	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(bus != NULL && CONTING_IS_BUS(bus));

	item_data = conting_drawing_get_item_data(CONTING_DRAWING(bus));

	g_return_if_fail(item_data != NULL && CONTING_IS_ITEM_DATA(item_data));
	conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);

	color_by_voltage(&color, voltage);

	g_object_set(bus, "color", &color, NULL);
}

static void
conting_visitor_visit_trans2(ContingVisitor *self, ContingTrans2 *trans2)
{
	ContingBus *bus0, *bus1;

	GdkColor color;

	ContingItemData *item_data;
	gdouble voltage;

	conting_trans2_get_buses(trans2, &bus0, &bus1);

	if ((item_data = conting_drawing_get_item_data(bus0))) {
		conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);
		color_by_voltage(&color, voltage);
		g_object_set(trans2, "color0", &color);
	}

	if ((item_data = conting_drawing_get_item_data(bus1))) {
		conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);
		color_by_voltage(&color, voltage);
		g_object_set(trans2, "color1", &color);
		
	}
}
static void
conting_visitor_visit_trans3(ContingVisitor *self, ContingTrans3 *trans3)
{
	ContingBus *bus0, *bus1, *bus2;

	GdkColor color;

	ContingItemData *item_data;
	gdouble voltage;

	conting_trans3_get_buses(trans3, &bus0, &bus1, &bus2);
	g_print("bus0 = %p %s\n", bus0, CONTING_IS_BUS(bus0) ? "YES" : "NO");
	g_print("bus1 = %p %s\n", bus1, CONTING_IS_BUS(bus1) ? "YES" : "NO");
	g_print("bus2 = %p %s\n", bus2, CONTING_IS_BUS(bus2) ? "YES" : "NO");

	if ((item_data = conting_drawing_get_item_data(bus0))) {
		conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);
		color_by_voltage(&color, voltage);
		g_object_set(trans3, "color0", &color);
		g_print("bus0 voltage %lf\n", voltage);
	}

	if ((item_data = conting_drawing_get_item_data(bus1))) {
		conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);
		color_by_voltage(&color, voltage);
		g_object_set(trans3, "color1", &color);
		g_print("bus1 voltage %lf\n", voltage);
	}

	if ((item_data = conting_drawing_get_item_data(bus2))) {
		conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);
		color_by_voltage(&color, voltage);
		g_object_set(trans3, "color2", &color);
		g_print("bus2 voltage %lf\n", voltage);
	}
}
static void
conting_visitor_visit_simple(ContingVisitor *visitor, ContingDrawing *drawing)
{
}

static void
conting_visitor_color_visitor_init(gpointer g_iface,
		gpointer iface_data)
{
	ContingVisitorClass *visitor_class;

	visitor_class = g_iface;

	visitor_class->visit_line = conting_visitor_visit_line;
	visitor_class->visit_bus = conting_visitor_visit_bus;
	visitor_class->visit_load = conting_visitor_visit_simple;
	visitor_class->visit_gen = conting_visitor_visit_simple;
	
	visitor_class->visit_trans2 = conting_visitor_visit_trans2;
	visitor_class->visit_trans3 = conting_visitor_visit_trans3;
}

GType
conting_visitor_color_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo type_info = {
			sizeof(ContingVisitorColorClass),
			NULL,
			NULL,
			NULL, /* conting_visitor_color_class_init, */
			NULL,
			NULL,
			sizeof(ContingVisitorColor),
			0,
			NULL, /* conting_visitor_color_instance_init, */
			NULL,
		};

		static const GInterfaceInfo iface_info = {
			conting_visitor_color_visitor_init,
			NULL,
			NULL
		};

		type = g_type_register_static(G_TYPE_OBJECT,
				"ContingVisitorColor",
				&type_info, 0);

		g_type_add_interface_static(type,
				CONTING_TYPE_VISITOR,
				&iface_info);
	}

	return type;
}
