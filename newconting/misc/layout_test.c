#include <gtk/gtk.h>
#include <stdlib.h>
#include <math.h>

static gdouble perc = 33.3;

static void
entry_activated(GtkEntry *entry, gpointer user_data)
{
	const gchar *text = gtk_entry_get_text(entry);

	perc = atoi(text);

	if (perc > 100)
		perc = 100;

	gtk_widget_queue_draw(GTK_WIDGET(user_data));
}

static gboolean
layout_expose_event(GtkWidget *widget, GdkEventExpose *event,
		gpointer user_data)
{
	cairo_t *cr;
	gdouble radius;
	gint x, y, w, h;
	char buf[256];

	cr = gdk_cairo_create(GTK_LAYOUT(widget)->bin_window);

	radius = MIN(widget->allocation.width / 2,
			widget->allocation.height / 2) - 5;

	cairo_arc(cr, widget->allocation.x + widget->allocation.width / 2,
			widget->allocation.y + widget->allocation.height / 2,
			radius, 0, 2 * M_PI);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_stroke(cr);

	x = 300;
	y = 200;
	w = 30;
	h = 100;

	cairo_move_to(cr, x, y + (h - (perc / 100.0) * h));
	cairo_line_to(cr, x, y + h);
	cairo_line_to(cr, x + w, y + h);
	cairo_line_to(cr, x + w, y + (h - (perc / 100.0) * h));
	cairo_line_to(cr, x, y + (h - (perc / 100.0) * h));
	cairo_set_source_rgb(cr,
			perc > 60 ? perc / 100.0 : perc / 150.0,
			0.2,
			perc > 60 ? 3.5 / perc : 20.0 / perc);

	cairo_fill_preserve(cr);
	cairo_stroke(cr);

	cairo_move_to(cr, x, y);
	cairo_line_to(cr, x, y + h);
	cairo_line_to(cr, x + w, y + h);
	cairo_line_to(cr, x + w, y);
	cairo_line_to(cr, x, y);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_stroke(cr);


	cairo_set_font_size(cr, 10.0 * (w / 30.0));
	cairo_move_to(cr,
			x + w / (perc < 100.0 ? (perc < 10 ? 3.5 : 5.0) : 13.0),
			y + h / 2.0);
	cairo_select_font_face(cr, "Arial",
			CAIRO_FONT_SLANT_NORMAL,
			CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_source_rgb(cr, 0, 0, 0);
	sprintf(buf, "%d%%", (int) perc);
	cairo_show_text(cr, buf);
	cairo_stroke(cr);

	cairo_destroy(cr);

	return FALSE;
}

int main(int argc, char *argv[])
{
	GtkWidget *window, *layout, *entry;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(GTK_WIDGET(window), 400, 400);

	layout = gtk_layout_new(NULL, NULL);
	g_signal_connect(G_OBJECT(layout), "expose-event",
			G_CALLBACK(layout_expose_event), NULL);

	entry = gtk_entry_new();
	g_signal_connect(G_OBJECT(entry), "activate",
			G_CALLBACK(entry_activated), layout);
	gtk_layout_put(GTK_LAYOUT(layout), entry,
			100, 100);

	gtk_container_add(GTK_CONTAINER(window), layout);
	gtk_widget_show(layout);

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
