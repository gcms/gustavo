#include <gtk/gtk.h>
#include "contingoneline.h"
#include "contingutil.h"
#include <assert.h>
#include <math.h>

enum {
	CONTING_ONE_LINE_PROP_0,
	CONTING_ONE_LINE_PROP_PPU
};

typedef enum {
    CONTING_ONE_LINE_NONE,
    CONTING_ONE_LINE_CREATED,
	CONTING_ONE_LINE_GRABBING,
	CONTING_ONE_LINE_SELECTING
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

	ArtDRect selection_box;
};

gboolean
conting_one_line_contains(ContingOneLine *self,
		                  ContingDrawing *drawing)
{
	ContingOneLinePrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_ONE_LINE(self), FALSE);

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	return drawing == priv->placing_drawing
		|| g_slist_find(priv->drawings, drawing) != NULL;
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

	/* is it evil? */
	if (priv->state == CONTING_ONE_LINE_GRABBING
			&& priv->grabbed_drawing == drawing) {
		conting_drawing_ungrab(drawing);
		priv->state = CONTING_ONE_LINE_NONE;
	}

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

static void
conting_one_line_cancel_placing(ContingOneLine *self)
{
	ContingOneLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	/* trabalho semelhante a conting_one_line_delete_drawing() */

	priv->state = CONTING_ONE_LINE_NONE;
	conting_drawing_delete(priv->placing_drawing); /* disconnect handlers */
	g_object_unref(priv->placing_drawing);
	priv->placing_drawing = NULL;
}

static void
conting_one_line_update_selection(ContingOneLine *self, gdouble x, gdouble y)
{
	/* FIXME: create functions to handle selection.
	 * Need to avoid calling gtk_widget_queue_draw*() directly */
	ContingOneLinePrivate *priv;
	ArtDRect old_bounds, bounds;

	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	conting_util_correct_bounds(&priv->selection_box, &old_bounds);

	priv->selection_box.x1 = x;
	priv->selection_box.y1 = y;

	conting_util_correct_bounds(&priv->selection_box, &bounds);
	
	conting_util_union_bounds(&bounds, &old_bounds, &bounds);

	conting_util_expand_bounds(&bounds, priv->ppu + 4);
	gtk_widget_queue_draw_area(priv->widget,
			(gint) bounds.x0, (gint) bounds.y0,
			(gint) (bounds.x1 - bounds.x0),
			(gint) (bounds.y1 - bounds.y0));
}
static void
conting_one_line_get_property(GObject *self,
                              guint prop_id,
                              GValue *value,
                              GParamSpec *pspec)
{
	ContingOneLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	switch (prop_id) {
		case CONTING_ONE_LINE_PROP_PPU:
			g_value_set_int(value, priv->ppu);
			break;
		default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}
static void
conting_one_line_set_property(GObject *self,
                              guint prop_id,
                              const GValue *value,
                              GParamSpec *pspec)
{
	ContingOneLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	switch (prop_id) {
		case CONTING_ONE_LINE_PROP_PPU:
			priv->ppu = g_value_get_int(value);
			break;
		default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
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
		case CONTING_ONE_LINE_SELECTING:
			conting_one_line_update_selection(CONTING_ONE_LINE(user_data),
					event->x, event->y);
			break;
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
						conting_one_line_send_event(
								CONTING_ONE_LINE(user_data),
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
	fprintf(stderr, "button_press\n");

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
		case CONTING_ONE_LINE_SELECTING:
        case CONTING_ONE_LINE_NONE:
			{
				for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
                	if (conting_drawing_answer(CONTING_DRAWING(n->data),
                                world_x, world_y)) {
						conting_one_line_send_event(
								CONTING_ONE_LINE(user_data),
								CONTING_DRAWING(n->data), (GdkEvent *) event);
                        g_print("%p (%s) answered\n",
                                n->data, g_type_name(G_OBJECT_TYPE(n->data)));
                        break;
                    }
                }
				if (n != NULL)
					break;

				g_print("selecting start()\n");
				priv->state = CONTING_ONE_LINE_SELECTING;
				priv->selection_box.x0 = event->x;
				priv->selection_box.y0 = event->y;
				
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
	fprintf(stderr, "button_release\n");

    conting_one_line_window_to_world(CONTING_ONE_LINE(user_data),
            event->x, event->y,
            &world_x, &world_y);

    priv = CONTING_ONE_LINE_GET_PRIVATE(user_data);

    switch (priv->state) {
		case CONTING_ONE_LINE_SELECTING:
			g_print("selecting end()\n");
			priv->state = CONTING_ONE_LINE_NONE;
			conting_one_line_update_selection(CONTING_ONE_LINE(user_data),
					event->x, event->y);
			break;
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
						conting_one_line_send_event(
								CONTING_ONE_LINE(user_data),
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
		case CONTING_ONE_LINE_SELECTING:
			{
				GdkRectangle rect;
    			static GdkGC *gc = NULL;
			    if (gc == NULL) {
			        static GdkColor color;
			        gdk_color_parse("black", &color);
			        gc = gdk_gc_new(widget->window);
			        gdk_gc_set_foreground(gc, &color);
			        gdk_gc_set_background(gc, &color);
					gdk_gc_set_rgb_fg_color(gc, &color);
					gdk_gc_set_rgb_bg_color(gc, &color);
					gdk_gc_set_fill(gc, GDK_SOLID);
					gdk_gc_set_line_attributes(gc, 1,
							GDK_LINE_ON_OFF_DASH, GDK_CAP_NOT_LAST,
							GDK_JOIN_MITER);
				}
				conting_util_bounds_to_rect(&priv->selection_box, &rect);
				gdk_draw_rectangle(priv->widget->window, gc, FALSE,
						rect.x, rect.y, rect.width, rect.height);
			}
			break;
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
		priv->ppu += 0.3;
		gtk_widget_queue_draw(priv->widget);
		return TRUE;
	} else if (event->keyval == GDK_Page_Down) {
		priv->ppu -= 0.3;
		if (priv->ppu < 1.0)
			priv->ppu = 1.0;
		gtk_widget_queue_draw(priv->widget);
		return TRUE;
	}

	switch (priv->state) {
		case CONTING_ONE_LINE_CREATED:
			conting_one_line_send_event(CONTING_ONE_LINE(user_data),
					priv->placing_drawing, (GdkEvent *) event);
			if (event->keyval == GDK_Escape) {
				conting_one_line_cancel_placing(CONTING_ONE_LINE(user_data));
			}
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
	GObjectClass *gobject_class;

	gobject_class = G_OBJECT_CLASS(g_class);
	gobject_class->set_property = conting_one_line_set_property;
	gobject_class->get_property = conting_one_line_get_property;

	g_object_class_install_property(G_OBJECT_CLASS(g_class),
			CONTING_ONE_LINE_PROP_PPU,
			g_param_spec_int("ppu",
							 "Points per unit",
							 "The zooming of this diagram",
							 0.5,	/* 50% zoom, minimum value */
							 4,		/* 400% zoom, maximum value */
							 1,		/* 100% zoom, default value */
							 G_PARAM_READABLE | G_PARAM_WRITABLE));

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
