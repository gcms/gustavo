#include "continginfodialog.h"
#include "contingbus.h"
#include "contingdata.h"

#include <assert.h>

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
	ContingItemData *prev_assoc;

	GtkWidget *bus_view;
	GtkWidget *bus_info;
};
static void
conting_info_dialog_create_info(ContingInfoDialog *self);

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
tree_cell_data_func(GtkTreeViewColumn *tree_column, GtkCellRenderer *cell,
		GtkTreeModel *tree_model, GtkTreeIter *iter, gpointer user_data)
{
	ContingItemData *data;
	ContingItemType item_type;
	const gchar *busname;

	gtk_tree_model_get(tree_model, iter,
			0, &data,
			-1);

	if (data == NULL) {
		g_object_set(G_OBJECT(cell),
				"text", "(None)",
				NULL);
		return;
	}

	g_object_get(G_OBJECT(data),
			"type", &item_type,
			NULL);

	assert(item_type == CONTING_ITEM_TYPE_BUS);

	g_return_if_fail(cell != NULL && GTK_IS_CELL_RENDERER_TEXT(cell));

	conting_item_data_get_attr(data,
			"name", &busname,
			NULL);

	g_object_set(G_OBJECT(cell),
			"text", busname,
			NULL);
	
}
static void
tree_selection_changed_cb (GtkTreeSelection *selection, gpointer user_data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
    ContingItemData *data;
	ContingInfoDialog *self = user_data;
	ContingInfoDialogPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_INFO_DIALOG(self));

	priv = CONTING_INFO_DIALOG_GET_PRIVATE(self);

    if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
    	gtk_tree_model_get (model, &iter, 0, &data, -1);

		conting_data_assoc(priv->data, priv->drawing, data);

		conting_info_dialog_create_info(self);
	}

}
static void
attrs_foreach(const gchar *key, const GValue *value, gpointer vbox)
{
	GtkWidget *label;
	char buf[256], result[256];

	switch (G_VALUE_TYPE(value)) {
		case G_TYPE_INT:
			sprintf(buf, "%d", g_value_get_int(value));
			break;
		case G_TYPE_DOUBLE:
			sprintf(buf, "%lf", g_value_get_double(value));
			break;
		case G_TYPE_STRING:
			sprintf(buf, "%s", g_value_get_string(value));
			break;
		case G_TYPE_POINTER:
		default:
			sprintf(buf, "%p", g_value_get_pointer(value));
			break;
	}

	sprintf(result, "<u>%s</u> = <b>%s</b>", (gchar *) key, buf);

	label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label), result);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 1);
	gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);

	gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 4);
}

static void
conting_info_dialog_create_info(ContingInfoDialog *self)
{
	ContingInfoDialogPrivate *priv;
	GtkWidget *vbox;
	GtkWidget *child;
	ContingItemData *data;

	g_return_if_fail(self != NULL && CONTING_IS_INFO_DIALOG(self));

	priv = CONTING_INFO_DIALOG_GET_PRIVATE(self);

	vbox = gtk_vbox_new(FALSE, 0);

	data = conting_data_get(priv->data, priv->drawing);

	if (data) {
		conting_item_data_attr_foreach(data, attrs_foreach, vbox);
	}

	gtk_widget_show_all(vbox);

	child = gtk_bin_get_child(GTK_BIN(priv->bus_info));

	if (child)
		gtk_container_remove(GTK_CONTAINER(priv->bus_info), child);

	gtk_container_add(GTK_CONTAINER(priv->bus_info), vbox);
}

static void
conting_info_dialog_create_widgets(ContingInfoDialog *self)
{
	ContingInfoDialogPrivate *priv;
	GtkWidget *hbox, *swin;
	gchar buf[256];

	GtkListStore *store;
	GtkTreeIter iter, select_iter;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *select;
	GList *n, *unassoc;

	g_return_if_fail(self != NULL && CONTING_IS_INFO_DIALOG(self));

	priv = CONTING_INFO_DIALOG_GET_PRIVATE(self);

	/* filling store */
	store = gtk_list_store_new(1, G_TYPE_POINTER);

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter,
			0, NULL,
			-1);

	select_iter = iter;
	if (priv->prev_assoc) {
		gtk_list_store_append(store, &select_iter);
		gtk_list_store_set(store, &select_iter,
				0, priv->prev_assoc,
				-1);
	}

	unassoc = conting_data_get_unassoc(priv->data);
	g_print("g_list_next)\n");
	for (n = unassoc; n != NULL; n = g_list_next(n)) {
		ContingItemData *data = n->data;
		ContingItemType item_type;

		g_object_get(G_OBJECT(data),
				"type", &item_type,
				NULL);

		if (item_type != CONTING_ITEM_TYPE_BUS) {
			continue;
		}

		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter,
				0, data,
				-1);
	}

	g_print("g_list_free()\n");
	g_list_free(unassoc);


	sprintf(buf, "%p %s Information",
			priv->drawing, g_type_name(G_OBJECT_TYPE(priv->drawing)));
	gtk_window_set_title(GTK_WINDOW(self), buf);

	gtk_widget_push_composite_child();

	hbox = gtk_hbox_new(TRUE, TRUE);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(self)->vbox), hbox);

	if (!CONTING_IS_BUS_BASE(priv->drawing)) {
		goto DRAWING_INFO;
	}

	/* List View creation */
	priv->bus_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	g_object_unref(G_OBJECT(store));	/* view holds a reference */
	
	/* Adding column */
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Bus", renderer, NULL);
	gtk_tree_view_column_set_cell_data_func(column, renderer,
			tree_cell_data_func, NULL, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(priv->bus_view), column);

	/* handling selection */
	select = gtk_tree_view_get_selection(GTK_TREE_VIEW(priv->bus_view));
	gtk_tree_selection_unselect_all(select);
	gtk_tree_selection_set_mode(select, GTK_SELECTION_SINGLE);
	gtk_tree_selection_select_iter(select, &select_iter);
	g_signal_connect(G_OBJECT(select), "changed",
			G_CALLBACK(tree_selection_changed_cb), self);

	gtk_widget_set_size_request(GTK_WIDGET(priv->bus_view), 100, 200);
			
	/* Adding scrollbars */
	swin = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(swin), priv->bus_view);

	gtk_box_pack_start(GTK_BOX(hbox), swin,
			TRUE, TRUE, 0);



DRAWING_INFO:
	priv->bus_info = gtk_frame_new("Attributes");
	conting_info_dialog_create_info(self);

	gtk_box_pack_start(GTK_BOX(hbox), priv->bus_info,
			TRUE, TRUE, 0);


	gtk_widget_show_all(hbox);
	
	gtk_widget_pop_composite_child();
}

static GObject *
conting_info_dialog_constructor(GType type,
		                        guint n_construct_props,
								GObjectConstructParam *construct_param)
{
	GObject *self;
	ContingInfoDialogPrivate *priv;

	self = G_OBJECT_CLASS(parent_class)->constructor(type,
			n_construct_props,
			construct_param);

	priv = CONTING_INFO_DIALOG_GET_PRIVATE(self);

	priv->prev_assoc = conting_data_get(priv->data, priv->drawing);

	conting_info_dialog_create_widgets(CONTING_INFO_DIALOG(self));

	return self;
}

static void
conting_info_dialog_response_cb(GtkDialog *self, gint response_id,
		gpointer user_data)
{
	ContingInfoDialogPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_INFO_DIALOG(self));

	priv = CONTING_INFO_DIALOG_GET_PRIVATE(self);

	if (!CONTING_IS_BUS(priv->drawing))
		goto DESTROY;

    if (response_id == GTK_RESPONSE_CANCEL) {
		conting_data_assoc(priv->data, priv->drawing, priv->prev_assoc);
	}

DESTROY:
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
	priv->prev_assoc = NULL;

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
