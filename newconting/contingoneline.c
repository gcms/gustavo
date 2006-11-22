#include <gtk/gtk.h>
#include "contingoneline.h"
#include "contingutil.h"
#include "contingxml.h"
#include "continggroup.h"
#include "contingdata.h"
#include "contingitemdata.h"
#include "continginfodialog.h"
#include "contingline.h"
#include "contingbusbase.h"

#include "contingfilecdf.h"
#include "contingfilepeco.h"

#include "contingdrawingoperation.h"
#include "contingserializable.h"

#include <assert.h>
#include <math.h>

static gint change_mode_id = 0;
static GType param_types[] = { G_TYPE_INT };

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
    ArtDRect scrolling_area;
    ArtDRect selection_box;

    ContingData *file_data;

    GSList *drawings;
    
    ContingDrawing *placing_drawing;
    ContingDrawing *grabbed_drawing;
	ContingDrawing *current_drawing;
	ContingDrawing *entered_drawing;

	GList *operations_bank[CONTING_ONE_LINE_MODE_NUMBER];
#define operations operations_bank[priv->mode]

    ContingOneLineMode mode;

	GdkColor bgcolor;
};

/* FRIEND METHOD */
void conting_one_line_update(ContingOneLine *self, ArtDRect *bounds);

/* PRIVATE METHOD */
static void conting_one_line_ungroup_all(ContingOneLine *self);

/* SIGNAL CALLBACK */
static void
create_motion(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data);
/* SIGNAL CALLBACK */
static void
create_button(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data);
/* SIGNAL CALLBACK */
static void
create_key(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data);
/* SIGNAL CALLBACK */
static void
edit_motion(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data);
/* SIGNAL CALLBACK */
static void
edit_button(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data);
/* SIGNAL CALLBACK */
static void
edit_key(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data);
/* SIGNAL CALLBACK */
static void
edit_notify(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data);

/* PRIVATE METHOD */
static void
conting_one_line_connect_edit(ContingOneLine *self, ContingDrawing *drawing)
{
	ContingOneLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));
	g_return_if_fail(drawing != NULL && CONTING_IS_DRAWING(drawing));

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);
	
	g_signal_handlers_disconnect_by_func(drawing, create_motion, NULL);
	g_signal_handlers_disconnect_by_func(drawing, create_button, NULL);
	g_signal_handlers_disconnect_by_func(drawing, create_key, NULL);
    g_signal_handlers_disconnect_matched(drawing, G_SIGNAL_MATCH_DATA,
            0, 0, NULL, NULL, NULL);

	g_signal_connect(G_OBJECT(drawing), "motion-event",
			G_CALLBACK(edit_motion), NULL);
	g_signal_connect(G_OBJECT(drawing), "button-event",
			G_CALLBACK(edit_button), NULL);
	g_signal_connect(G_OBJECT(drawing), "key-event",
			G_CALLBACK(edit_key), NULL);
	g_signal_connect(G_OBJECT(drawing), "notify-event",
			G_CALLBACK(edit_notify), NULL);
}

/* SIGNAL CALLBACK */
static void
view_button(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data);

/* PRIVATE METHOD */
static void
conting_one_line_connect_view(ContingOneLine *self, ContingDrawing *drawing)
{
	ContingOneLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));
	g_return_if_fail(drawing != NULL && CONTING_IS_DRAWING(drawing));

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	g_signal_handlers_disconnect_matched(drawing, G_SIGNAL_MATCH_DATA,
			0, 0, NULL, NULL, NULL);

	g_signal_connect(G_OBJECT(drawing), "button-event",
			G_CALLBACK(view_button), NULL);
}

/* PRIVATE METHOD */
static void
conting_one_line_place(ContingOneLine *self, ContingDrawing *drawing)
{
	ContingOneLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));
	g_return_if_fail(drawing != NULL && CONTING_IS_DRAWING(drawing));

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	assert(priv->placing_drawing == drawing
			|| priv->placing_drawing == NULL);
	assert(conting_drawing_is_placed(drawing));
	
	priv->drawings = g_slist_append(priv->drawings, drawing);
    priv->state = CONTING_ONE_LINE_NONE;
    priv->placing_drawing = NULL;

	conting_one_line_connect_edit(self, drawing);
}


static void
edit_enter(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
	g_print("%p entered\n", drawing);
}
static void
edit_left(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
	g_print("%p left\n", drawing);
}
static void
edit_notify(ContingDrawing *drawing, ContingDrawingEvent *event,
        gpointer user_data)
{
    switch (event->type) {
        case CONTING_DRAWING_ENTER:
            edit_enter(drawing, event, user_data);
            break;
        case CONTING_DRAWING_LEAVE:
            edit_left(drawing, event, user_data);
            break;
        default:
            break;
    }
}

static void
edit_button_press(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
	ArtPoint pi;
	
    g_print("edit_button_press %p (%s)\n",
            drawing, g_type_name(G_OBJECT_TYPE(drawing)));


	conting_drawing_set_selected(drawing, TRUE);
	
	pi.x = event->button.x;
	pi.y = event->button.y;

	if (CONTING_IS_LINE(drawing) && (event->button.state & GDK_CONTROL_MASK)) {
		conting_line_create_point(CONTING_LINE(drawing), &pi);
	}
	
	conting_drawing_grab(drawing, &pi);
}
static void
edit_2button_press(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
    g_print("edit_2button_press %p (%s)\n",
            drawing, g_type_name(G_OBJECT_TYPE(drawing)));
    
	conting_drawing_ungrab(drawing);
	conting_one_line_edit(conting_drawing_get_one_line(drawing), drawing);
}
static void
edit_button_release(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
    g_print("edit_button_release %p (%s)\n",
            drawing, g_type_name(G_OBJECT_TYPE(drawing)));
	conting_drawing_ungrab(drawing);
}


static void
edit_button(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
    g_print("edit_button %p (%s)\n",
            drawing, g_type_name(G_OBJECT_TYPE(drawing)));
	switch (event->type) {
		case CONTING_DRAWING_BUTTON_PRESS:
			edit_button_press(drawing, event, user_data);
			break;
		case CONTING_DRAWING_2BUTTON_PRESS:
			edit_2button_press(drawing, event, user_data);
			break;
		case CONTING_DRAWING_BUTTON_RELEASE:
			edit_button_release(drawing, event, user_data);
			break;
		default:
			break;
	}
}

static void
edit_motion(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
	ArtPoint pi;
	
	pi.x = event->motion.x;
	pi.y = event->motion.y;
	conting_drawing_motion(drawing, &pi);
}

static void
create_motion(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
	ArtPoint pi;

	pi.x = event->motion.x;
	pi.y = event->motion.y;

	if (CONTING_IS_LINE(drawing)) {
		conting_line_set_shift_mask(CONTING_LINE(drawing),
				event->motion.state & GDK_SHIFT_MASK);
	}

	conting_drawing_motion_place(drawing, &pi);

	g_print("create_motion()\n");
}

#include <gdk/gdkkeysyms.h>
static void
create_key(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
	switch (event->key.keyval) {
		case GDK_Escape:
			conting_drawing_delete(drawing);
			break;
		case GDK_r:
			if (CONTING_IS_COMPONENT(drawing)) {
				conting_component_rotate(CONTING_COMPONENT(drawing));
			}
			break;
		default:
			break;
	}
}

static void
create_button_press(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
	ContingOneLine *self;

	g_return_if_fail(drawing != NULL && CONTING_IS_DRAWING(drawing));

	self = conting_drawing_get_one_line(drawing);

	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

	g_print("create_button()\n");
    
	conting_drawing_place(drawing);

    if (conting_drawing_is_placed(drawing)) {
		conting_one_line_place(self, drawing);
	}
}

static void
create_button(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
	switch (event->type) {
		case CONTING_DRAWING_BUTTON_PRESS:
			create_button_press(drawing, event, user_data);
			break;
		default:
			break;
	}
}

static void
edit_key(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
	switch (event->key.keyval) {
		case GDK_Delete:
			conting_drawing_delete(drawing);
			break;
		case GDK_r:
			if (CONTING_IS_COMPONENT(drawing)) {
				conting_component_rotate(CONTING_COMPONENT(drawing));
			}
			break;
		default:
			break;
	}
}

static void
view_2button_press(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
	conting_one_line_edit(conting_drawing_get_one_line(drawing), drawing);
}

static void
view_button(ContingDrawing *drawing, ContingDrawingEvent *event,
		gpointer user_data)
{
	g_print("view_button\n");
	switch (event->type) {
		case CONTING_DRAWING_2BUTTON_PRESS:
			view_2button_press(drawing, event, user_data);
			break;
		default:
	}
}

static GdkColor *
color_by_name(const gchar *name)
{
    static GdkColor color;
    
    gdk_color_parse(name, &color);

    return &color;
}

/* PRIVATE METHOD */
static void
conting_one_line_set_edit(ContingOneLine *self) 
{
	ContingOneLinePrivate *priv;
    GSList *n;
    ContingVisitor *color;

	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    conting_one_line_ungroup_all(self);

	/* Initialize visitors */
    color = g_object_new(CONTING_TYPE_VISITOR_COLOR, NULL);

    for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
		/* connect edit signals */
		conting_one_line_connect_edit(self, n->data);

		/* Apply the visitors */
        conting_drawing_accept(n->data, color);
    }
	/* Destroy visitors */
    g_object_unref(color);

	/* Setup background color */
	gdk_color_parse("white", &priv->bgcolor);

    
    priv->mode = CONTING_ONE_LINE_EDIT;
	priv->state = CONTING_ONE_LINE_NONE;
    conting_one_line_update(self, NULL);
}

/* PRIVATE METHOD */
static void
conting_one_line_set_view(ContingOneLine *self)
{
	ContingOneLinePrivate *priv;
	GSList *n;
	
	ContingVisitor *color;
	ContingDrawingOperationDefault *default_opr;

	g_print("set view\n\n\n\n\n");
	g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	assert(conting_data_check(priv->file_data, NULL));

	if (priv->mode == CONTING_ONE_LINE_VIEW)
		return;

    conting_one_line_ungroup_all(self);
    for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
        conting_drawing_set_selected(CONTING_DRAWING(n->data), FALSE);
    }


    /* TODO: load these from settings */
	color = g_object_new(CONTING_TYPE_VISITOR_COLOR, NULL);
    conting_visitor_color_add_interval(CONTING_VISITOR_COLOR(color),
            -1, 25, color_by_name("blue"));
    conting_visitor_color_add_interval(CONTING_VISITOR_COLOR(color),
            25, 50, color_by_name("orange"));
    conting_visitor_color_add_interval(CONTING_VISITOR_COLOR(color),
            50, 100, color_by_name("red"));
    conting_visitor_color_add_interval(CONTING_VISITOR_COLOR(color),
            100, 200, color_by_name("yellow"));
    conting_visitor_color_add_interval(CONTING_VISITOR_COLOR(color),
            200, -1, color_by_name("green"));
    conting_visitor_color_use_default(CONTING_VISITOR_COLOR(color),
            FALSE);

    
	for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
		ContingDrawing *drawing = n->data;
		
		conting_one_line_connect_view(self, n->data);

		conting_drawing_accept(drawing, color);
	}

	g_object_unref(color);
	gdk_color_parse("black", &priv->bgcolor);

	/* Enable filtering. This shouldn't be needed everytime. Maybe
	 * change the place of it to instance init. */
	default_opr = conting_one_line_get_default_operation_by_mode(self,
			CONTING_ONE_LINE_VIEW);
	conting_drawing_operation_default_set_filter(default_opr, TRUE);


	priv->mode = CONTING_ONE_LINE_VIEW;
	priv->state = CONTING_ONE_LINE_NONE;

	conting_one_line_update(self, NULL);
}


/* PUBLIC METHOD */
gboolean
conting_one_line_set_mode(ContingOneLine *self, ContingOneLineMode mode)
{
	ContingOneLinePrivate *priv;
    ContingOneLineMode prev_mode;

	g_return_val_if_fail(self != NULL && CONTING_IS_ONE_LINE(self), FALSE);

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    prev_mode = priv->mode;

	switch (mode) {
		case CONTING_ONE_LINE_EDIT:
            conting_one_line_set_edit(self);
			break;
		case CONTING_ONE_LINE_VIEW:
			if (!conting_data_check(priv->file_data, NULL)) {
				return FALSE;
			} else {
				conting_one_line_set_view(self);
			}
			break;
		default:
			break;
	}

	priv->mode = mode;
    
    g_print("OMG\n\n");
    if (prev_mode != mode) {
        g_signal_emit_by_name(self, "change-mode", mode);
    }

	return TRUE;
}

/* FRIEND METHOD */
/* TODO: make a pool of cursors */
void
conting_one_line_cursor(ContingOneLine *self, GdkCursorType type)
{
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	gdk_window_set_cursor(priv->widget->window, gdk_cursor_new(type));
}

/* CALLBACK FUNCTION */
static const gchar *
conting_drawing_return_attr(ContingDrawingOperation *opr,
        ContingDrawing *drawing, gpointer user_data)
{
    const gchar *attr_name = user_data;

    if (!CONTING_IS_BUS_BASE(drawing)) {
        return NULL;
    }

    return conting_drawing_get_attr(drawing, attr_name);
}


/* PRIVATE METHOD */
static void
conting_one_line_get_update_bounds(ContingOneLine *self,
        ContingDrawing *drawing, ArtDRect *bounds)
{
    ContingOneLinePrivate *priv;
    GList *n;
    ArtDRect tmp_bounds;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));
    g_return_if_fail(drawing != NULL && CONTING_IS_DRAWING(drawing));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);
    
    conting_drawing_get_update_bounds(drawing, bounds);

    for (n = priv->operations; n != NULL; n = g_list_next(n)) {
        ContingDrawingOperation *opr = n->data;

        conting_drawing_operation_get_bounds(opr, drawing, &tmp_bounds);
        conting_util_union_bounds(bounds, &tmp_bounds, bounds);
    }
}

/* PUBLIC METHOD */
void
conting_one_line_update_drawing(ContingOneLine *self, ContingDrawing *drawing)
{
    ArtDRect bounds;

    conting_one_line_get_update_bounds(self, drawing, &bounds);

    conting_one_line_update(self, &bounds);
}

/* PRIVATE METHOD */
static void
conting_one_line_draw(ContingOneLine *self, ContingDrawing *drawing)
{
    ContingOneLinePrivate *priv;
    cairo_t *cr;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));
    g_return_if_fail(drawing != NULL && CONTING_IS_DRAWING(drawing));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    cr = conting_drawing_get_cairo(drawing);

    conting_drawing_draw(drawing, cr);

    cairo_destroy(cr);
}

/* STATIC PUBLIC METHOD */
cairo_t *
conting_drawing_get_cairo_absolute(ContingDrawing *drawing)
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

    return cr;

}
/* STATIC PUBLIC METHOD */
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

	cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
    
    return cr;

}

/* PRIVATE METHOD */
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

/* PUBLIC METHOD */
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

/* PUBLIC METHOD */
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

/* PUBLIC METHOD */
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

/* PUBLIC METHOD */
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
	if (priv->placing_drawing)
		conting_drawing_delete(priv->placing_drawing);

    id_drawing = g_hash_table_new(NULL, NULL);

    LIBXML_TEST_VERSION

    doc = xmlReadFile(filename, NULL, 0);

    oneline = xmlDocGetRootElement(doc);

    for (drawing_node = oneline->children; drawing_node;
            drawing_node = drawing_node->next) {
        xmlChar *id, *class;
    	GType type;
        ContingDrawing *drawing;

        if (!xmlStrEqual(drawing_node->name, BAD_CAST "drawing"))
            continue;

        id = xmlGetProp(drawing_node, BAD_CAST "id");
        class = xmlGetProp(drawing_node, BAD_CAST "class");

        type = g_type_from_name(class);
    	if (!type)
            continue;

        drawing = CONTING_DRAWING(
				g_object_new(type, "one-line", self, NULL));
	
		assert(drawing);
		if (!drawing)
			continue;

        printf("putting %lu (%s) = %p\n",
                strtoul(id, NULL, 10), g_type_name(type), drawing);

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

		assert(drawing);
        printf("drawing %lu (%s) = %p\n",
                strtoul(id, NULL, 10),
                g_type_name(G_OBJECT_TYPE(drawing)),
                drawing);
        
        
        conting_serializable_read(CONTING_SERIALIZABLE(drawing),
                drawing_node, id_drawing);

		/*
        conting_one_line_place(self, drawing);
        */
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

    conting_one_line_set_mode(self, CONTING_ONE_LINE_EDIT);
}

/* PUBLIC METHOD */
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

    conting_one_line_ungroup_all(self);

    LIBXML_TEST_VERSION

    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "one-line");
    xmlDocSetRootElement(doc, root_node);

    for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
        drawing_node = xmlNewNode(NULL, BAD_CAST "drawing");

		
		/* Adding id property */
        g_object_get(G_OBJECT(n->data), "id", &id, NULL);
        sprintf(buff, "%d", id);
        xmlNewProp(drawing_node, BAD_CAST "id", BAD_CAST buff);
		
		/* Adding class name property */
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

/* PUBLIC METHOD */
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

/* PUBLIC METHOD */
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
/* FRIEND METHOD */
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

/* PUBLIC METHOD */
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

/* PUBLIC METHOD */
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

/* PUBLIC METHOD */
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

    if (world_y)
        *world_y = p.y;
}

/* PUBLIC METHOD */
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

/* PUBLIC METHOD */
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

/* PUBLIC METHOD */
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

    g_signal_handlers_disconnect_matched(drawing, G_SIGNAL_MATCH_DATA,
            0, 0, NULL, NULL, NULL);
    g_object_unref(drawing);

    conting_one_line_update(self, &bounds);
}

/* PRIVATE METHOD */
static gboolean
conting_one_line_send_event(ContingOneLine *self,
                            ContingDrawing *drawing,
                            GdkEvent *event)
{
    ArtDRect bounds;
    gboolean result;

    g_return_val_if_fail(self != NULL && CONTING_IS_ONE_LINE(self), FALSE);

    conting_one_line_get_update_bounds(self, drawing, &bounds);
    result = conting_drawing_event(drawing, event);
    conting_one_line_update(self, &bounds);

    /* Avoid the case in which drawing was deleted */
    if (conting_one_line_contains(self, drawing)) {
        conting_one_line_get_update_bounds(self, drawing, &bounds);
        
        conting_one_line_update(self, &bounds);
    }

    return result;
}

/* PRIVATE METHOD */
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
/* PUBLIC METHOD IMPL */
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
/* PUBLIC METHOD IMPL */
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
			{
				gint x, y;
				gdk_drawable_get_size(priv->widget->window, &x, &y);

				x = (gdouble) x / priv->ppu;
				y = (gdouble) y / priv->ppu;
				
    	        priv->ppu = g_value_get_double(value);

				x = (gdouble) x * priv->ppu;
				y = (gdouble) y * priv->ppu;

				gtk_widget_set_size_request(priv->widget, x, y);
				gtk_widget_queue_resize_no_redraw(priv->widget);

        	    conting_one_line_update(CONTING_ONE_LINE(self), NULL);
			}
            break;
        case CONTING_ONE_LINE_PROP_DATA:
            priv->file_data = g_value_get_object(value);
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

/* SIGNAL CALLBACK */
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

	conting_one_line_cursor(user_data, GDK_LEFT_PTR);

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
				gboolean leave = TRUE; /* should leave the current drawing? */
				ContingDrawing *entered = NULL; /* drawing to be entered */

                for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
                    if (conting_drawing_answer(CONTING_DRAWING(n->data),
                                world_x, world_y)) {
                        if (n->data == priv->entered_drawing) {
                            leave = FALSE;
                        }
                        if (entered == NULL) {
                            entered = n->data;
                        }
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

                    assert(priv->entered_drawing == NULL);

                    if (entered) {
                        GdkEvent *eevent = gdk_event_copy((GdkEvent *) event);
                        eevent->type = GDK_ENTER_NOTIFY;
                        conting_one_line_send_event(
                                CONTING_ONE_LINE(user_data),
                                entered, (GdkEvent *) eevent);
                        priv->entered_drawing = entered;
                    }
                }

                if (priv->entered_drawing) {
                    assert(conting_drawing_answer(priv->entered_drawing,
                                world_x, world_y));

                    conting_one_line_send_event(user_data,
                            priv->entered_drawing, (GdkEvent *) event);
                }

            }
               break;
        case CONTING_ONE_LINE_CREATED:
            assert(priv->placing_drawing != NULL
                    && CONTING_IS_DRAWING(priv->placing_drawing));
            conting_one_line_send_event(CONTING_ONE_LINE(user_data),
                    priv->placing_drawing, (GdkEvent *) event);
            break;
    }

    return TRUE;
}


static void
conting_one_line_ungroup(ContingOneLine *self, ContingGroup *group)
{
    ContingOneLinePrivate *priv;
    GSList *gn;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    gn = conting_group_get_children(group);

    priv->drawings = g_slist_remove(priv->drawings, group);

    while (gn) {
        conting_drawing_set_selected(CONTING_DRAWING(gn->data), FALSE);
        priv->drawings = g_slist_append(priv->drawings, gn->data);
        g_object_set(G_OBJECT(gn->data),
                "group", NULL,
                NULL);
        gn = g_slist_next(gn);
    }

    g_object_unref(group);
}

static void
conting_one_line_ungroup_all(ContingOneLine *self)
{
    ContingOneLinePrivate *priv;
    GSList *n;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    for (n = priv->drawings; n != NULL; /* */) {
        ContingDrawing *drawing = n->data;

        n = g_slist_next(n);

        if (CONTING_IS_GROUP(drawing)) {
            conting_one_line_ungroup(self, CONTING_GROUP(drawing));
        }
    }
}

/* SIGNAL CALLBACK */
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

    /* can add a check if CTRL key is hold to allow selection of multiple
     * components */
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
				g_print("\n\n\nBUTTON PRESS");
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
                        conting_one_line_ungroup(CONTING_ONE_LINE(user_data),
                                CONTING_GROUP(drawing));
                    }
                }
                if (found)
                    break;

                g_print("NONE ANSWER\n");

                g_print("selecting start()\n");

				if (priv->mode == CONTING_ONE_LINE_EDIT) {
	                priv->state = CONTING_ONE_LINE_SELECTING;
    	            priv->selection_box.x0 = event->x;
        	        priv->selection_box.y0 = event->y;
            	    priv->selection_box.x1 = event->x;
                	priv->selection_box.y1 = event->y;
				}
            }
            break;
        case CONTING_ONE_LINE_CREATED:
            assert(priv->placing_drawing != NULL
                    && CONTING_IS_DRAWING(priv->placing_drawing));

			conting_one_line_send_event(user_data, priv->placing_drawing,
					(GdkEvent *) event);
            break;
    }

    return TRUE;
}

/* PRIVATE METHOD */
static void
conting_one_line_group(ContingOneLine *self)
{
    ContingOneLinePrivate *priv;
    GSList *n;
    ArtDRect selection_bounds;
    ContingGroup *group;
   
    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);  
   
    group = CONTING_GROUP(
            g_object_new(CONTING_TYPE_GROUP,
                "one-line", self, NULL));

    conting_util_correct_bounds(&priv->selection_box,
            &priv->selection_box);

    conting_one_line_window_to_world(self,
            priv->selection_box.x0, priv->selection_box.y0,
            &selection_bounds.x0, &selection_bounds.y0);

    conting_one_line_window_to_world(self,
            priv->selection_box.x1, priv->selection_box.y1,
            &selection_bounds.x1, &selection_bounds.y1);

    for (n = priv->drawings; n != NULL; ) {
        ContingDrawing *drawing;
        ArtDRect bounds;

        drawing = n->data;
        conting_drawing_get_bounds(drawing, &bounds);

        n = g_slist_next(n);
        if (conting_util_bounds_contains(&selection_bounds, &bounds)) {
            g_print("%p added\n", drawing);
            priv->drawings = g_slist_remove(priv->drawings, drawing);
            conting_group_add_drawing(group, drawing);
        }
    }

    conting_drawing_place(CONTING_DRAWING(group));


    if (conting_drawing_is_placed(CONTING_DRAWING(group))) {
	    g_signal_connect(G_OBJECT(group), "motion-event",
		    	G_CALLBACK(edit_motion), NULL);
	    g_signal_connect(G_OBJECT(group), "button-event",
	    		G_CALLBACK(edit_button), NULL);
    	g_signal_connect(G_OBJECT(group), "key-event",
			    G_CALLBACK(edit_key), NULL);
    	g_signal_connect(G_OBJECT(group), "notify-event",
	    		G_CALLBACK(edit_notify), NULL);
        priv->drawings = g_slist_append(priv->drawings, group);
    } else {
        assert(FALSE);
    }
}

/* SIGNAL CALLBACK */
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
            conting_one_line_group(user_data);
            priv->state = CONTING_ONE_LINE_NONE;
            conting_one_line_update_selection(user_data, event->x, event->y);
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

/* SIGNAL CALLBACK */
static gboolean
widget_expose_event(GtkWidget *widget,
                    GdkEventExpose *event,
                    gpointer user_data) {
    ContingOneLinePrivate *priv;
    GSList *n;
    GList *opr_n;
	cairo_t *cr;

    g_return_val_if_fail(user_data != NULL && CONTING_IS_ONE_LINE(user_data),
            FALSE);

    priv = CONTING_ONE_LINE_GET_PRIVATE(user_data);

    assert(widget == priv->widget);


	cr = gdk_cairo_create(widget->window);
	/*
    gdk_draw_rectangle(widget->window, widget->style->white_gc, TRUE,
            widget->allocation.x, widget->allocation.y,
            widget->allocation.width, widget->allocation.height);
			*/
	cairo_rectangle(cr, 0, 0,
			widget->allocation.width, widget->allocation.height);
	cairo_set_source_rgb(cr,
			priv->bgcolor.red / (gdouble) G_MAXUINT16,
			priv->bgcolor.green / (gdouble) G_MAXUINT16,
			priv->bgcolor.blue / (gdouble) G_MAXUINT16);
	cairo_fill(cr);
	cairo_stroke(cr);

	cairo_destroy(cr);

	/*
    for (n = priv->drawings; n != NULL; n = g_slist_next(n)) {
        ContingDrawing *drawing = n->data;

        conting_one_line_draw(user_data, drawing);
    }
	*/

    for (opr_n = priv->operations; opr_n; opr_n = g_list_next(opr_n)) {
        ContingDrawingOperation *opr = opr_n->data;

        for (n = priv->drawings; n; n = g_slist_next(n)) {
            conting_drawing_operation_draw(opr, n->data);
        }
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
    		for (opr_n = priv->operations; opr_n; opr_n = g_list_next(opr_n)) {
		        ContingDrawingOperation *opr = opr_n->data;

		        conting_drawing_operation_draw(opr, priv->placing_drawing);
		    }
            break;
        default:
            break;
    }


    return TRUE;
}



/* SIGNAL CALLBACK */
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


/* PUBLIC METHOD */
void
conting_one_line_create(ContingOneLine *self,
                        ContingDrawing *drawing) {
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    if (priv->mode != CONTING_ONE_LINE_EDIT)
        return;

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

	assert(priv->placing_drawing);

	g_signal_connect(G_OBJECT(priv->placing_drawing), "motion-event",
			G_CALLBACK(create_motion), NULL);
	g_signal_connect(G_OBJECT(priv->placing_drawing), "button-event",
			G_CALLBACK(create_button), NULL);
	g_signal_connect(G_OBJECT(priv->placing_drawing), "key-event",
			G_CALLBACK(create_key), NULL);
}

/* WARNING: HACK, should be static */
/* PRIVATE METHOD */
void
conting_one_line_operation_update(ContingOneLine *self,
        ContingDrawingOperation *opr)
{
    ContingOneLinePrivate *priv;
    GSList *n;
    ArtDRect total_bounds, bounds;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    if (!priv->drawings)
        return;

    conting_drawing_operation_get_bounds(opr, priv->drawings->data, &bounds);
    total_bounds = bounds;

    for (n = priv->drawings->next; n != NULL; n = g_slist_next(n)) {
        ContingDrawing *drawing = n->data;

        conting_drawing_operation_get_bounds(opr, drawing, &bounds);
        conting_util_union_bounds(&total_bounds, &bounds, &total_bounds);
    }

    conting_one_line_update(self, &total_bounds);
}

/* PUBLIC METHOD */
void
conting_one_line_add_operation(ContingOneLine *self,
        ContingDrawingOperation *opr)
{
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    priv->operations = g_list_append(priv->operations, opr);

    conting_one_line_operation_update(self, opr);
}

/* PUBLIC METHOD */
void
conting_one_line_remove_operation(ContingOneLine *self,
        ContingDrawingOperation *opr)
{
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    priv->operations = g_list_remove(priv->operations, opr);

    conting_one_line_operation_update(self, opr);
}

/* PUBLIC METDHO IMPLEMENTATION */
static void
conting_one_line_finalize(GObject *self)
{
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    /* TODO: implement it */
}

/* PUBLIC METHOD */
void
conting_one_line_set_widget(ContingOneLine *self,
                            GtkWidget *widget)
{
    ContingOneLinePrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_ONE_LINE(self));

    priv = CONTING_ONE_LINE_GET_PRIVATE(self);

    /* TODO: disconnect signals if there is an widget already set */
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

/* CLASS INIT */
static void
conting_one_line_class_init(gpointer g_class,
                            gpointer class_data)
{
    GObjectClass *gobject_class;

    gobject_class = G_OBJECT_CLASS(g_class);
    gobject_class->set_property = conting_one_line_set_property;
    gobject_class->get_property = conting_one_line_get_property;
    gobject_class->finalize = conting_one_line_finalize;

    g_object_class_install_property(G_OBJECT_CLASS(g_class),
            CONTING_ONE_LINE_PROP_PPU,
            g_param_spec_double("ppu",
                             "Points per unit",
                             "The zooming of this diagram",
                             0.5,    /* 50% zoom, minimum value */
                             4,        /* 400% zoom, maximum value */
                             1,        /* 100% zoom, default value */
                             G_PARAM_READABLE | G_PARAM_WRITABLE));

    g_object_class_install_property(G_OBJECT_CLASS(g_class),
            CONTING_ONE_LINE_PROP_DATA,
            g_param_spec_object("data",
                                "ContingData object",
                                "Data file associated",
                                CONTING_TYPE_DATA,
                                G_PARAM_READABLE | G_PARAM_WRITABLE));

    change_mode_id = g_signal_newv(
            "change-mode",
            G_TYPE_FROM_CLASS(g_class),
            G_SIGNAL_RUN_FIRST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
            NULL, /* class closure */
            NULL, /* accumulator */
            NULL, /* accu_data */
            g_cclosure_marshal_VOID__INT,
            G_TYPE_NONE, /* return_type */
            1,
            param_types);

    g_type_class_add_private(g_class, sizeof(ContingOneLinePrivate));

}

/* INSTANCE INIT */
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

	/* Init default operations */
	for (priv->mode = 0; priv->mode < CONTING_ONE_LINE_MODE_NUMBER;
			priv->mode++) {
		ContingDrawingOperation *operation;

		priv->operations = NULL;

		operation = g_object_new(CONTING_TYPE_DRAWING_OPERATION_DEFAULT, NULL);
		priv->operations = g_list_append(priv->operations, operation);
	}

	priv->mode = CONTING_ONE_LINE_EDIT;


/*        
    priv->operations = NULL;

    {
        ContingDrawingOperation *operation;

		operation = g_object_new(CONTING_TYPE_DRAWING_OPERATION_DEFAULT, NULL);
		priv->operations = g_list_append(priv->operations, operation);

        operation = g_object_new(CONTING_TYPE_DRAWING_OPERATION_LABEL, NULL);
        g_object_set(operation,
                "label-func", conting_drawing_return_attr,
                "user-data", "name",
                NULL);
        priv->operations = g_list_append(priv->operations, operation);
    }
        */

	gdk_color_parse("white", &priv->bgcolor);
}

/* GET TYPE */
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


/* PUBLIC STATIC METHOD */
ContingItemData *
conting_drawing_get_item_data(ContingDrawing *drawing)
{
	ContingOneLine *self;
	ContingOneLinePrivate *priv;

	g_object_get(drawing, "one-line", &self, NULL);

	g_return_val_if_fail(self != NULL && CONTING_IS_ONE_LINE(self), NULL);

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	return conting_data_get(priv->file_data, drawing);
}
/* PUBLIC METHOD */
ContingOneLineMode
conting_one_line_get_mode(ContingOneLine *self)
{
	ContingOneLinePrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_ONE_LINE(self), -1);

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	return priv->mode;
}

/* PUBLIC METHOD */
ContingDrawingOperationDefault*
conting_one_line_get_default_operation_by_mode(ContingOneLine *self,
		ContingOneLineMode mode)
{
	ContingOneLinePrivate *priv;
	GList *n;

	g_return_val_if_fail(self != NULL && CONTING_IS_ONE_LINE(self), NULL);

	priv = CONTING_ONE_LINE_GET_PRIVATE(self);

	/* TODO: find a better way to store and find the
	 * ContingDrawingOperationDefault */

	for (n = priv->operations_bank[mode]; n != NULL; n = g_list_next(n)) {
		if (G_OBJECT_TYPE(n->data) == CONTING_TYPE_DRAWING_OPERATION_DEFAULT)
			return n->data;
	}

	return NULL;
}
