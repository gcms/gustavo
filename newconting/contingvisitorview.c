#include "contingvisitor.h"
#include "contingvisitorview.h"
#include "contingmodel.h"
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

	gtk_box_set_spacing(GTK_BOX(GTK_DIALOG(dialog)->vbox), 5);

	return GTK_DIALOG(dialog);
}


static void
conting_visitor_view_visit_trans2(ContingVisitor *self, ContingTrans2 *trans2)
{
	GtkDialog *dialog;

	gchar text[256];
	GtkWidget *label;
	
	ContingItemData *item_data, *flow_data;
	ContingModel *model;
	gdouble tap = 0, min_tap = 0, max_tap = 0;
	gint x, y;


	dialog = get_dialog("Transformador",
			conting_drawing_get_one_line(CONTING_DRAWING(trans2)));

	item_data = conting_drawing_get_item_data(CONTING_DRAWING(trans2));

	conting_item_data_get_attr(item_data,
			"voltage ratio", &tap,
			"min tap", &min_tap,
			"max tap", &max_tap,
			"tap bus number", &x,
			"z bus number", &y,
			NULL);

	model = conting_one_line_load_flow_model(conting_drawing_get_one_line(
				CONTING_DRAWING(trans2)));
	if (model != NULL && CONTING_IS_MODEL(model)) {
		flow_data = conting_model_get_item(model, CONTING_ITEM_TYPE_FLOW_TRANS,
				conting_item_branch_key_new(x, y));
		if (flow_data != NULL && CONTING_IS_ITEM_DATA(flow_data)) {
			gdouble init, inf, sup, final;

			conting_item_data_get_attr(flow_data,
					"initial", &init,
					"min", &inf,
					"max", &sup,
					"final", &final,
					NULL);

			label = gtk_label_new(NULL);
			gtk_label_set_markup(GTK_LABEL(label),
					"<b>Valores dos Taps</b>");
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

			sprintf(text, "Valor inicial: %lf", init);
			label = gtk_label_new(text);
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

			sprintf(text, "Limite inferior: %lf", inf);
			label = gtk_label_new(text);
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

			sprintf(text, "Limite superior: %lf", sup);
			label = gtk_label_new(text);
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

			sprintf(text, "Valor final: %lf", final);
			label = gtk_label_new(text);
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);
		}
	}

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
conting_visitor_view_visit_bus(ContingVisitor *visitor,
		ContingBus *bus)
{
	GtkDialog *dialog;
	ContingOneLine *oneline;

	gchar text[256];
	GtkWidget *label;
	
	ContingItemData *item_data, *flow_data;
	ContingModel *model;

	oneline = conting_drawing_get_one_line(CONTING_DRAWING(bus));
	item_data = conting_drawing_get_item_data(CONTING_DRAWING(bus));

	model = conting_one_line_load_flow_model(oneline);
	if (model != NULL && CONTING_IS_MODEL(model)) {
		flow_data = conting_model_get_item(model, CONTING_ITEM_TYPE_FLOW_BUS,
				GINT_TO_POINTER(conting_item_data_bus_number(item_data)));
		if (flow_data != NULL && CONTING_IS_ITEM_DATA(flow_data)) {
			const gchar *name;
			gint number;
			gdouble base_voltage, voltage, angle;

			conting_item_data_get_attr(item_data,
					"name", &name,
					"voltage", &base_voltage,
					NULL);

			conting_item_data_get_attr(flow_data,
					"number", &number,
					"voltage", &voltage,
					"angle", &angle,
					NULL);

			dialog = get_dialog("Barra", oneline);

			sprintf(text, "<b>Barra %d</b>", number);
			label = gtk_label_new(NULL);
			gtk_label_set_markup(GTK_LABEL(label), text);
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

			sprintf(text, "<b>%s</b>", name);
			label = gtk_label_new(NULL);
			gtk_label_set_markup(GTK_LABEL(label), text);
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

			sprintf(text, "Módulo: %.4f kV", base_voltage * voltage);
			label = gtk_label_new(text);
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

		
			sprintf(text, "Angulo: %.4f", angle);
			label = gtk_label_new(text);
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

			gtk_widget_show_all(GTK_WIDGET(dialog));
		}
	}
}
static void
conting_visitor_view_visit_gen(ContingVisitor *visitor,
		ContingGen *gen)
{
	GtkDialog *dialog;
	ContingOneLine *oneline;

	gchar text[256];
	GtkWidget *label;
	
	ContingItemData *item_data, *flow_data;
	ContingModel *model;

	oneline = conting_drawing_get_one_line(CONTING_DRAWING(gen));
	item_data = conting_drawing_get_item_data(CONTING_DRAWING(gen));

	model = conting_one_line_load_flow_model(oneline);
	if (model != NULL && CONTING_IS_MODEL(model)) {
		flow_data = conting_model_get_item(model, CONTING_ITEM_TYPE_FLOW_BUS,
				GINT_TO_POINTER(conting_item_data_bus_number(item_data)));
		if (flow_data != NULL && CONTING_IS_ITEM_DATA(flow_data)) {
			gdouble active, reactive, li, ls;

			conting_item_data_get_attr(flow_data,
					"gen-active", &active,
					"gen-reactive", &reactive,
					"li", &li,
					"ls", &ls,
					NULL);

			dialog = get_dialog("Gerador", oneline);

			label = gtk_label_new(NULL);
			gtk_label_set_markup(GTK_LABEL(label), "<b>Geração</b>");
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

			sprintf(text, "Ativa: %.4f MW", active);
			label = gtk_label_new(text);
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

		
			sprintf(text, "Reativa: %.4f MVAr", reactive);
			label = gtk_label_new(text);
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

			sprintf(text, "Limite inferior: %.4f MVAr", li);
			label = gtk_label_new(text);
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

			sprintf(text, "Limite superior: %.4f MVAr", ls);
			label = gtk_label_new(text);
			gtk_box_pack_start(GTK_BOX(dialog->vbox), label, TRUE, TRUE, 0);

			gtk_widget_show_all(GTK_WIDGET(dialog));
		}
	}
}

static void
conting_visitor_view_visitor_init(gpointer g_iface,
		gpointer iface_data)
{
	ContingVisitorClass *visitor_class;

	visitor_class = g_iface;

	visitor_class->visit_line = (gpointer) conting_visitor_view_visit_base;
	visitor_class->visit_bus = conting_visitor_view_visit_bus;
	visitor_class->visit_load = (gpointer) conting_visitor_view_visit_base;
	visitor_class->visit_gen = conting_visitor_view_visit_gen;
	
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
