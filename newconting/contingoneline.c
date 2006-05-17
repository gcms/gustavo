#include <gtk/gtk.h>
#include "contingoneline.h"
#include <assert.h>

typedef enum {
    CONTING_ONE_LINE_NONE,
    CONTING_ONE_LINE_CREATED,
	CONTING_ONE_LINE_GRABBING
} ContingOneLineState;

#define CONTING_ONE_LINE_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_ONE_LINE, ContingOneLinePrivate))
typedef struct ContingOneLinePrivate_ ContingOneLinePrivate;
struct ContingOneLinePrivate_ {
    GtkWidget *widget;
    ContingOneLineState state;

    gdouble ppu;

    ContingDrawing *placing_drawing;

    ArtDRect scrolling_area;

    GSList *drawings;

    ContingDrawing *grabbed_drawing;
};

gboolean
conting_one_line_contains(ContingOneLine *self,
		                  ContingDrawing *drawing)
{
	ContingOneLinePrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_ONE_LINE(self), FALSE);

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	return g_slist_find(priv->drawings, drawing) != NULL;
}

GSList *
conting_one_line_answer(ContingOneLine *self,
                        gdouble world_x, gdouble world_y)
{
    ContingOneLinePrivate *priv;
    GSList *list, *n;

    g_return_val_if_fail(self != NULL && CONTING_IS_ONE_LINE(self), NULL);

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    list = NULL;
    for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
        if (conting_drawing_answer(CONTING_DRAWING(n->data),
                    world_x, world_y)) {
            list = g_slist_append(list, n->data);
        }
    }

    return list;
}
void
conting_one_line_update(ContingOneLine *self,
                        ArtDRect *bounds)
{
    ContingOneLinePrivate *priv;
    ArtDRect win_bounds;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    conting_one_line_world_to_window(self,
            bounds->x0, bounds->y0,
            &win_bounds.x0, &win_bounds.y0);
    conting_one_line_world_to_window(self,
            bounds->x1, bounds->y1,
            &win_bounds.x1, &win_bounds.y1);

    g_print("update: (%lf, %lf); (%lf, %lf)\n",
            win_bounds.x0, win_bounds.y0, win_bounds.x1, win_bounds.y1);

    gtk_widget_queue_draw_area(priv->widget,
            (gint) win_bounds.x0, (gint) win_bounds.y0,
            (gint) (win_bounds.x1 - win_bounds.x0),
            (gint) (win_bounds.y1 - win_bounds.y0));
}

void
conting_one_line_grab(ContingOneLine *self,
                      ContingDrawing *grabbed_drawing)
{
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    assert(priv->grabbed_drawing == NULL);

    priv->grabbed_drawing = grabbed_drawing;

	priv->state = CONTING_ONE_LINE_GRABBING;
}

void
conting_one_line_ungrab(ContingOneLine *self,
                        ContingDrawing *grabbed_drawing)
{
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    assert(priv->grabbed_drawing != NULL
            && priv->grabbed_drawing == grabbed_drawing);

    g_return_if_fail(priv->grabbed_drawing == grabbed_drawing);

    priv->grabbed_drawing = NULL;

	priv->state = CONTING_ONE_LINE_NONE;
}

void
conting_one_line_window_to_world(ContingOneLine *self,
                                 gdouble win_x, gdouble win_y,
                                 gdouble *world_x, gdouble *world_y)
{
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    if (world_x)
        *world_x = priv->scrolling_area.x0 + (win_x / priv->ppu);

    if (world_y)
        *world_y = priv->scrolling_area.y0 + (win_y / priv->ppu);
}

void
conting_one_line_world_to_window(ContingOneLine *self,
                                 gdouble world_x, gdouble world_y,
                                 gdouble *win_x, gdouble *win_y)
{
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    if (*win_x)
        *win_x = (world_x - priv->scrolling_area.x0) * priv->ppu;

    if (*win_y)
        *win_y = (world_y - priv->scrolling_area.y0) * priv->ppu;
}
void
conting_one_line_delete_drawing(ContingOneLine *self,
                                ContingDrawing *drawing)
{
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    priv->drawings = g_slist_remove(priv->drawings, drawing);

    g_object_unref(drawing);

	/*
	 * TODO: remove this from here and find some way to update the screen
	 * when a drawing is deleted.
	 */
	gtk_widget_queue_draw(priv->widget);
}

static void
conting_one_line_send_event(ContingOneLine *self,
		                    ContingDrawing *drawing,
                        	GdkEvent *event)
{
	ArtDRect bounds;

	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

	conting_drawing_get_bounds(drawing, &bounds);
	conting_drawing_event(drawing, event);
	conting_one_line_update(self, &bounds);
	if (conting_one_line_contains(self, drawing)) {
		conting_drawing_update(drawing);
	}
}

static gboolean
widget_motion_notify_event(GtkWidget *widget,
                           GdkEventMotion *event,
                           gpointer user_data)
{
    ContingOneLinePrivate *priv;
    gdouble world_x, world_y;

    g_return_val_if_fail(user_data != NULL && CONTING_IS_ONE_LINE(user_data),
            FALSE);

    priv = CONTING_ONE_LINE_GET_PRIVATE(user_data);

    conting_one_line_window_to_world(CONTING_ONE_LINE(user_data),
            event->x, event->y,
            &world_x, &world_y);

    switch (priv->state) {
		case CONTING_ONE_LINE_GRABBING:
			assert(priv->grabbed_drawing
					&& CONTING_IS_DRAWING(priv->grabbed_drawing));
			conting_one_line_send_event(CONTING_ONE_LINE(user_data),
					priv->grabbed_drawing, (GdkEvent *) event);
			break;
        case CONTING_ONE_LINE_NONE:
			{
				GSList *n;
	            for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
    	        	if (conting_drawing_answer(CONTING_DRAWING(n->data),
								world_x, world_y)) {
						conting_one_line_send_event(CONTING_ONE_LINE(user_data),
								CONTING_DRAWING(n->data), (GdkEvent *) event);
						break;
					}
				}
			}
       	    break;
        case CONTING_ONE_LINE_CREATED:
            assert(priv->placing_drawing != NULL
                    && CONTING_IS_DRAWING(priv->placing_drawing));
            {
				conting_one_line_send_event(CONTING_ONE_LINE(user_data),
						priv->placing_drawing, (GdkEvent *) event);
				/*
                gdouble translate[6];
                art_affine_translate(translate, world_x, world_y);
                conting_drawing_affine_absolute(priv->placing_drawing,
                        translate);
				*/
                gtk_widget_queue_draw(priv->widget);
            }
            break;
    }

/*    gtk_widget_queue_draw(priv->widget); */

    return TRUE;
}

static gboolean
widget_button_press_event(GtkWidget *widget,
                           GdkEventButton *event,
                           gpointer user_data)
{
    ContingOneLinePrivate *priv;
    GSList *n;
    gdouble world_x, world_y;

    g_return_val_if_fail(user_data != NULL && CONTING_IS_ONE_LINE(user_data),
            FALSE);

    conting_one_line_window_to_world(CONTING_ONE_LINE(user_data),
            event->x, event->y,
            &world_x, &world_y);

    priv = CONTING_ONE_LINE_GET_PRIVATE(user_data);

    /* can add a check if CTRL key is hold */
    for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
        conting_drawing_set_selected(CONTING_DRAWING(n->data), FALSE);
    }

    switch (priv->state) {
		case CONTING_ONE_LINE_GRABBING:
			assert(priv->grabbed_drawing
					&& CONTING_IS_DRAWING(priv->grabbed_drawing));
			conting_one_line_send_event(CONTING_ONE_LINE(user_data),
					priv->grabbed_drawing, (GdkEvent *) event);
			break;
        case CONTING_ONE_LINE_NONE:
			{
				for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
                	if (conting_drawing_answer(CONTING_DRAWING(n->data),
                                world_x, world_y)) {
						conting_one_line_send_event(CONTING_ONE_LINE(user_data),
								CONTING_DRAWING(n->data), (GdkEvent *) event);
                        g_print("%p (%s) answered\n",
                                n->data, g_type_name(G_OBJECT_TYPE(n->data)));
                        break;
                    }
                }
            }
            break;
        case CONTING_ONE_LINE_CREATED:
            assert(priv->placing_drawing != NULL
                    && CONTING_IS_DRAWING(priv->placing_drawing));
            {
/*
				conting_one_line_send_event(CONTING_ONE_LINE(user_data),
						priv->placing_drawing, (GdkEvent *) event);
						*/
				/*
                gdouble translate[6];
                art_affine_translate(translate, world_x, world_y);
                conting_drawing_affine_absolute(priv->placing_drawing,
                        translate);
*/
                conting_drawing_place(priv->placing_drawing);

                if (conting_drawing_is_placed(priv->placing_drawing)) {
                    priv->drawings = g_slist_append(priv->drawings,
                            priv->placing_drawing);
                    priv->state = CONTING_ONE_LINE_NONE;
                    priv->placing_drawing = NULL;
                }
            }
    }

    return TRUE;
}

static gboolean
widget_button_release_event(GtkWidget *widget,
                            GdkEventButton *event,
                            gpointer user_data)
{
    ContingOneLinePrivate *priv;
    gdouble world_x, world_y;

    g_return_val_if_fail(user_data != NULL && CONTING_IS_ONE_LINE(user_data),
            FALSE);

    conting_one_line_window_to_world(CONTING_ONE_LINE(user_data),
            event->x, event->y,
            &world_x, &world_y);

    priv = CONTING_ONE_LINE_GET_PRIVATE(user_data);

    switch (priv->state) {
		case CONTING_ONE_LINE_GRABBING:
			assert(priv->grabbed_drawing
					&& CONTING_IS_DRAWING(priv->grabbed_drawing));
			conting_one_line_send_event(CONTING_ONE_LINE(user_data),
					priv->grabbed_drawing, (GdkEvent *) event);
			break;
        case CONTING_ONE_LINE_NONE:
            {
                GSList *n;
                for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
                    if (conting_drawing_answer(CONTING_DRAWING(n->data),
                                world_x, world_y)) {
						conting_one_line_send_event(CONTING_ONE_LINE(user_data),
								CONTING_DRAWING(n->data), (GdkEvent *) event);
                        g_print("%p (%s) answered\n",
                                n->data, g_type_name(G_OBJECT_TYPE(n->data)));
                        break;
                    }
                }
            }
            break;
        default:
            break;
    }

    return TRUE;
}

static gboolean
widget_expose_event(GtkWidget *widget,
                    GdkEventExpose *event,
                    gpointer user_data) {
    ContingOneLinePrivate *priv;
    GSList *n;

    g_return_val_if_fail(user_data != NULL && CONTING_IS_ONE_LINE(user_data),
            FALSE);

    priv = CONTING_ONE_LINE_GET_PRIVATE(user_data);
    gdk_draw_rectangle(widget->window, widget->style->white_gc, TRUE,
            widget->allocation.x, widget->allocation.y,
            widget->allocation.width, widget->allocation.height);

    for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
        conting_drawing_draw(CONTING_DRAWING(n->data),
                priv->widget->window, &widget->allocation);
    }

    switch (priv->state) {
        case CONTING_ONE_LINE_NONE:
            break;
        case CONTING_ONE_LINE_CREATED:
            conting_drawing_draw(CONTING_DRAWING(priv->placing_drawing),
                    priv->widget->window, &widget->allocation);
            break;
		default:
			break;
    }


    return TRUE;
}
#include <gdk/gdkkeysyms.h>
static gboolean
widget_key_press_event(GtkWidget *widget,
                       GdkEventKey *event,
                       gpointer user_data)
{
    ContingOneLinePrivate *priv;
    GSList *n;

    g_return_val_if_fail(user_data != NULL && CONTING_IS_ONE_LINE(user_data),
            FALSE);

    priv = CONTING_ONE_LINE_GET_PRIVATE(user_data);

	if (event->keyval == GDK_Page_Up) {
		priv->ppu++;
		gtk_widget_queue_draw(priv->widget);
		return TRUE;
	} else if (event->keyval == GDK_Page_Down) {
		priv->ppu--;
		if (priv->ppu < 1.0)
			priv->ppu = 1.0;
		gtk_widget_queue_draw(priv->widget);
		return TRUE;
	}

	switch (priv->state) {
		case CONTING_ONE_LINE_CREATED:
			conting_one_line_send_event(CONTING_ONE_LINE(user_data),
					priv->placing_drawing, (GdkEvent *) event);
			break;
		case CONTING_ONE_LINE_GRABBING:
			conting_one_line_send_event(CONTING_ONE_LINE(user_data),
					priv->grabbed_drawing, (GdkEvent *) event);
			break;
		case CONTING_ONE_LINE_NONE:
    		for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
		        if (conting_drawing_is_selected(CONTING_DRAWING(n->data))) {
        		    conting_one_line_send_event(CONTING_ONE_LINE(user_data),
							CONTING_DRAWING(n->data), (GdkEvent *) event);
		            break;
        		}
		    }
			break;
		default:
			break;
    }

    return TRUE;
}


void
conting_one_line_create(ContingOneLine *self,
                        ContingDrawing *drawing) {
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    switch (priv->state) {
        case CONTING_ONE_LINE_NONE:
            priv->placing_drawing = drawing;
            priv->state = CONTING_ONE_LINE_CREATED;
            break;
		case CONTING_ONE_LINE_CREATED:
			{
				/* TODO: improve it, remove clutter.
				 * Can use conting_one_line_send_event()? */
				ArtDRect bounds;
				conting_drawing_get_bounds(priv->placing_drawing, &bounds);
				g_object_unref(priv->placing_drawing);
				priv->placing_drawing = NULL;
				conting_one_line_update(self, &bounds);
				priv->placing_drawing = drawing;
			}
			break;
        default:
            break;
    }
}

void
conting_one_line_set_widget(ContingOneLine *self,
                            GtkWidget *widget)
{
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    assert (priv->widget == NULL);

    priv->widget = widget;

    priv->scrolling_area.x0 = priv->widget->allocation.x;
    priv->scrolling_area.y0 = priv->widget->allocation.y;
    priv->scrolling_area.x1 = priv->scrolling_area.x0
        + priv->widget->allocation.width;
    priv->scrolling_area.y1 = priv->scrolling_area.y0
        + priv->widget->allocation.height;

    g_signal_connect(G_OBJECT(widget), "motion-notify-event",
            G_CALLBACK(widget_motion_notify_event), self);
    g_signal_connect(G_OBJECT(widget), "button-press-event",
            G_CALLBACK(widget_button_press_event), self);
    g_signal_connect(G_OBJECT(widget), "button-release-event",
            G_CALLBACK(widget_button_release_event), self);
    g_signal_connect(G_OBJECT(widget), "expose-event",
            G_CALLBACK(widget_expose_event), self);
    g_signal_connect(G_OBJECT(gtk_widget_get_toplevel(widget)),
            "key-press-event", G_CALLBACK(widget_key_press_event), self);
}

static void
conting_one_line_class_init(gpointer g_class,
                            gpointer class_data)
{
    g_type_class_add_private(g_class, sizeof(ContingOneLinePrivate));
}

static void
conting_one_line_instance_init(GTypeInstance *self,
                               gpointer g_class)
{    
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    priv->widget = NULL;
    priv->state = CONTING_ONE_LINE_NONE;
    priv->ppu = 2;

    priv->placing_drawing = NULL;
    priv->drawings = NULL;
}

GType
conting_one_line_get_type(void)
{
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingOneLineClass),
            NULL,
            NULL,
            conting_one_line_class_init,
            NULL,
            NULL,
            sizeof(ContingOneLine),
            0,
            conting_one_line_instance_init,
            NULL
        };

        type = g_type_register_static(G_TYPE_OBJECT,
                "ContingOneLine",
                &type_info,
                0);
    }

    return type;
}

#include "contingcomponent.h"
#include "contingline.h"
static void
conting_one_line_create_by_type(ContingOneLine *oneline, GType type)
{
	GObject *object;

	object = g_object_new(type,
			"one-line", oneline,
			NULL);

	conting_one_line_create(oneline, CONTING_DRAWING(object));
}
static void
left_button_clicked(GtkButton *button,
                    gpointer user_data)
{
	conting_one_line_create_by_type(CONTING_ONE_LINE(user_data),
			CONTING_TYPE_COMPONENT);
}

static void
right_button_clicked(GtkButton *button,
                    gpointer user_data)
{
	conting_one_line_create_by_type(CONTING_ONE_LINE(user_data),
			CONTING_TYPE_LINE);
}
static void darea_realize(GtkWidget *widget, gpointer user_data) {
    gtk_widget_add_events(widget, GDK_EXPOSURE_MASK
            | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK
            | GDK_POINTER_MOTION_MASK | GDK_KEY_PRESS_MASK
            | GDK_KEY_RELEASE_MASK);
}


int main(int argc, char *argv[]) {
    GtkWidget *window, *swindow, *darea;
    GtkWidget *vbox, *hbox;
    GtkWidget *left_button, *right_button;
    ContingOneLine *oneline;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(GTK_WIDGET(window), 300, 300);

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    swindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swindow),
            GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_box_pack_start(GTK_BOX(vbox), swindow, TRUE, TRUE, 0);

    darea = gtk_drawing_area_new();
    gtk_widget_set_size_request(GTK_WIDGET(darea), 600, 600);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(swindow), darea);
    g_signal_connect(G_OBJECT(darea), "realize",
            G_CALLBACK(darea_realize), NULL);

    oneline = CONTING_ONE_LINE(g_object_new(CONTING_TYPE_ONE_LINE, NULL));
    conting_one_line_set_widget(oneline, darea);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);

    left_button = gtk_button_new_with_label("LEFT");
    g_signal_connect(G_OBJECT(left_button), "clicked",
            G_CALLBACK(left_button_clicked), oneline);
    gtk_box_pack_start_defaults(GTK_BOX(hbox), left_button);

    right_button = gtk_button_new_with_label("RIGHT");
    g_signal_connect(G_OBJECT(right_button), "clicked",
            G_CALLBACK(right_button_clicked), oneline);
    gtk_box_pack_start_defaults(GTK_BOX(hbox), right_button);
    
    gtk_widget_show_all(window);

    gtk_main();
    
    return 0;
}
