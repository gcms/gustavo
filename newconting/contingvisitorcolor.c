#include "contingvisitor.h"
#include "contingvisitorcolor.h"
#include <assert.h>

static gpointer parent_class = NULL;

#define CONTING_VISITOR_COLOR_GET_PRIVATE(o) \
   (G_TYPE_INSTANCE_GET_PRIVATE((o), CONTING_TYPE_VISITOR_COLOR, \
                                ContingVisitorColorPrivate))

typedef struct ContingVisitorColorPrivate_ ContingVisitorColorPrivate;

struct ContingVisitorColorPrivate_ {
    gboolean use_default;
    GdkColor default_color;

    GList *color_ranges;
};

typedef struct {
    gdouble start, end;
    GdkColor color;
} ContingColorRange;

static void
color_by_voltage(ContingVisitor *self, GdkColor *color, gdouble voltage)
{
    ContingVisitorColorPrivate *priv;
    GList *n;

    g_return_if_fail(self != NULL && CONTING_IS_VISITOR_COLOR(self));

    priv = CONTING_VISITOR_COLOR_GET_PRIVATE(self);

    *color = priv->default_color;

    if (priv->use_default)
        return;

    for (n = priv->color_ranges; n != NULL; n = g_list_next(n)) {
        ContingColorRange *range = n->data;

        if ((range->start == - 1 || voltage >= range->start)
                && (range->end == -1 || voltage <= range->end)) {
            *color = range->color;
            return;
        }
    }
    
    /*
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
    */
}

void
conting_visitor_color_set_default_color(ContingVisitorColor *self,
        GdkColor *color)
{
    ContingVisitorColorPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_VISITOR_COLOR(self));

    priv = CONTING_VISITOR_COLOR_GET_PRIVATE(self);

    priv->default_color = *color;
}

void
conting_visitor_color_use_default(ContingVisitorColor *self, gboolean use)
{
    ContingVisitorColorPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_VISITOR_COLOR(self));

    priv = CONTING_VISITOR_COLOR_GET_PRIVATE(self);

    priv->use_default = use;
}

void
conting_visitor_color_add_interval(ContingVisitorColor *self,
        gdouble start, gdouble end, GdkColor *color)
{
    ContingVisitorColorPrivate *priv;
    ContingColorRange *range;

    g_return_if_fail(self != NULL && CONTING_IS_VISITOR_COLOR(self));

    priv = CONTING_VISITOR_COLOR_GET_PRIVATE(self);

    range = g_new(ContingColorRange, 1);

    range->start = start;
    range->end = end;
    range->color = *color;

    priv->color_ranges = g_list_prepend(priv->color_ranges, range);
}

static void
conting_visitor_color_visit_line(ContingVisitor *self, ContingLine *line)
{
	ContingVisitorColorPrivate *priv;
	ContingComponent *comp0, *comp1;

	GdkColor color;

	ContingItemData *item_data;
	gdouble voltage;

	g_return_if_fail(self != NULL && CONTING_IS_VISITOR_COLOR(self));

	priv = CONTING_VISITOR_COLOR_GET_PRIVATE(self);

	if (priv->use_default) {
		g_object_set(line, "color", &priv->default_color, NULL);
		return;
	}

	conting_line_get_links(line, &comp0, &comp1);
	assert(comp0 && comp1);

	if (CONTING_IS_BUS(comp0)) {
		item_data = conting_drawing_get_item_data(CONTING_DRAWING(comp0));
	} else if CONTING_IS_BUS(comp1) {
		item_data = conting_drawing_get_item_data(CONTING_DRAWING(comp1));
	} else {
		item_data = NULL;

		conting_drawing_accept(CONTING_DRAWING(comp0), self);
		g_object_get(comp0, "color", &color, NULL);
		g_object_set(line, "color", &color, NULL);
	}

	g_return_if_fail(item_data != NULL && CONTING_IS_ITEM_DATA(item_data));
	conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);

	color_by_voltage(self, &color, voltage);

	g_object_set(line, "color", &color, NULL);
}

static void
conting_visitor_color_visit_bus(ContingVisitor *self,
		ContingDrawing *drawing)
{
	ContingVisitorColorPrivate *priv;
	GdkColor color;
	
	ContingItemData *item_data;
	gdouble voltage;

	g_return_if_fail(self != NULL && CONTING_IS_VISITOR_COLOR(self));
	g_return_if_fail(drawing != NULL && CONTING_IS_DRAWING(drawing));

	priv = CONTING_VISITOR_COLOR_GET_PRIVATE(self);

	if (priv->use_default) {
		g_object_set(drawing, "color", &priv->default_color, NULL);
		return;
	}

	item_data = conting_drawing_get_item_data(drawing);

	g_return_if_fail(item_data != NULL && CONTING_IS_ITEM_DATA(item_data));
	conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);

	color_by_voltage(self, &color, voltage);

	g_object_set(drawing, "color", &color, NULL);
}
static void
conting_visitor_color_visit_c_shunt(ContingVisitor *self,
        ContingCShunt *cshunt)
{
}

static void
conting_visitor_color_visit_trans2(ContingVisitor *self,
        ContingTrans2 *trans2)
{
	ContingVisitorColorPrivate *priv;
	ContingBus *bus0, *bus1;

	GdkColor color;

	ContingItemData *item_data;
	gdouble voltage;

	g_return_if_fail(self != NULL && CONTING_IS_VISITOR_COLOR(self));

	priv = CONTING_VISITOR_COLOR_GET_PRIVATE(self);

	if (priv->use_default) {
		g_object_set(trans2, "color0", &priv->default_color, NULL);
		g_object_set(trans2, "color1", &priv->default_color, NULL);
		return;
	}

	conting_trans2_get_buses(trans2, &bus0, &bus1);

	if ((item_data = conting_drawing_get_item_data(CONTING_DRAWING(bus0)))) {
		conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);
		color_by_voltage(self, &color, voltage);
		g_object_set(trans2, "color0", &color, NULL);
	}

	if ((item_data = conting_drawing_get_item_data(CONTING_DRAWING(bus1)))) {
		conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);
		color_by_voltage(self, &color, voltage);
		g_object_set(trans2, "color1", &color, NULL);
		
	}
}
static void
conting_visitor_color_visit_trans3(ContingVisitor *self,
        ContingTrans3 *trans3)
{
	ContingVisitorColorPrivate *priv;
	ContingBus *bus0, *bus1, *bus2;

	GdkColor color;

	ContingItemData *item_data;
	gdouble voltage;
	
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR_COLOR(self));

	priv = CONTING_VISITOR_COLOR_GET_PRIVATE(self);

	if (priv->use_default) {
		g_object_set(trans3, "color0", &priv->default_color, NULL);
		g_object_set(trans3, "color1", &priv->default_color, NULL);
		g_object_set(trans3, "color2", &priv->default_color, NULL);
		return;
	}


	conting_trans3_get_buses(trans3, &bus0, &bus1, &bus2);
	g_print("bus0 = %p %s\n", bus0, CONTING_IS_BUS(bus0) ? "YES" : "NO");
	g_print("bus1 = %p %s\n", bus1, CONTING_IS_BUS(bus1) ? "YES" : "NO");
	g_print("bus2 = %p %s\n", bus2, CONTING_IS_BUS(bus2) ? "YES" : "NO");

	if ((item_data = conting_drawing_get_item_data(CONTING_DRAWING(bus0)))) {
		conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);
		color_by_voltage(self, &color, voltage);
		g_object_set(trans3, "color0", &color, NULL);
		g_print("bus0 voltage %lf\n", voltage);
	}

	if ((item_data = conting_drawing_get_item_data(CONTING_DRAWING(bus1)))) {
		conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);
		color_by_voltage(self, &color, voltage);
		g_object_set(trans3, "color1", &color, NULL);
		g_print("bus1 voltage %lf\n", voltage);
	}

	if ((item_data = conting_drawing_get_item_data(CONTING_DRAWING(bus2)))) {
		conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);
		color_by_voltage(self, &color, voltage);
		g_object_set(trans3, "color2", &color, NULL);
		g_print("bus2 voltage %lf\n", voltage);
	}
}
/*
static void
conting_visitor_color_visit_simple(ContingVisitor *visitor,
        ContingDrawing *drawing)
{
}
*/

static void
conting_visitor_color_visitor_init(gpointer g_iface,
		gpointer iface_data)
{
	ContingVisitorClass *visitor_class;

	visitor_class = g_iface;

	visitor_class->visit_line = conting_visitor_color_visit_line;
	visitor_class->visit_bus = (gpointer) conting_visitor_color_visit_bus;
	visitor_class->visit_load = (gpointer) conting_visitor_color_visit_bus;
	visitor_class->visit_gen = (gpointer) conting_visitor_color_visit_bus;
	visitor_class->visit_ce = (gpointer) conting_visitor_color_visit_bus;
	visitor_class->visit_cs = (gpointer) conting_visitor_color_visit_bus;
	
	visitor_class->visit_trans2 = conting_visitor_color_visit_trans2;
	visitor_class->visit_trans3 = conting_visitor_color_visit_trans3;

	visitor_class->visit_c_shunt = conting_visitor_color_visit_c_shunt;
}

static void
conting_visitor_color_finalize(GObject *self)
{
    ContingVisitorColorPrivate *priv;
    GList *n;

    g_return_if_fail(self != NULL && CONTING_IS_VISITOR_COLOR(self));

    priv = CONTING_VISITOR_COLOR_GET_PRIVATE(self);

    for (n = priv->color_ranges; n != NULL; n = g_list_next(n)) {
        g_free(n->data);
    }

    g_list_free(priv->color_ranges);

    priv->color_ranges = NULL;

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void
conting_visitor_color_instance_init(GTypeInstance *self, gpointer g_class)
{
    ContingVisitorColorPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_VISITOR_COLOR(self));

    priv = CONTING_VISITOR_COLOR_GET_PRIVATE(self);

    priv->use_default = TRUE;
    gdk_color_parse("black", &priv->default_color);

    priv->color_ranges = NULL;
}

static void
conting_visitor_color_class_init(gpointer g_class, gpointer class_data)
{
    GObjectClass *gobject_class;

    gobject_class = G_OBJECT_CLASS(g_class);
    gobject_class->finalize = conting_visitor_color_finalize;

    g_type_class_add_private(g_class, sizeof(ContingVisitorColorPrivate));

	parent_class = g_type_class_peek_parent(g_class);
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
			conting_visitor_color_class_init,
			NULL,
			NULL,
			sizeof(ContingVisitorColor),
			0,
			conting_visitor_color_instance_init,
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
