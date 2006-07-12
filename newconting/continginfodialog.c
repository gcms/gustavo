#include "continginfodialog.h"
#include "contingdata.h"

static gpointer parent_class = NULL;

enum {
    CONTING_INFO_DIALOG_PROP_0,
    CONTING_INFO_DIALOG_PROP_DATA,
    CONTING_INFO_DIALOG_PROP_DRAWING
};

#define CONTING_INFO_DIALOG_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE((o), \
            CONTING_TYPE_INFO_DIALOG, ContingInfoDialogPrivate))

typedef struct ContingInfoDialogPrivate_ ContingInfoDialogPrivate;
struct ContingInfoDialogPrivate_ {
    ContingData *data;
    ContingDrawing *drawing;

	GtkWidget *bus_view;
	GtkWidget *bus_info;
};
static void
conting_info_dialog_get_property(GObject *self,
                             guint prop_id,
                             GValue *value,
                             GParamSpec *pspec)
{
    ContingInfoDialogPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_INFO_DIALOG(self));

    priv = CONTING_INFO_DIALOG_GET_PRIVATE(self);

    switch (prop_id) {
        case CONTING_INFO_DIALOG_PROP_DATA:
            g_value_set_object(value, priv->data);
            break;
        case CONTING_INFO_DIALOG_PROP_DRAWING:
            g_value_set_object(value, priv->drawing);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

static void
conting_info_dialog_set_property(GObject *self,
                             guint prop_id,
                             const GValue *value,
                             GParamSpec *pspec)
{
    ContingInfoDialogPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_INFO_DIALOG(self));

    priv = CONTING_INFO_DIALOG_GET_PRIVATE(self);

    switch (prop_id) {
        case CONTING_INFO_DIALOG_PROP_DATA:
            priv->data = g_value_get_object(value);
            break;
        case CONTING_INFO_DIALOG_PROP_DRAWING:
            priv->drawing = g_value_get_object(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

static void
conting_info_dialog_create_widgets(ContingInfoDialog *self)
{
	ContingInfoDialogPrivate *priv;
	GtkWidget *hbox;
	gchar buf[256];

	g_return_if_fail(self != NULL && CONTING_IS_INFO_DIALOG(self));

	priv = CONTING_INFO_DIALOG_GET_PRIVATE(self);


	sprintf(buf, "%p %s Information",
			priv->drawing, g_type_name(G_OBJECT_TYPE(priv->drawing)));
	gtk_window_set_title(GTK_WINDOW(self), buf);

	gtk_widget_push_composite_child();

	hbox = gtk_hbox_new(TRUE, TRUE);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(self)->vbox), hbox);

	priv->bus_view = gtk_label_new(NULL);
	sprintf(buf, "<u>%p</u>", priv->drawing);
	gtk_label_set_markup(GTK_LABEL(priv->bus_view),  buf);
	gtk_box_pack_start(GTK_BOX(hbox), priv->bus_view,
			TRUE, TRUE, 0);

	priv->bus_info = gtk_label_new(NULL);
	sprintf(buf, "<u>%s</u>", g_type_name(G_OBJECT_TYPE(priv->drawing)));
	gtk_label_set_markup(GTK_LABEL(priv->bus_info), buf);
	gtk_box_pack_start(GTK_BOX(hbox), priv->bus_info,
			TRUE, TRUE, 0);

	
	gtk_widget_pop_composite_child();
}

static GObject *
conting_info_dialog_constructor(GType type,
		                        guint n_construct_props,
								GObjectConstructParam *construct_param)
{
	GObject *self;

	self = G_OBJECT_CLASS(parent_class)->constructor(type,
			n_construct_props,
			construct_param);

	conting_info_dialog_create_widgets(CONTING_INFO_DIALOG(self));

	return self;
}

static void
conting_info_dialog_response_cb(GtkDialog *self, gint response_id)
{
    if (response_id == GTK_RESPONSE_OK) {
		g_print("OK\n");
	} else {
		g_print("Cancel\n");
	}

	gtk_widget_destroy(GTK_WIDGET(self));
}

static void
conting_info_dialog_class_init(gpointer g_class, gpointer class_data)
{
    GObjectClass *gobject_class;

    gobject_class = G_OBJECT_CLASS(g_class);
	gobject_class->constructor = conting_info_dialog_constructor;
    gobject_class->set_property = conting_info_dialog_set_property;
    gobject_class->get_property = conting_info_dialog_get_property;


    g_object_class_install_property(gobject_class,
            CONTING_INFO_DIALOG_PROP_DATA,
            g_param_spec_object("data",
                                "ContingData",
                                "ContingData associated with the drawing",
                                CONTING_TYPE_DATA,
                                G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY
                                | G_PARAM_WRITABLE));
                                
    g_object_class_install_property(gobject_class,
            CONTING_INFO_DIALOG_PROP_DRAWING,
            g_param_spec_object("drawing",
                                "ContingDrawing",
                                "ContingDrawing associated",
                                CONTING_TYPE_DRAWING,
                                G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY
                                | G_PARAM_WRITABLE));

	parent_class = g_type_class_peek_parent(g_class);

    g_type_class_add_private(g_class, sizeof(ContingInfoDialogPrivate));
}

static void
conting_info_dialog_instance_init(GTypeInstance *self, gpointer g_class)
{
    ContingInfoDialogPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_INFO_DIALOG(self));

    priv = CONTING_INFO_DIALOG_GET_PRIVATE(self);

    priv->data = NULL;
    priv->drawing = NULL;

	gtk_dialog_set_has_separator(GTK_DIALOG(self), FALSE);

	g_signal_connect(G_OBJECT(self), "response",
			G_CALLBACK(conting_info_dialog_response_cb), NULL);
}


GType
conting_info_dialog_get_type(void)
{
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingInfoDialogClass),
            NULL,
            NULL,
            conting_info_dialog_class_init,
            NULL,
            NULL,
            sizeof(ContingInfoDialog),
            0,
            conting_info_dialog_instance_init,
            NULL
        };

        type = g_type_register_static(GTK_TYPE_DIALOG, "ContingInfoDialog",
                &type_info, 0);
    }

    return type;
}


GtkWidget *
conting_info_dialog_new(ContingData *data, ContingDrawing *drawing)
{
	GObject *self;

	self = g_object_new(CONTING_TYPE_INFO_DIALOG,
			"data", data,
			"drawing", drawing,
			NULL);

	gtk_dialog_add_button(GTK_DIALOG(self),
			GTK_STOCK_OK, GTK_RESPONSE_OK);
	gtk_dialog_add_button(GTK_DIALOG(self),
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);

	return GTK_WIDGET(self);
}
