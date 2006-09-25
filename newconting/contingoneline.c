#include <gtk/gtk.h>
#include "contingoneline.h"
#include "contingutil.h"
#include "contingxml.h"
#include "continggroup.h"
#include "contingdata.h"
#include "continginfodialog.h"

#include "contingfilecdf.h"
#include "contingfilepeco.h"

#include "contingserializable.h"

#include <assert.h>
#include <math.h>

enum {
	CONTING_ONE_LINE_PROP_0,
	CONTING_ONE_LINE_PROP_PPU,
	CONTING_ONE_LINE_PROP_DATA
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

	ContingData *file_data;

	ContingDrawing *current_drawing;
	ContingDrawing *entered_drawing;
};

cairo_t *
conting_drawing_get_cairo(ContingDrawing *drawing)
{
	cairo_t *cr;
	ContingOneLine *oneline;
	ContingOneLinePrivate *priv;
	gdouble affine[6];

	g_return_val_if_fail(drawing != NULL && CONTING_IS_DRAWING(drawing),
			NULL);

	g_object_get(drawing, "one-line", &oneline, NULL);

	g_return_val_if_fail(oneline != NULL && CONTING_IS_ONE_LINE(oneline),
			NULL);

	priv = CONTING_ONE_LINE_GET_PRIVATE(oneline);

	cr = gdk_cairo_create(priv->widget->window);

	conting_one_line_world_to_window_affine(oneline, affine);
	cairo_transform(cr, (cairo_matrix_t *) affine);

	conting_drawing_get_i2w_affine(drawing, affine);
	cairo_transform(cr, (cairo_matrix_t *) affine);
	
	return cr;

}

static void
conting_one_line_dialog_drawing(ContingOneLine *self, ContingDrawing *drawing)
{
	ContingOneLinePrivate *priv;

	GtkWidget *window /*, *label */;
	/*
	char buf[256];
	*/

	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));
	g_return_if_fail(drawing != NULL && CONTING_IS_DRAWING(drawing));

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);
/*
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	sprintf(buf, "%p\t%s", drawing, g_type_name(G_OBJECT_TYPE(drawing)));
	gtk_window_set_title(GTK_WINDOW(window), buf);
	label = gtk_label_new(buf);

	gtk_container_add(GTK_CONTAINER(window), label);
	*/

	/* TODO: Generate an event indicating that the pointer moved outside the
	 * range of the contingoneline. This should signal an leave event
	 * of the current entered_drawing and an enter after the closing of the
	 * window */

	window = conting_info_dialog_new(priv->file_data, drawing);
	gtk_window_set_transient_for(GTK_WINDOW(window),
			GTK_WINDOW(gtk_widget_get_toplevel(priv->widget)));
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ON_PARENT);
	gtk_window_set_modal(GTK_WINDOW(window), TRUE);

	gtk_widget_show(window);
}

void
conting_one_line_edit(ContingOneLine *self, ContingDrawing *drawing)
{
	ContingOneLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));
	g_return_if_fail(drawing != NULL && CONTING_IS_DRAWING(drawing));

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	assert(g_slist_find(priv->drawings, drawing));

	conting_one_line_dialog_drawing(self, drawing);
}

void
conting_one_line_clear(ContingOneLine *self)
{
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    while (priv->drawings) {
        conting_drawing_delete(CONTING_DRAWING(priv->drawings->data));
    }
}

void
conting_one_line_load_data(ContingOneLine *self, const char *filename)
{
	ContingOneLinePrivate *priv;
	ContingFile *file;

	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	conting_data_clear(priv->file_data);

	if (conting_file_follows(CONTING_TYPE_FILE_CDF, filename)) {
		file = conting_file_cdf_new();
	} else if (conting_file_follows(CONTING_TYPE_FILE_PECO, filename)) {
		g_print("PECO\n");
		file = conting_file_peco_new();
	} else {
		g_print("NONE\n");
		return;
	}

	conting_data_load_file(priv->file_data, file, filename);
	g_print("UNREF\n");
	g_object_unref(file);
}

void
conting_one_line_open(ContingOneLine *self, const char *filename)
{
    ContingOneLinePrivate *priv;
    xmlDocPtr doc;
    xmlNodePtr oneline;
    xmlNodePtr drawing_node;
    GHashTable *id_drawing;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    conting_one_line_clear(self);

    id_drawing = g_hash_table_new(NULL, NULL);

    LIBXML_TEST_VERSION

    doc = xmlReadFile(filename, NULL, 0);

    oneline = xmlDocGetRootElement(doc);

    for (drawing_node = oneline->children; drawing_node;
            drawing_node = drawing_node->next) {
        xmlChar *id, *class;
        ContingDrawing *drawing;

        if (!xmlStrEqual(drawing_node->name, BAD_CAST "drawing"))
            continue;

        id = xmlGetProp(drawing_node, BAD_CAST "id");
        class = xmlGetProp(drawing_node, BAD_CAST "class");

        drawing = CONTING_DRAWING(g_object_new(g_type_from_name(class),
                "one-line", self,
                NULL));

        printf("putting %lu = %p\n", strtoul(id, NULL, 10), drawing);

        g_hash_table_insert(id_drawing,
                GUINT_TO_POINTER(strtoul(id, NULL, 10)), drawing);

        xmlFree(id);
        xmlFree(class);
    }

    for (drawing_node = oneline->children; drawing_node;
            drawing_node = drawing_node->next) {
        xmlChar *id;
        ContingDrawing *drawing;

        if (!xmlStrEqual(drawing_node->name, BAD_CAST "drawing")) {
            continue;
		}

        id = xmlGetProp(drawing_node, BAD_CAST "id");
        drawing = g_hash_table_lookup(id_drawing,
                GUINT_TO_POINTER(strtoul(id, NULL, 10)));
        printf("drawing %lu = %p\n", strtoul(id, NULL, 10), drawing);
        
        
        conting_serializable_read(CONTING_SERIALIZABLE(drawing),
				drawing_node, id_drawing);
        priv->drawings = g_slist_append(priv->drawings, drawing);

        xmlFree(id);
    }

    for (drawing_node = oneline->children; drawing_node;
            drawing_node = drawing_node->next) {
		if (!xmlStrEqual(drawing_node->name, BAD_CAST "data"))
			continue;

		conting_serializable_read(CONTING_SERIALIZABLE(priv->file_data),
				drawing_node, id_drawing);
	}

    g_hash_table_destroy(id_drawing);

    xmlFreeDoc(doc);

    xmlCleanupParser();

    conting_one_line_update(self, NULL);
}

void
conting_one_line_save(ContingOneLine *self, const char *filename)
{
    ContingOneLinePrivate *priv;
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;
    xmlNodePtr drawing_node = NULL;
    char buff[256];
    guint id;
    GSList *n;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    LIBXML_TEST_VERSION

    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "one-line");
    xmlDocSetRootElement(doc, root_node);

    for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
        drawing_node = xmlNewNode(NULL, BAD_CAST "drawing");
        g_object_get(G_OBJECT(n->data), "id", &id, NULL);
        sprintf(buff, "%d", id);
        xmlNewProp(drawing_node, BAD_CAST "id", BAD_CAST buff);
		xmlNewProp(drawing_node, BAD_CAST "class",
				BAD_CAST G_OBJECT_TYPE_NAME(n->data));
        conting_serializable_write(CONTING_SERIALIZABLE(n->data), drawing_node,
				NULL);
        xmlAddChild(root_node, drawing_node);
    }

	conting_serializable_write(CONTING_SERIALIZABLE(priv->file_data),
			root_node, NULL);

    xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

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
#define TOLERANCE CONTING_DRAWING_TOLERANCE
void
conting_one_line_update(ContingOneLine *self,
                        ArtDRect *bounds)
{
    ContingOneLinePrivate *priv;
    ArtDRect win_bounds;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	if (bounds == NULL) {
		gtk_widget_queue_draw(priv->widget);
		return;
	}

	conting_util_expand_bounds(bounds, TOLERANCE + 2);

    conting_one_line_world_to_window(self,
            bounds->x0, bounds->y0,
            &win_bounds.x0, &win_bounds.y0);
    conting_one_line_world_to_window(self,
            bounds->x1, bounds->y1,
            &win_bounds.x1, &win_bounds.y1);

/*
    g_print("update1: (%lf, %lf); (%lf, %lf)\n",
			bounds->x0, bounds->y0, bounds->x1, bounds->y1);
    g_print("update2: (%lf, %lf); (%lf, %lf)\n",
            win_bounds.x0, win_bounds.y0, win_bounds.x1, win_bounds.y1);
			*/

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
	ArtPoint p;
	gdouble affine[6];

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);
	
	art_affine_translate(affine,
			priv->scrolling_area.x0, priv->scrolling_area.y0);
	art_affine_scale(affine, 1.0 / priv->ppu, 1.0 / priv->ppu);

	p.x = win_x;
	p.y = win_y;
	art_affine_point(&p, &p, affine);

    if (world_x)
		*world_x = p.x;
/* 
 * *world_x = priv->scrolling_area.x0 + (win_x / priv->ppu);
 */

    if (world_y)
		*world_y = p.y;
/*
 *      *world_y = priv->scrolling_area.y0 + (win_y / priv->ppu);
 */
}

void
conting_one_line_world_to_window(ContingOneLine *self,
                                 gdouble world_x, gdouble world_y,
                                 gdouble *win_x, gdouble *win_y)
{
    ContingOneLinePrivate *priv;
	gdouble affine[6];
	ArtPoint p;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	art_affine_translate(affine, -priv->scrolling_area.x0,
			-priv->scrolling_area.y0);
	art_affine_scale(affine, priv->ppu, priv->ppu);

	p.x = world_x;
	p.y = world_y;

	art_affine_point(&p, &p, affine);

    if (win_x)
		*win_x = p.x;
/*      
 *      *win_x = (world_x - priv->scrolling_area.x0) * priv->ppu;
 */

    if (win_y)
		*win_y = p.y;
/*      
 *      *win_y = (world_y - priv->scrolling_area.y0) * priv->ppu;
 */
}

void
conting_one_line_world_to_window_affine(ContingOneLine *self,
		gdouble affine[6])
{
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	art_affine_translate(affine, -priv->scrolling_area.x0,
			-priv->scrolling_area.y0);
	art_affine_scale(affine, priv->ppu, priv->ppu);
}
void
conting_one_line_delete_drawing(ContingOneLine *self,
                                ContingDrawing *drawing)
{
    ContingOneLinePrivate *priv;
	ArtDRect bounds;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	/* is it evil?
	if (priv->state == CONTING_ONE_LINE_GRABBING
			&& priv->grabbed_drawing == drawing) {
		conting_drawing_ungrab(drawing);
		priv->state = CONTING_ONE_LINE_NONE;
	}
	*/

	conting_drawing_get_bounds(drawing, &bounds);

	if (priv->state == CONTING_ONE_LINE_CREATED
			&& drawing == priv->placing_drawing) {
		priv->placing_drawing = NULL;
		priv->state = CONTING_ONE_LINE_NONE;
	} else {
		if (drawing == priv->current_drawing) {
			priv->current_drawing = NULL;
		} else if (drawing == priv->entered_drawing) {
			priv->entered_drawing = NULL;
		}
	    priv->drawings = g_slist_remove(priv->drawings, drawing);
	}

    g_print("%p (%s) (%lf, %lf), (%lf, %lf) deleting\n",
			drawing, g_type_name(G_OBJECT_TYPE(drawing)),
			bounds.x0, bounds.y0, bounds.x1, bounds.y1);

	g_print("UNREF\n");
    g_object_unref(drawing);

	conting_one_line_update(self, &bounds);
}

static gboolean
conting_one_line_send_event(ContingOneLine *self,
		                    ContingDrawing *drawing,
                        	GdkEvent *event)
{
	ArtDRect bounds;
	gboolean result;

	g_return_val_if_fail(self != NULL && CONTING_IS_ONE_LINE(self), FALSE);

	conting_drawing_get_update_bounds(drawing, &bounds);
	result = conting_drawing_event(drawing, event);
	conting_one_line_update(self, &bounds);
	if (conting_one_line_contains(self, drawing)) {
		conting_drawing_update(drawing);
	}

	return result;
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
			g_value_set_double(value, priv->ppu);
			break;
		case CONTING_ONE_LINE_PROP_DATA:
			g_value_set_object(value, priv->file_data);
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
			priv->ppu = g_value_get_double(value);
			conting_one_line_update(CONTING_ONE_LINE(self), NULL);
			break;
		case CONTING_ONE_LINE_PROP_DATA:
			priv->file_data = g_value_get_object(value);
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
				gboolean leave = TRUE;
				ContingDrawing *entered = NULL;

	            for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
    	        	if (conting_drawing_answer(CONTING_DRAWING(n->data),
								world_x, world_y)) {
						if (n->data == priv->entered_drawing) {
							leave = FALSE;
						}
						if (entered == NULL) {
							entered = n->data;
						}

						if (conting_one_line_send_event(
								CONTING_ONE_LINE(user_data),
								CONTING_DRAWING(n->data), (GdkEvent *) event))
							break;
					}
				}

				if (leave) {
					if (priv->entered_drawing) {
						GdkEvent *levent = gdk_event_copy((GdkEvent *) event);
						levent->type = GDK_LEAVE_NOTIFY;
						conting_one_line_send_event(CONTING_ONE_LINE(user_data),
								priv->entered_drawing, (GdkEvent *) levent);
						priv->entered_drawing = NULL;
					}

					if (entered) {
						GdkEvent *eevent = gdk_event_copy((GdkEvent *) event);
						assert(priv->entered_drawing != entered);
						eevent->type = GDK_ENTER_NOTIFY;
						conting_one_line_send_event(
								CONTING_ONE_LINE(user_data),
								entered, (GdkEvent *) eevent);
						priv->entered_drawing = entered;
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
				/*
                gtk_widget_queue_draw(priv->widget);
				*/
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
		case CONTING_ONE_LINE_SELECTING:
        case CONTING_ONE_LINE_NONE:
			{
				gboolean found = FALSE;
				for (n = priv->drawings; n != NULL; /* */) {
					ContingDrawing *drawing = n->data;
					n = g_slist_next(n);

                	if (conting_drawing_answer(drawing, world_x, world_y)
							&& !found) {
                        g_print("%p (%s) answered\n",
                                drawing, g_type_name(G_OBJECT_TYPE(drawing)));
						if (conting_one_line_send_event(
								CONTING_ONE_LINE(user_data),
								drawing, (GdkEvent *) event)) {
							found = TRUE;
						}
                    } else if (CONTING_IS_GROUP(drawing)) {
						/* TODO: change the place of this group
						 * creation/deletion code. */
						GSList *gn = conting_group_get_children(
								CONTING_GROUP(drawing));

						priv->drawings = g_slist_remove(priv->drawings,
								drawing);

						while (gn) {
							conting_drawing_set_selected(
									CONTING_DRAWING(gn->data), FALSE);
							priv->drawings = g_slist_append(priv->drawings,
									gn->data);
							g_object_set(G_OBJECT(gn->data),
									"group", NULL,
									NULL);
							gn = g_slist_next(gn);

						}
					}
                }
				if (found)
					break;

				g_print("NONE ANSWER\n");

				g_print("selecting start()\n");
				priv->state = CONTING_ONE_LINE_SELECTING;
				priv->selection_box.x0 = event->x;
				priv->selection_box.y0 = event->y;
				priv->selection_box.x1 = event->x;
				priv->selection_box.y1 = event->y;
				
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
		case CONTING_ONE_LINE_SELECTING:
			{
				/* TODO: change the place of this group creation/deletion
				 * code. maybe make it part of continggroup.c */
				GSList *n;
				ArtDRect selection_bounds;
				ContingGroup *group = CONTING_GROUP(
						g_object_new(CONTING_TYPE_GROUP,
							"one-line", user_data, NULL));

				conting_util_correct_bounds(&priv->selection_box,
						&priv->selection_box);

				conting_one_line_window_to_world(CONTING_ONE_LINE(user_data),
						priv->selection_box.x0, priv->selection_box.y0,
						&selection_bounds.x0, &selection_bounds.y0);
				conting_one_line_window_to_world(CONTING_ONE_LINE(user_data),
						priv->selection_box.x1, priv->selection_box.y1,
						&selection_bounds.x1, &selection_bounds.y1);

				for (n = priv->drawings; n != NULL; ) {
					ContingDrawing *drawing;
					ArtDRect bounds;

					drawing = n->data;
					conting_drawing_get_bounds(drawing, &bounds);

					n = g_slist_next(n);
					if (conting_util_bounds_contains(&selection_bounds,
								&bounds)) {
						g_print("%p added\n", drawing);
						priv->drawings = g_slist_remove(priv->drawings,
								drawing);
						conting_group_add_drawing(group, drawing);
					}
				}

				conting_drawing_place(CONTING_DRAWING(group));

				if (conting_drawing_is_placed(CONTING_DRAWING(group))) {
					priv->drawings = g_slist_append(priv->drawings, group);
				} else {
					assert(FALSE);
				}

				priv->state = CONTING_ONE_LINE_NONE;
				conting_one_line_update_selection(CONTING_ONE_LINE(user_data),
						event->x, event->y);
			}
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
                        g_print("%p (%s) answered\n",
                                n->data, g_type_name(G_OBJECT_TYPE(n->data)));
						if (conting_one_line_send_event(
								CONTING_ONE_LINE(user_data),
								CONTING_DRAWING(n->data), (GdkEvent *) event))
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
		ContingDrawing *drawing = n->data;
		cairo_t *cr = conting_drawing_get_cairo(drawing);

        conting_drawing_draw(drawing, cr);

		cairo_destroy(cr);
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
			{
				cairo_t *cr = conting_drawing_get_cairo(priv->placing_drawing);

				conting_drawing_draw(priv->placing_drawing, cr);

				cairo_destroy(cr);
			}
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
			/*
			if (event->keyval == GDK_Escape) {
				conting_one_line_cancel_placing(CONTING_ONE_LINE(user_data));
			}
			*/
			break;
		case CONTING_ONE_LINE_GRABBING:
			conting_one_line_send_event(CONTING_ONE_LINE(user_data),
					priv->grabbed_drawing, (GdkEvent *) event);
			break;
		case CONTING_ONE_LINE_NONE:

    		for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
		        if (conting_drawing_is_selected(CONTING_DRAWING(n->data))) {
        		    if (conting_one_line_send_event(CONTING_ONE_LINE(user_data),
							CONTING_DRAWING(n->data), (GdkEvent *) event))
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
	g_print("UNREF\n");
				g_object_unref(priv->placing_drawing);
				priv->placing_drawing = NULL;
				conting_one_line_update(self, &bounds);
				priv->placing_drawing = drawing;
				conting_drawing_update(drawing);
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
			g_param_spec_double("ppu",
							 "Points per unit",
							 "The zooming of this diagram",
							 0.5,	/* 50% zoom, minimum value */
							 4,		/* 400% zoom, maximum value */
							 1,		/* 100% zoom, default value */
							 G_PARAM_READABLE | G_PARAM_WRITABLE));

	g_object_class_install_property(G_OBJECT_CLASS(g_class),
			CONTING_ONE_LINE_PROP_DATA,
			g_param_spec_object("data",
								"ContingData object",
								"Data file associated",
								CONTING_TYPE_DATA,
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
    priv->ppu = 1.5;

    priv->placing_drawing = NULL;
    priv->drawings = NULL;

	priv->file_data = CONTING_DATA(g_object_new(CONTING_TYPE_DATA, NULL));
	assert(priv->file_data);

	priv->current_drawing = NULL;
	priv->entered_drawing = NULL;
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

/*
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
bus_button_clicked(GtkButton *button,
                    gpointer user_data)
{
	conting_one_line_create_by_type(CONTING_ONE_LINE(user_data),
			CONTING_TYPE_BUS);
}

static void
line_button_clicked(GtkButton *button,
                    gpointer user_data)
{
	conting_one_line_create_by_type(CONTING_ONE_LINE(user_data),
			CONTING_TYPE_LINE);
}

static void
trans2_button_clicked(GtkButton *button,
					  gpointer user_data)
{
	conting_one_line_create_by_type(CONTING_ONE_LINE(user_data),
			CONTING_TYPE_TRANS2);
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
    GtkWidget *bus_button, *line_button, *trans2_button;
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

    bus_button = gtk_button_new_with_label("BUS");
    g_signal_connect(G_OBJECT(bus_button), "clicked",
            G_CALLBACK(bus_button_clicked), oneline);
    gtk_box_pack_start_defaults(GTK_BOX(hbox), bus_button);

    line_button = gtk_button_new_with_label("LINE");
    g_signal_connect(G_OBJECT(line_button), "clicked",
            G_CALLBACK(line_button_clicked), oneline);
    gtk_box_pack_start_defaults(GTK_BOX(hbox), line_button);

	trans2_button = gtk_button_new_with_label("TRANS2");
	g_signal_connect(G_OBJECT(trans2_button), "clicked",
			G_CALLBACK(trans2_button_clicked), oneline);
    gtk_box_pack_start_defaults(GTK_BOX(hbox), trans2_button);
    
    gtk_widget_show_all(window);

    gtk_main();
    
    return 0;
}
*/
