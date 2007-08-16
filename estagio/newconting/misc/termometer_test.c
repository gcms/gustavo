#include "contingtermometer.h"

static gboolean
term_scrolled(GtkWidget *widget, GdkEventScroll *event,
		gpointer user_data)
{
	ContingTermometer *term;
	gdouble val;

	term = CONTING_TERMOMETER(widget);

	val = conting_termometer_get_value(term);

	switch (event->direction) {
		case GDK_SCROLL_UP:
			val = MIN(val + 1, 100.0);
			break;
		case GDK_SCROLL_DOWN:
			val = MAX(val - 1, 0.0);
			break;
		default:
			return FALSE;
	}

	conting_termometer_set_value(term, val);

	return TRUE;
}

GtkWidget *window, *vbox, *hbox, *hbox2, *label, *termometer;

static void
window_realized(GtkWidget *widget, gpointer user_data)
{
	GtkRequisition req;

	gtk_widget_add_events(widget, GDK_SCROLL_MASK | GDK_EXPOSURE_MASK);
	

	gtk_widget_size_request(hbox, &req);
	printf("realized hbox request (%d, %d)\n",
			req.width, req.height);

	gtk_widget_set_size_request(label, req.width, -1);

	gtk_window_move(GTK_WINDOW(window), 30, 30);
}


static gboolean
window_exposed(GtkWidget *widget, GdkEventExpose *event, gpointer user_data)
{
	g_print("EXPOSED\n");
	/*
	gtk_paint_box(widget->style, widget->window, GTK_STATE_NORMAL,
			GTK_SHADOW_NONE, &event->area, widget, NULL,
			0, 0, widget->allocation.width, widget->allocation.height);
			*/
	gdk_draw_rectangle(widget->window, widget->style->black_gc,
			FALSE, 0, 0,
			widget->allocation.width - 1, widget->allocation.height - 1);

	return FALSE;
}


int main(int argc, char *argv[])
{
	GtkRequisition req;
	GdkColor color;

	gtk_init(&argc, &argv);

	gdk_color_parse("beige", &color);

	window = gtk_window_new(GTK_WINDOW_POPUP);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);
	g_signal_connect(G_OBJECT(window), "realize",
			G_CALLBACK(window_realized), NULL);
	g_signal_connect_after(G_OBJECT(window), "expose-event",
			G_CALLBACK(window_exposed), NULL);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	label = gtk_label_new("This is a label, blah, blah asdf, ehehe, hello, it's nice");
	gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 20);
	gtk_container_set_border_width(GTK_CONTAINER(hbox), 20);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 4);

	termometer = conting_termometer_new(0);
	gtk_widget_set_size_request(termometer, 30, 100);
	g_signal_connect(G_OBJECT(termometer), "scroll-event",
			G_CALLBACK(term_scrolled), NULL);
	gtk_box_pack_start(GTK_BOX(hbox), termometer, FALSE, FALSE, 0);

	termometer = conting_termometer_new(30);
	gtk_widget_set_size_request(termometer, 30, 100);
	g_signal_connect_after(G_OBJECT(termometer), "scroll-event",
			G_CALLBACK(term_scrolled), NULL);
	gtk_box_pack_start(GTK_BOX(hbox), termometer, FALSE, FALSE, 0);

	gtk_widget_show(hbox);
	gtk_widget_size_request(hbox, &req);
	printf("hbox request (%d, %d)\n",
			req.width, req.height);

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
