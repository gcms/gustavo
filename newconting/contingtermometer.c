#include "contingtermometer.h"

#define CONTING_TERMOMETER_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE((o), \
            CONTING_TYPE_TERMOMETER, ContingTermometerPrivate))

enum {
	CONTING_TERMOMETER_PROP_0,
	CONTING_TERMOMETER_PROP_VALUE
};

typedef struct _ContingTermometerPrivate ContingTermometerPrivate;
struct _ContingTermometerPrivate
{
	gdouble value;
};
static void
conting_termometer_realize(GtkWidget *self)
{
	GdkWindowAttr attr;
	gint attr_mask;

	g_return_if_fail(self != NULL && CONTING_IS_TERMOMETER(self));

	GTK_WIDGET_SET_FLAGS(self, GTK_REALIZED);

	attr.x = self->allocation.x;
	attr.y = self->allocation.y;
	attr.width = self->allocation.width;
	attr.height = self->allocation.height;
	attr.wclass = GDK_INPUT_OUTPUT;
	attr.window_type = GDK_WINDOW_CHILD;
	attr.event_mask = gtk_widget_get_events(self)
		| GDK_EXPOSURE_MASK | GDK_SCROLL_MASK;
	attr.visual = gtk_widget_get_visual(self);
	attr.colormap = gtk_widget_get_colormap(self);

	attr_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
	self->window = gdk_window_new(self->parent->window, &attr, attr_mask);
	self->style = gtk_style_attach(self->style, self->window);

	gdk_window_set_user_data(self->window, self);

	gtk_style_set_background(self->style, self->window, GTK_STATE_ACTIVE);
}

static void
conting_termometer_get_property(GObject *self, guint prop_id,
		GValue *value, GParamSpec *pspec)
{
	ContingTermometerPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_TERMOMETER(self));

	priv = CONTING_TERMOMETER_GET_PRIVATE(self);

	switch (prop_id) {
		case CONTING_TERMOMETER_PROP_VALUE:
			g_value_set_double(value, priv->value);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
			break;
	}
}

static void
conting_termometer_set_property(GObject *self, guint prop_id,
		const GValue *value, GParamSpec *pspec)
{
	ContingTermometerPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_TERMOMETER(self));

	priv = CONTING_TERMOMETER_GET_PRIVATE(self);

	switch (prop_id) {
		case CONTING_TERMOMETER_PROP_VALUE:
			priv->value = g_value_get_double(value);
			gtk_widget_queue_draw(GTK_WIDGET(self));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
			break;
	}
}

static void
conting_termometer_size_request(GtkWidget *self, GtkRequisition *req)
{
	req->width = 30;
	req->height = 100;
}
static void
conting_termometer_size_allocate(GtkWidget *self, GtkAllocation *allocation)
{

	g_return_if_fail(self != NULL && CONTING_IS_TERMOMETER(self));

	self->allocation = *allocation;

	if (GTK_WIDGET_REALIZED(self)) {
		gdk_window_move_resize(self->window,
				allocation->x, allocation->y,
				allocation->width, allocation->height);
	}
}

static gboolean
conting_termometer_expose_event(GtkWidget *self, GdkEventExpose *event)
{
	ContingTermometerPrivate *priv;
	cairo_t *cr;
	gdouble x, y, w, h;
	gchar buf[256];

	g_return_val_if_fail(self != NULL && CONTING_IS_TERMOMETER(self), FALSE);

	priv = CONTING_TERMOMETER_GET_PRIVATE(self);

	cr = gdk_cairo_create(self->window);
	x = y = 0;
	/*
	w = self->allocation.x + self->allocation.width;
	h = self->allocation.y + self->allocation.height;
//	x = self->allocation.x;
//	y = self->allocation.y;
	*/
	w = self->allocation.width;
	h = self->allocation.height;

	gdk_draw_rectangle(self->window, self->style->bg_gc[GTK_STATE_NORMAL],
			TRUE, x, y, w, h);


	cairo_move_to(cr, x, y + (h - (priv->value / 100.0) * h));
	cairo_line_to(cr, x, y + h);
	cairo_line_to(cr, x + w, y + h);
	cairo_line_to(cr, x + w, y + (h - (priv->value / 100.0) * h));
	cairo_line_to(cr, x, y + (h - (priv->value / 100.0) * h));
	cairo_set_source_rgb(cr,
			priv->value > 60 ? priv->value / 100.0 : priv->value / 150.0,
			0.2,
			priv->value > 60 ? 3.5 / priv->value : 20.0 / priv->value);

	cairo_fill_preserve(cr);
	cairo_stroke(cr);

	cairo_move_to(cr, x, y);
	cairo_set_line_width(cr, MAX(1.0, w / 30.0));
	cairo_line_to(cr, x, y + h);
	cairo_line_to(cr, x + w, y + h);
	cairo_line_to(cr, x + w, y);
	cairo_line_to(cr, x, y);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_stroke(cr);


	cairo_set_font_size(cr, 10.0 * (w / 30.0));
	cairo_move_to(cr,
			x + w / (priv->value < 100.0 ?
				(priv->value < 10 ? 3.5 : 5.0) : 13.0),
			y + h / 2.0 + w / 6.0);
	cairo_select_font_face(cr, "Arial",
			CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_source_rgb(cr, 0, 0, 0);
	sprintf(buf, "%d%%", (gint) priv->value);
	cairo_show_text(cr, buf);
	cairo_stroke(cr);

	cairo_destroy(cr);

	return FALSE;
}

static void
conting_termometer_instance_init(GTypeInstance *self, gpointer g_class)
{
	ContingTermometerPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_TERMOMETER(self));

	priv = CONTING_TERMOMETER_GET_PRIVATE(self);

	priv->value = 0;
}

static void
conting_termometer_class_init(gpointer g_class, gpointer class_data)
{
	GObjectClass *object_class;
	GtkWidgetClass *widget_class;

	object_class = G_OBJECT_CLASS(g_class);
	object_class->set_property = conting_termometer_set_property;
	object_class->get_property = conting_termometer_get_property;

	widget_class = GTK_WIDGET_CLASS(g_class);
	widget_class->realize = conting_termometer_realize;
	widget_class->size_request = conting_termometer_size_request;
	widget_class->size_allocate = conting_termometer_size_allocate;
	widget_class->expose_event = conting_termometer_expose_event;

	g_object_class_install_property(object_class,
			CONTING_TERMOMETER_PROP_VALUE,
			g_param_spec_double("value",
							    "Percentage value",
								"The value of the termometer",
								0.0,
								100.0,
								50.0,
								G_PARAM_READABLE | G_PARAM_WRITABLE));

	g_type_class_add_private(g_class, sizeof(ContingTermometerPrivate));
}

GType
conting_termometer_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static GTypeInfo type_info = {
			sizeof(ContingTermometerClass),
			NULL,
			NULL,
			conting_termometer_class_init,
			NULL,
			NULL,
			sizeof(ContingTermometer),
			0,
			conting_termometer_instance_init,
			NULL
		};

		type = g_type_register_static(GTK_TYPE_WIDGET,
				"ContingTermometer", &type_info, 0);
	}

	return type;
}

GtkWidget *
conting_termometer_new(gdouble value)
{
	GObject *self;

	self = g_object_new(CONTING_TYPE_TERMOMETER,
			"value", value,
			NULL);

	return GTK_WIDGET(self);
}

void
conting_termometer_set_value(ContingTermometer *self, gdouble value)
{
	g_return_if_fail(self != NULL && CONTING_IS_TERMOMETER(self));

	g_object_set(G_OBJECT(self),
			"value", value,
			NULL);
}

gdouble
conting_termometer_get_value(ContingTermometer *self)
{
	gdouble value;

	g_return_val_if_fail(self != NULL && CONTING_IS_TERMOMETER(self), -1);

	g_object_get(G_OBJECT(self),
			"value", &value,
			NULL);

	return value;
}
