#include <gtk/gtk.h>
#include "contingdrawing.h"
#include "contingdrawingbus.h"
#include "contingdrawingline.h"

static GSList *drawings = NULL;

static ContingDrawing *current_drawing = NULL;
static GdkPoint current_position = { -1, -1 };
static gboolean current_drawing_start = TRUE;

static ContingDrawing *selected_drawing = NULL;
static GdkPoint selected_position = { -1, -1 };

typedef struct DrawingToDraw_ DrawingToDraw;
struct DrawingToDraw_ {
	ContingDrawing *drawing;
	GdkPoint position;
};

static void select(DrawingToDraw *dd, GtkWidget *widget) {
	selected_drawing = dd->drawing;
	selected_position = dd->position;

	gtk_widget_queue_draw(widget);
}

static void darea_realize(GtkWidget *widget, gpointer user_data) {
	gtk_widget_add_events(widget, GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK
			| GDK_POINTER_MOTION_MASK);
}

static gboolean darea_expose_event(GtkWidget *widget, GdkEventExpose *event,
		gpointer user_data) {
	gint w, h;
	GSList *n;

	gdk_drawable_get_size(widget->window, &w, &h);
	gdk_draw_rectangle(widget->window, widget->style->white_gc, TRUE,
			0, 0, w, h);

	for (n = drawings; n != NULL; n = g_slist_next(n)) {
		DrawingToDraw *dd = n->data;

		conting_drawing_draw(dd->drawing,
				widget->window, widget->style->black_gc,
				&dd->position);
	}

	if (selected_drawing != NULL) {
		conting_drawing_draw_selected(selected_drawing,
				widget->window, widget->style->fg_gc[GTK_STATE_NORMAL],
				&selected_position);
	}

	if (current_drawing != NULL) {
		GdkPoint pos;

		gtk_widget_get_pointer(widget, &pos.x, &pos.y);
		conting_drawing_draw(current_drawing,
				widget->window, widget->style->black_gc,
				&pos);
	}

	g_print("expose_event\n");
	return TRUE;
}

static gboolean darea_button_press_event(GtkWidget *widget,
		GdkEventButton *event, gpointer user_data) {
	GSList *n;
	ContingDrawing *violated_drawing = NULL;
	GdkPoint violated_position = { -1, -1 };


	selected_drawing = NULL;
	gtk_widget_queue_draw(widget);

	if (current_drawing == NULL) {
		for (n = drawings; n != NULL; n = g_slist_next(n)) {
			DrawingToDraw *dd = n->data;

			if (conting_drawing_violates(dd->drawing, current_drawing,
						event->x - dd->position.x,
						event->y - dd->position.y)) {
				select(dd, widget);
				break;
			}
		}

		/* dd selected */
	} else {
		for (n = drawings; n != NULL; n = g_slist_next(n)) {
			DrawingToDraw *dd = n->data;

			g_print("VIOLATES: %p? ", dd->drawing);
			if (conting_drawing_violates(dd->drawing, current_drawing,
						event->x - dd->position.x,
						event->y - dd->position.y)) {
				g_print(" YES\n");
				violated_drawing = dd->drawing;
				violated_position = dd->position;
				break;
			}
			g_print(" NO\n");
		}

		if (violated_drawing != NULL) {
			if (conting_drawing_can_link(violated_drawing, current_drawing,
						event->x - violated_position.x,
						event->y - violated_position.y)) {
				if (current_drawing_start) {
					current_position.x = event->x;
					current_position.y = event->y;
				}
				current_drawing_start = conting_drawing_place_linked(
						current_drawing,
						event->x - current_position.x,
						event->y - current_position.y,
						violated_drawing,
						event->x - violated_position.x,
						event->y - violated_position.y);
				/* returns TRUE if the link is finished */
			} else {
				/* can't draw, there's another drawing already in the space
				 * and the drawing doesn't accept links from current_drawing*/
			}
		} else { /* violeted_drawing == NULL */
			conting_drawing_place(current_drawing);
			DrawingToDraw *dd = g_new(DrawingToDraw, 1);
			dd->drawing = current_drawing;

			dd->position.x = event->x;
			dd->position.y = event->y;

			drawings = g_slist_append(drawings, dd);

			current_drawing = NULL;
			current_drawing_start = TRUE;
		}
	}

	g_print("button_press_event\n");
	return TRUE;
}

static gboolean darea_motion_notify_event(GtkWidget *widget,
		GdkEventMotion *event, gpointer user_data) {
	if (current_drawing != NULL) {
		gtk_widget_queue_draw(widget);
	}
	g_print("motion_notify_event\n");
	return TRUE;
}

static void bus_button_clicked(GtkButton *button, gpointer user_data) {
	if (current_drawing != NULL && !current_drawing_start) {
		return;
	}
	current_drawing = conting_drawing_bus_new();
	current_drawing_start = TRUE;
}

static void line_button_clicked(GtkButton *button, gpointer user_data) {
	g_print("line_button_clicked()\n");
	if (current_drawing != NULL && !current_drawing_start) {
		return;
	}
	current_drawing = conting_drawing_line_new();
	current_drawing_start = TRUE;
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *darea, *swindow, *bus_button, *line_button;
	GtkWidget *vbox, *hbox;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(GTK_WIDGET(window), 400, 400);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show(vbox);

	swindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swindow),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	gtk_box_pack_start(GTK_BOX(vbox), swindow, TRUE, TRUE, 0);
	gtk_widget_show(swindow);

    darea = gtk_drawing_area_new();
	gtk_widget_set_size_request(GTK_WIDGET(darea), 800, 800);
	g_signal_connect(G_OBJECT(darea), "realize",
			G_CALLBACK(darea_realize), NULL);
	g_signal_connect(G_OBJECT(darea), "expose_event",
			G_CALLBACK(darea_expose_event), NULL);
	g_signal_connect(G_OBJECT(darea), "button_press_event",
			G_CALLBACK(darea_button_press_event), NULL);
	g_signal_connect(G_OBJECT(darea), "motion_notify_event",
			G_CALLBACK(darea_motion_notify_event), NULL);

	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(swindow), darea);
	gtk_widget_show(darea);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
	gtk_widget_show(hbox);

	bus_button = gtk_button_new_with_label("BUS");
	g_signal_connect(G_OBJECT(bus_button), "clicked",
			G_CALLBACK(bus_button_clicked), NULL);
	gtk_box_pack_start_defaults(GTK_BOX(hbox), bus_button);
	gtk_widget_show(bus_button);
	
	line_button = gtk_button_new_with_label("LINE");
	g_signal_connect(G_OBJECT(line_button), "clicked",
			G_CALLBACK(line_button_clicked), NULL);
	gtk_box_pack_start_defaults(GTK_BOX(hbox), line_button);
	gtk_widget_show(line_button);

	gtk_widget_show(window);

    gtk_main();

    return 0;
}
