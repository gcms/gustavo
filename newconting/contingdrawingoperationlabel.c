#include "contingdrawing.h"
#include "contingdrawingoperation.h"
#include "contingoneline.h"
#include "contingbusbase.h"

#include <string.h>
#include <math.h>

typedef struct ContingDrawingOperationLabel_ ContingDrawingOperationLabel;
typedef struct ContingDrawingOperationLabelClass_ ContingDrawingOperationLabelClass;

enum {
	CONTING_DRAWING_OPERATION_LABEL_PROP_0,
	CONTING_DRAWING_OPERATION_LABEL_PROP_TEXT,
	CONTING_DRAWING_OPERATION_LABEL_PROP_ATTR,
	CONTING_DRAWING_OPERATION_LABEL_PROP_USER_DATA,
	CONTING_DRAWING_OPERATION_LABEL_PROP_PLACE,
	CONTING_DRAWING_OPERATION_LABEL_PROP_COLOR,
	CONTING_DRAWING_OPERATION_LABEL_PROP_COLOR_NAME
};

struct ContingDrawingOperationLabel_ {
	GObject parent;

	ContingLabelFunc label_func;
	gpointer user_data;
	const gchar *text;

	ContingDrawingOperationLabelPlace place;

	GdkColor color;
};

struct ContingDrawingOperationLabelClass_
{
	GObjectClass parent;
};

static void
conting_drawing_operation_label_get_bounds(ContingDrawingOperation *self,
		ContingDrawing *drawing, ArtDRect *bounds)
{
	ContingDrawingOperationLabel *opr;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_OPERATION(self));

	conting_drawing_get_update_bounds(drawing, bounds);

	opr = (ContingDrawingOperationLabel *) self;

	if (opr->label_func(self, drawing, opr->user_data)) {
		bounds->x1 += 80;
		bounds->y0 -= 50;
	}
}

static void
conting_drawing_operation_label_draw(ContingDrawingOperation *self,
		ContingDrawing *drawing)
{
	ContingDrawingOperationLabel *opr;
	cairo_t *cr;
    ArtDRect bounds;
	ArtPoint pw0;
	const gchar *name;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_OPERATION(self));

	opr = (ContingDrawingOperationLabel *) self;

	if (opr->label_func) {
		name = opr->label_func(self, drawing, opr->user_data);
	} else {
		name = opr->text;
	}

	if (name == NULL)
		return;

    conting_drawing_get_bounds(drawing, &bounds);

	pw0.x = MIN(bounds.x0, bounds.x1);
	pw0.y = MIN(bounds.y0, bounds.y1);

	pw0.y -= 5;

	cr = conting_drawing_get_cairo_absolute(drawing);

	cairo_set_font_size(cr, 8);
	cairo_select_font_face(cr, "Arial",
			CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_source_rgb(cr,
			(gdouble) opr->color.red / (gdouble) G_MAXUINT16,
			(gdouble) opr->color.green / (gdouble) G_MAXUINT16,
			(gdouble) opr->color.blue / (gdouble) G_MAXUINT16);
	{
		PangoLayout *layout;
		PangoFontDescription *font;
		PangoRectangle  logical_rect;

		layout = pango_cairo_create_layout(cr);

		font = pango_font_description_new();
		pango_font_description_set_size(font, 6 * PANGO_SCALE);
		g_print("size = %d\n", pango_font_description_get_size(font));
		pango_font_description_set_family_static(font, "Arial");
		pango_font_description_set_style(font, PANGO_STYLE_NORMAL);

		pango_layout_set_font_description(layout, font);
		pango_layout_set_text(layout, name, strlen(name));

		pango_layout_get_pixel_extents(layout, NULL, &logical_rect);

		switch (opr->place) {
			case CONTING_DRAWING_OPERATION_LABEL_TOP:
				pw0.x = MIN(bounds.x0, bounds.x1)
					+ fabs(bounds.x1 - bounds.x0) / 2.0
					- logical_rect.width / 2.0;
				pw0.y = MIN(bounds.y0, bounds.y1) - logical_rect.height;
				break;
			case CONTING_DRAWING_OPERATION_LABEL_BOTTOM:
				pw0.x = MIN(bounds.x0, bounds.x1)
					+ fabs(bounds.x1 - bounds.x0) / 2.0
					- logical_rect.width / 2.0;
				pw0.y = MAX(bounds.y0, bounds.y1);
				break;
		}
		cairo_move_to(cr, pw0.x, pw0.y);

		pango_cairo_update_layout(cr, layout);
		pango_cairo_show_layout(cr, layout);

	 	g_object_unref(layout);

/*	cairo_show_text(cr, name); */
	}
	cairo_stroke(cr);

	cairo_destroy(cr);
}

static void
conting_drawing_operation_label_get_property(GObject *self,
                             guint prop_id,
                             GValue *value,
                             GParamSpec *pspec)
{
    ContingDrawingOperationLabel *opr;

    g_return_if_fail(self != NULL && CONTING_IS_DRAWING_OPERATION(self));

    opr = (ContingDrawingOperationLabel *) self;

    switch (prop_id) {
        case CONTING_DRAWING_OPERATION_LABEL_PROP_ATTR:
            g_value_set_pointer(value, opr->label_func);
            break;
        case CONTING_DRAWING_OPERATION_LABEL_PROP_USER_DATA:
            g_value_set_pointer(value, opr->user_data);
            break;
        case CONTING_DRAWING_OPERATION_LABEL_PROP_TEXT:
            g_value_set_string(value, opr->text);
            break;
		case CONTING_DRAWING_OPERATION_LABEL_PROP_PLACE:
			g_value_set_int(value, opr->place);
			break;
		case CONTING_DRAWING_OPERATION_LABEL_PROP_COLOR:
			g_value_set_pointer(value, &opr->color);
			break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

static void
conting_drawing_operation_label_set_property(GObject *self,
                             guint prop_id,
                             const GValue *value,
                             GParamSpec *pspec)
{
    ContingDrawingOperationLabel *opr;

    g_return_if_fail(self != NULL && CONTING_IS_DRAWING_OPERATION(self));

	opr = (ContingDrawingOperationLabel *) self;

    switch (prop_id) {
        case CONTING_DRAWING_OPERATION_LABEL_PROP_ATTR:
            opr->label_func = g_value_get_pointer(value);
            break;
        case CONTING_DRAWING_OPERATION_LABEL_PROP_USER_DATA:
            opr->user_data = g_value_get_pointer(value);
            break;
        case CONTING_DRAWING_OPERATION_LABEL_PROP_TEXT:
            opr->text = g_value_get_string(value);
            break;
		case CONTING_DRAWING_OPERATION_LABEL_PROP_PLACE:
			opr->place = g_value_get_int(value);
			break;
		case CONTING_DRAWING_OPERATION_LABEL_PROP_COLOR:
			opr->color = *((GdkColor *) g_value_get_pointer(value));
			break;
		case CONTING_DRAWING_OPERATION_LABEL_PROP_COLOR_NAME:
			gdk_color_parse(g_value_get_string(value), &opr->color);
			break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}
static void
conting_drawing_operation_label_instance_init(GTypeInstance *self,
		gpointer g_class)
{
	ContingDrawingOperationLabel *opr = (ContingDrawingOperationLabel *) self;

	opr->label_func = NULL;
	opr->user_data = NULL;

	opr->text = NULL;

	gdk_color_parse("red", &opr->color);
}

static void
conting_drawing_operation_label_drawing_operation_init(gpointer g_iface,
		gpointer iface_data)
{
	ContingDrawingOperationClass *opr_class;

	opr_class = g_iface;
	opr_class->get_bounds = conting_drawing_operation_label_get_bounds;
	opr_class->draw = conting_drawing_operation_label_draw;
}

static void
conting_drawing_operation_label_class_init(gpointer g_class,
		gpointer class_data)
{
	GObjectClass *gobject_class;

	gobject_class = G_OBJECT_CLASS(g_class);
	gobject_class->set_property = conting_drawing_operation_label_set_property;
	gobject_class->get_property = conting_drawing_operation_label_get_property;

	g_object_class_install_property(gobject_class,
			CONTING_DRAWING_OPERATION_LABEL_PROP_ATTR,
			g_param_spec_pointer("label-func",
								 "Label generation function",
								 "Function which returns the text used",
								 G_PARAM_READABLE | G_PARAM_WRITABLE));

	g_object_class_install_property(gobject_class,
			CONTING_DRAWING_OPERATION_LABEL_PROP_USER_DATA,
			g_param_spec_pointer("user-data",
								 "User data function",
								 "user_data parameter",
								 G_PARAM_READABLE | G_PARAM_WRITABLE));

	g_object_class_install_property(gobject_class,
			CONTING_DRAWING_OPERATION_LABEL_PROP_TEXT,
			g_param_spec_string("text",
								"Label text",
								"default label text",
								"",
								G_PARAM_READABLE | G_PARAM_WRITABLE));

	/* too lazy to make it into an enum
	 * TODO: make it g_param_spec_enum*/
	g_object_class_install_property(gobject_class,
			CONTING_DRAWING_OPERATION_LABEL_PROP_PLACE,
			g_param_spec_int("place",
								"Label place",
								"default label text",
								0, 1, 0,
								G_PARAM_READABLE | G_PARAM_WRITABLE));
	
	g_object_class_install_property(gobject_class,
			CONTING_DRAWING_OPERATION_LABEL_PROP_COLOR,
			g_param_spec_pointer("color",
								 "Font color",
								 "Font color",
								 G_PARAM_READABLE | G_PARAM_WRITABLE));

	g_object_class_install_property(gobject_class,
			CONTING_DRAWING_OPERATION_LABEL_PROP_COLOR_NAME,
			g_param_spec_string("color-name",
								"Font color name",
								"The name of the color",
								"red",
								G_PARAM_WRITABLE));
	
}

GType
conting_drawing_operation_label_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo type_info = {
			sizeof(ContingDrawingOperationLabelClass),
			NULL,
			NULL,
			conting_drawing_operation_label_class_init,
			NULL,
			NULL,
			sizeof(ContingDrawingOperationLabel),
			0,
			conting_drawing_operation_label_instance_init,
			NULL,
		};

		static const GInterfaceInfo iface_info = {
			conting_drawing_operation_label_drawing_operation_init,
			NULL,
			NULL
		};

		type = g_type_register_static(G_TYPE_OBJECT,
				"ContingDrawingOperationLabel",
				&type_info, 0);

		g_type_add_interface_static(type,
				CONTING_TYPE_DRAWING_OPERATION,
				&iface_info);
	}

	return type;
}
ContingDrawingOperation *
conting_drawing_operation_label_new_with_func(ContingLabelFunc func,
        gpointer user_data)
{
    GObject *opr;

    opr = g_object_new(CONTING_TYPE_DRAWING_OPERATION_LABEL, NULL);

    g_object_set(opr,
            "label-func", func,
            "user-data", user_data,
            NULL);

    return CONTING_DRAWING_OPERATION(opr);

}
