#include "contingdrawing.h"
#include "contingdrawingoperation.h"
#include "contingline.h"

#include <string.h>
#include <math.h>

typedef struct ContingDrawingOperationLoad_ ContingDrawingOperationLoad;
typedef struct ContingDrawingOperationLoadClass_ ContingDrawingOperationLoadClass;

enum {
	CONTING_DRAWING_OPERATION_LOAD_PROP_0,
	CONTING_DRAWING_OPERATION_LOAD_PROP_COLOR,
	CONTING_DRAWING_OPERATION_LOAD_PROP_COLOR_NAME
};

struct ContingDrawingOperationLoad_ {
	GObject parent;

	GdkColor color;
};

struct ContingDrawingOperationLoadClass_
{
	GObjectClass parent;
};

static void
conting_drawing_operation_load_get_bounds(ContingDrawingOperation *self,
		ContingDrawing *drawing, ArtDRect *bounds)
{
	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_OPERATION(self));

	conting_drawing_get_update_bounds(drawing, bounds);
}

ContingItemData *
get_flow_model(ContingDrawing *drawing)
{
	ContingItemData *item, *flow;
	ContingModel *model;
	gint x, y;

	g_return_val_if_fail(drawing != NULL && CONTING_IS_DRAWING(drawing), NULL);
	item = conting_drawing_get_item_data(drawing);

	g_return_val_if_fail(item != NULL && CONTING_IS_ITEM_DATA(item), NULL);
	/** Assuming that the item is always a CONTING_ITEM_DATA_TYPE_BRANCH */

	conting_item_data_get_attr(item,
			"tap bus number", &x,
			"z bus number", &y,
			NULL);

	g_print("X IS %d, Y IS %d\n", x, y);

	model = conting_one_line_load_flow_model(
			conting_drawing_get_one_line(drawing));
	g_return_val_if_fail(model != NULL && CONTING_IS_MODEL(model), NULL);

	return conting_model_get_item(model,
			CONTING_ITEM_TYPE_FLOW_BRANCH,
			conting_item_branch_key_new(x, y));
}

static gdouble
draw_line_max(cairo_t *cr,
		const ArtPoint *a, const ArtPoint *b, gdouble max_d,
		ArtPoint *last_point)
{
	ArtPoint p0, p1;
	gdouble d;

	p0 = *a;
	p1 = *b;

	d = sqrt(pow(p0.x - p1.x, 2) + pow(p0.y - p1.y, 2));
	g_print("distance between (%lf, %lf) and (%lf, %lf) is %lf\n",
			p0.x, p0.y, p1.x, p1.y, d);

	if (d > max_d) {
		conting_util_points_max(&p0, &p1, max_d);
		d = sqrt(pow(p0.x - p1.x, 2) + pow(p0.y - p1.y, 2));
	}

	cairo_move_to(cr, p0.x, p0.y);
	cairo_line_to(cr, p1.x, p1.y);
	cairo_stroke(cr);

	if (last_point)
		*last_point = p1;

	return d;
}

static gint
get_bus_number(ContingBus *bus)
{
	ContingItemData *item;
	gint number;

	g_return_val_if_fail(bus != NULL && CONTING_IS_BUS(bus), -1);

	item = conting_drawing_get_item_data(CONTING_DRAWING(bus));
	g_return_val_if_fail(item != NULL && CONTING_IS_ITEM_DATA(item), -1);

	number = -1;
	conting_item_data_get_attr(item,
			"number", &number,
			NULL);

	return number;
}


static void
conting_drawing_operation_load_draw(ContingDrawingOperation *self,
		ContingDrawing *drawing)
{
	ContingDrawingOperationLoad *opr;
	ContingLine *line;
	ContingBusBase *bus0, *bus1;

	ContingItemData *flow_data;
	gint number1, number2;
	gdouble p1, p2;
	const ArtPoint bus1_p, bus2_p;

	const GList *points;
	GList *n0, *n1;
	gdouble acc_d;

	cairo_t *cr;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_OPERATION(self));

	opr = (ContingDrawingOperationLoad *) self;

	/* Maybe use a visitor? */
	if (!CONTING_IS_LINE(drawing))
		return;


	line = CONTING_LINE(drawing);
	conting_line_get_buses(line, &bus0, &bus1);
	if (bus0 == NULL || !CONTING_IS_BUS(bus0)
			|| bus1 == NULL || !CONTING_IS_BUS(bus1))
		return;

	flow_data = get_flow_model(drawing);

	g_print("flow_data = %p\n", flow_data);

	conting_item_data_get_attr(flow_data,
			"start-1", &number1,
			"start-2", &number2,
			"p-1", &p1,
			"p-2", &p2,
			NULL);

	points = conting_line_get_points(line);

	cr = conting_drawing_get_cairo(drawing);
	cairo_set_source_rgb(cr,
			(gdouble) opr->color.red / (gdouble) G_MAXUINT16,
			(gdouble) opr->color.green / (gdouble) G_MAXUINT16,
			(gdouble) opr->color.blue / (gdouble) G_MAXUINT16);
	
	acc_d = 0;
	n0 = g_list_first((GList *) points);
	n1 = g_list_next((GList *) n0);

	while (n1 && acc_d < 10) {
		acc_d += draw_line_max(cr, n0->data, n1->data, 10 - acc_d, &bus1_p);

		n0 = n1;
		n1 = g_list_next(n1);
	}

	acc_d = 0;
	n0 = g_list_last((GList *) points);
	n1 = g_list_previous(n0);

	while (n1 && acc_d < 10) {
		acc_d += draw_line_max(cr, n0->data, n1->data, 10 - acc_d, &bus2_p);

		n0 = n1;
		n1 = g_list_previous(n1);
	}

	cairo_set_source_rgb(cr, 0, 1, 0);
	{
		PangoLayout *layout;
		PangoFontDescription *font;
		PangoRectangle  logical_rect;
		gchar buf[256];

		layout = pango_cairo_create_layout(cr);

		font = pango_font_description_new();
		pango_font_description_set_size(font, 4.5 * PANGO_SCALE);
	/*	g_print("size = %d\n", pango_font_description_get_size(font)); */
		pango_font_description_set_family_static(font, "Arial");
		pango_font_description_set_style(font, PANGO_STYLE_NORMAL);

		pango_layout_set_font_description(layout, font);

		sprintf(buf, "%.2lf", p1);
		pango_layout_set_text(layout, buf, strlen(buf));

		pango_layout_get_pixel_extents(layout, NULL, &logical_rect);

		cairo_move_to(cr, bus1_p.x, bus1_p.y);

		pango_cairo_update_layout(cr, layout);
		pango_cairo_show_layout(cr, layout);

	 	g_object_unref(layout);
	}


/*
	conting_line_get_limits(line, &p1, &p2);
	cairo_move_to(cr, p1.x0, p1.y0);
	cairo_line_to(cr, p1.x1, p1.y1);
	cairo_line_to(cr, p1.x0, p1.y0);
	cairo_stroke(cr);

	{
		gdouble m;
		gdouble affine[6];
		ArtPoint po0, po1;

		po0.x = p1.x0;
		po0.y = p1.y0;
		po1.x = p1.x1;
		po1.y = p1.y1;

		m = (po0.y - po1.y) / (po0.x - po1.x);

		art_affine_rotate(affine, -180 * atan(m) / M_PI);
		art_affine_point(&po0, &po0, affine);
		art_affine_point(&po1, &po1, affine);

		po0.y += 5;
		po1.y += 5;

		art_affine_rotate(affine, 180 * atan(m) / M_PI);
		art_affine_point(&po0, &po0, affine);
		art_affine_point(&po1, &po1, affine);

		cairo_move_to(cr, po0.x, po0.y);
		cairo_line_to(cr, po1.x, po1.y);
		cairo_stroke(cr);
	}
	*/

	cairo_destroy(cr);
}

static void
conting_drawing_operation_load_get_property(GObject *self,
                             guint prop_id,
                             GValue *value,
                             GParamSpec *pspec)
{
    ContingDrawingOperationLoad *opr;

    g_return_if_fail(self != NULL && CONTING_IS_DRAWING_OPERATION(self));

    opr = (ContingDrawingOperationLoad *) self;

    switch (prop_id) {
		case CONTING_DRAWING_OPERATION_LOAD_PROP_COLOR:
			g_value_set_pointer(value, &opr->color);
			break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

static void
conting_drawing_operation_load_set_property(GObject *self,
                             guint prop_id,
                             const GValue *value,
                             GParamSpec *pspec)
{
    ContingDrawingOperationLoad *opr;

    g_return_if_fail(self != NULL && CONTING_IS_DRAWING_OPERATION(self));

	opr = (ContingDrawingOperationLoad *) self;

    switch (prop_id) {
		case CONTING_DRAWING_OPERATION_LOAD_PROP_COLOR:
			opr->color = *((GdkColor *) g_value_get_pointer(value));
			break;
		case CONTING_DRAWING_OPERATION_LOAD_PROP_COLOR_NAME:
			gdk_color_parse(g_value_get_string(value), &opr->color);
			break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}
static void
conting_drawing_operation_load_instance_init(GTypeInstance *self,
		gpointer g_class)
{
	ContingDrawingOperationLoad *opr = (ContingDrawingOperationLoad *) self;

	gdk_color_parse("red", &opr->color);
}

static void
conting_drawing_operation_load_drawing_operation_init(gpointer g_iface,
		gpointer iface_data)
{
	ContingDrawingOperationClass *opr_class;

	opr_class = g_iface;
	opr_class->get_bounds = conting_drawing_operation_load_get_bounds;
	opr_class->draw = conting_drawing_operation_load_draw;
}

static void
conting_drawing_operation_load_class_init(gpointer g_class,
		gpointer class_data)
{
	GObjectClass *gobject_class;

	gobject_class = G_OBJECT_CLASS(g_class);
	gobject_class->set_property = conting_drawing_operation_load_set_property;
	gobject_class->get_property = conting_drawing_operation_load_get_property;

	g_object_class_install_property(gobject_class,
			CONTING_DRAWING_OPERATION_LOAD_PROP_COLOR,
			g_param_spec_pointer("color",
								 "Font color",
								 "Font color",
								 G_PARAM_READABLE | G_PARAM_WRITABLE));

	g_object_class_install_property(gobject_class,
			CONTING_DRAWING_OPERATION_LOAD_PROP_COLOR_NAME,
			g_param_spec_string("color-name",
								"Font color name",
								"The name of the color",
								"red",
								G_PARAM_WRITABLE));
	
}

GType
conting_drawing_operation_load_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo type_info = {
			sizeof(ContingDrawingOperationLoadClass),
			NULL,
			NULL,
			conting_drawing_operation_load_class_init,
			NULL,
			NULL,
			sizeof(ContingDrawingOperationLoad),
			0,
			conting_drawing_operation_load_instance_init,
			NULL,
		};

		static const GInterfaceInfo iface_info = {
			conting_drawing_operation_load_drawing_operation_init,
			NULL,
			NULL
		};

		type = g_type_register_static(G_TYPE_OBJECT,
				"ContingDrawingOperationLoad",
				&type_info, 0);

		g_type_add_interface_static(type,
				CONTING_TYPE_DRAWING_OPERATION,
				&iface_info);
	}

	return type;
}

ContingDrawingOperation *
conting_drawing_operation_load_new(void)
{
    return g_object_new(CONTING_TYPE_DRAWING_OPERATION_LOAD, NULL);
}
