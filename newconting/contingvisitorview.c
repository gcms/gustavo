#include "contingvisitor.h"
#include "contingvisitorview.h"
#include <assert.h>

static gpointer parent_class = NULL;

static GtkDialog *
get_dialog(const gchar *title, ContingOneLine *oneline)
{
	GtkWidget *dialog;

	dialog = gtk_dialog_new_with_buttons(title, NULL, GTK_DIALOG_MODAL,
			GTK_STOCK_OK, GTK_RESPONSE_OK,
			NULL);

	g_signal_connect_swapped(dialog, "response",
			G_CALLBACK(gtk_widget_destroy), dialog);

	gtk_window_set_transient_for(GTK_WINDOW(dialog),
			GTK_WINDOW(gtk_widget_get_toplevel(
					conting_one_line_get_widget(oneline))));
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);
	gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);

	return GTK_DIALOG(dialog);
}


static void
conting_visitor_view_visit_trans2(ContingVisitor *self, ContingTrans2 *trans2)
{
	GtkDialog *dialog;

	gchar text[256];
	GtkWidget *label;
	
	ContingItemData *item_data;
	gdouble tap = 0, min_tap = 0, max_tap = 0;


	dialog = get_dialog("Transformador",
			conting_drawing_get_one_line(CONTING_DRAWING(trans2)));

	item_data = conting_drawing_get_item_data(CONTING_DRAWING(trans2));

	conting_item_data_get_attr(item_data,
			"voltage ratio", &tap,
			"min tap", &min_tap,
			"max tap", &max_tap,
			NULL);

	sprintf(text, "Tap: %lf", tap);
	label = gtk_label_new(text);
	gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

	if (max_tap != 0 || min_tap != 0) {
		sprintf(text, "Tap minimo: %lf", min_tap);
		label = gtk_label_new(text);
		gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);
		
		sprintf(text, "Tap maximo: %lf", max_tap);
		label = gtk_label_new(text);
		gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);
	}

	gtk_widget_show_all(GTK_WIDGET(dialog));
}

static void
conting_visitor_view_visit_base(ContingVisitor *visitor,
		ContingDrawing *drawing)
{
}

static void
conting_visitor_view_visitor_init(gpointer g_iface,
		gpointer iface_data)
{
	ContingVisitorClass *visitor_class;

	visitor_class = g_iface;

	visitor_class->visit_line = (gpointer) conting_visitor_view_visit_base;
	visitor_class->visit_bus = (gpointer) conting_visitor_view_visit_base;
	visitor_class->visit_load = (gpointer) conting_visitor_view_visit_base;
	visitor_class->visit_gen = (gpointer) conting_visitor_view_visit_base;
	
	visitor_class->visit_trans2 = conting_visitor_view_visit_trans2;
	visitor_class->visit_trans3 = (gpointer) conting_visitor_view_visit_base;
}

static void
conting_visitor_view_instance_init(GTypeInstance *self, gpointer g_class)
{
}

static void
conting_visitor_view_class_init(gpointer g_class, gpointer class_data)
{
	parent_class = g_type_class_peek_parent(g_class);
}

GType
conting_visitor_view_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo type_info = {
			sizeof(ContingVisitorViewClass),
			NULL,
			NULL,
			conting_visitor_view_class_init,
			NULL,
			NULL,
			sizeof(ContingVisitorView),
			0,
			conting_visitor_view_instance_init,
			NULL,
		};

		static const GInterfaceInfo iface_info = {
			conting_visitor_view_visitor_init,
			NULL,
			NULL
		};

		type = g_type_register_static(G_TYPE_OBJECT,
				"ContingVisitorView",
				&type_info, 0);

		g_type_add_interface_static(type,
				CONTING_TYPE_VISITOR,
				&iface_info);
	}

	return type;
}
