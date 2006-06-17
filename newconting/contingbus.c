#include "contingbus.h"
#include "contingutil.h"
#include <string.h>
#include <math.h>
#include <assert.h>

#define CONTING_BUS_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_BUS, ContingBusPrivate))

#define TOLERANCE 3
#define SIZE ((TOLERANCE * 2) - 1)

static gpointer parent_class = NULL;

typedef struct ContingBusPrivate_ ContingBusPrivate;
struct ContingBusPrivate_ {
	gboolean dragging;
	ArtPoint dragging_point;

	GHashTable *points;
	GSList *links;

	ArtPoint *start_resize;

	gdouble rotate[6];
};

static void
conting_bus_draw(ContingDrawing *self,
                  GdkDrawable *drawable,
                  const GdkRectangle *drawing_rect)
{
    ContingBusPrivate *priv;
    ContingComponent *comp;
	gdouble affine[6];
    ArtPoint pw0, pw1;
	GdkRectangle rect;

    static GdkGC *gc = NULL;
    if (gc == NULL) {
        static GdkColor color;
        gdk_color_parse("black", &color);
        gc = gdk_gc_new(drawable);
        gdk_gc_set_foreground(gc, &color);
        gdk_gc_set_background(gc, &color);
		gdk_gc_set_rgb_fg_color(gc, &color);
		gdk_gc_set_rgb_bg_color(gc, &color);
		gdk_gc_set_fill(gc, GDK_SOLID);
    }

    g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

    priv = CONTING_BUS_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

	conting_drawing_get_i2w_affine(self, affine);

    art_affine_point(&pw0, &comp->p0, affine);

    art_affine_point(&pw1, &comp->p1, affine);

	conting_one_line_world_to_window(conting_drawing_get_one_line(self),
			pw0.x, pw0.y, &pw0.x, &pw0.y);
	conting_one_line_world_to_window(conting_drawing_get_one_line(self),
			pw1.x, pw1.y, &pw1.x, &pw1.y);


	g_print("drawing: (%lf, %lf); (%lf, %lf)\n", pw0.x, pw0.y, pw1.x, pw1.y);
	
	rect.x = (gint) (pw0.x < pw1.x ? pw0.x : pw1.x);
	rect.y = (gint) (pw0.y < pw1.y ? pw0.y : pw1.y);
	rect.width = (gint) fabs(pw0.x - pw1.x);
	rect.height = (gint) fabs(pw0.y - pw1.y);

	gdk_draw_rectangle(drawable, gc, TRUE,
			rect.x, rect.y, rect.width, rect.height);

	if (conting_drawing_is_selected(self)) {
		gdk_draw_rectangle(drawable, gc, TRUE,
				(gint) (pw0.x - TOLERANCE), (gint) (pw0.y - TOLERANCE),
				SIZE, SIZE);
		gdk_draw_rectangle(drawable, gc, TRUE,
				(gint) (pw1.x - TOLERANCE), (gint) (pw0.y - TOLERANCE),
				SIZE, SIZE);
		gdk_draw_rectangle(drawable, gc, TRUE,
				(gint) (pw0.x - TOLERANCE), (gint) (pw1.y - TOLERANCE),
				SIZE, SIZE);
		gdk_draw_rectangle(drawable, gc, TRUE,
				(gint) (pw1.x - TOLERANCE), (gint) (pw1.y - TOLERANCE),
				SIZE, SIZE);
		
	}
}
static gboolean
conting_bus_get_link_point(ContingComponent *self,
                           ContingDrawing *line,
                           ArtPoint *p)
{
	ContingBusPrivate *priv;
	ArtPoint *point;

	g_return_val_if_fail(self != NULL && CONTING_IS_BUS(self), FALSE);

    priv = CONTING_BUS_GET_PRIVATE(self);

	point = g_hash_table_lookup(priv->points, line);

	if (point == NULL) {
		return FALSE;
	} else if (p != NULL) {
		gdouble affine[6];

		*p = *point;
		conting_drawing_get_i2w_affine(CONTING_DRAWING(self), affine);

		art_affine_point(p, p, affine);

		return TRUE;
	}

	return FALSE;
}

static void
conting_bus_finalize(GObject *self)
{
	ContingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

	g_hash_table_destroy(priv->points);
	g_slist_free(priv->links);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void
conting_bus_instance_init(GTypeInstance *self,
		                   gpointer g_class)
{
	ContingBusPrivate *priv;
    ContingComponent *comp;

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

    priv = CONTING_BUS_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

	comp->p0.x = -3;
	comp->p0.y = -20;
	comp->p1.x = 3;
	comp->p1.y = 20;

    /*
	comp->placed = FALSE;
    */
	priv->dragging = FALSE;
	priv->start_resize = NULL;

	priv->points = g_hash_table_new_full(NULL, NULL, NULL, g_free);
	priv->links = NULL;

	art_affine_rotate(priv->rotate, 0.0);
}
static void
conting_bus_disconnect_link(ContingBus *self,
		                      ContingDrawing *drawing)
{
	ContingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

	g_print("line %p disconnected from %p\n", drawing, self);

	g_hash_table_remove(priv->points, drawing);
	g_signal_handlers_disconnect_matched(drawing,
			G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, self);
	
	{
		GSList *n;
		g_print("still linked: ");
		for (n = priv->links; n != NULL; n = g_slist_next(n)) {
			g_print("%p, ", n->data);
		}
		g_print("\n");
	}
}
static void conting_bus_delete(ContingDrawing *self)
{
	ContingBusPrivate *priv;
	GSList *n;

	g_print("DELETING\n");

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

	if (priv->dragging) {
		priv->dragging = FALSE;
		conting_drawing_ungrab(self);
	}

	for (n = priv->links; n != NULL; n = g_slist_next(n)) {
		conting_bus_disconnect_link(CONTING_BUS(self),
				CONTING_DRAWING(n->data));
	}
	
	g_slist_free(priv->links);
	priv->links = NULL;

	g_signal_emit_by_name(self, "delete");

	CONTING_DRAWING_CLASS(parent_class)->delete(self);
}
static xmlNodePtr conting_bus_point_node(gconstpointer data, gpointer user_data)
{
	xmlNodePtr node;
	char buff[256];
	const ArtPoint *p = data;

	node = xmlNewNode(NULL, BAD_CAST "point");

	sprintf(buff, "%lf %lf", p->x, p->y);
	xmlAddChild(node, xmlNewText(BAD_CAST buff));


	return node;
}
static xmlNodePtr conting_bus_drawing_node(gconstpointer data, gpointer user_data)
{
	xmlNodePtr node;
	char buff[256];
	guint id;
	const ContingDrawing *drawing = data;

	g_return_val_if_fail(drawing != NULL && CONTING_IS_DRAWING(drawing), NULL);

	node = xmlNewNode(NULL, BAD_CAST "drawing");

	g_object_get(G_OBJECT(drawing),
			"id", &id,
			NULL);
	
	sprintf(buff, "%u", id);
	xmlNewProp(node, BAD_CAST "id", BAD_CAST buff);

	return node;
}

static gpointer
conting_bus_load_drawing(xmlNodePtr node, gpointer user_data)
{
    GHashTable *id_drawing = user_data;
    xmlChar *id;
    gpointer result;

    assert(xmlStrEqual(node->name, "drawing"));

    id = xmlGetProp(node, BAD_CAST "id");

    result = g_hash_table_lookup(id_drawing,
            GUINT_TO_POINTER(strtoul(id, NULL, 10)));
    assert(result);

    xmlFree(id);

    return result;
}
static gpointer
conting_bus_load_point(xmlNodePtr node, gpointer user_data)
{
    xmlChar *point_text;
    ArtPoint *p = g_new(ArtPoint, 1);
    assert(xmlStrEqual(node->name, "point"));

    point_text = xmlNodeListGetString(node->doc, node->children, TRUE);

    sscanf(point_text, "%lf %lf", &p->x, &p->y);

    xmlFree(point_text);

    return p;
}
static void
conting_bus_place_xml(ContingDrawing *self, xmlNodePtr drawing_node,
                      GHashTable *id_drawing)
{
    ContingBusPrivate *priv;
    ContingComponent *comp;
    xmlNodePtr class_node;

    g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

    priv = CONTING_BUS_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);
    
    for (class_node = drawing_node->children; class_node;
            class_node = class_node->next) {
        xmlChar *class_name;

        if (!xmlStrEqual(class_node->name, BAD_CAST "class"))
            continue;

        class_name = xmlGetProp(class_node, BAD_CAST "name");

        if (class_name && xmlStrEqual(class_name, "ContingBus")) {
            xmlNodePtr attr;

            for (attr = class_node->children; attr; attr = attr->next) {
                xmlChar *name, *type;

                if (!xmlStrEqual(attr->name, BAD_CAST "attribute"))
                    continue;

                name = xmlGetProp(attr, BAD_CAST "name");
                type = xmlGetProp(attr, BAD_CAST "type");

                printf("type = %s\tname = %s\n", type, name);

                if (xmlStrEqual(type, BAD_CAST "point")
                        && xmlStrEqual(name, BAD_CAST "p0")) {
                    conting_util_load_point(attr, &comp->p0);
                } else if (xmlStrEqual(type, BAD_CAST "point")
                        && xmlStrEqual(name, BAD_CAST "p1")) {
                    conting_util_load_point(attr, &comp->p1);
                } else if (xmlStrEqual(type, BAD_CAST "affine")
                        && xmlStrEqual(name, BAD_CAST "rotate")) {
                    conting_util_load_affine(attr, priv->rotate);
                } else if (xmlStrEqual(type, BAD_CAST "map")
                        && xmlStrEqual(name, BAD_CAST "points")) {
                    conting_util_load_hash(attr, priv->points,
                            conting_bus_load_drawing, conting_bus_load_point,
                            id_drawing);
                }

                xmlFree(name);
                xmlFree(type);
            }
        }

        if (class_name)
            xmlFree(class_name);
    }
/*
    priv->placed = TRUE;
    */

    CONTING_DRAWING_CLASS(parent_class)->place_xml(self,
            drawing_node, id_drawing);
}

static xmlNodePtr
conting_bus_xml_node(ContingDrawing *self, xmlNodePtr drawing_node)
{
    ContingBusPrivate *priv;
    ContingComponent *comp;
    xmlNodePtr class_node;

    g_return_val_if_fail(self != NULL && CONTING_IS_BUS(self), NULL);

    priv = CONTING_BUS_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

    class_node = xmlNewNode(NULL, BAD_CAST "class");
    xmlNewProp(class_node, BAD_CAST "name",
            BAD_CAST g_type_name(CONTING_TYPE_BUS));

    xmlAddChild(class_node, conting_util_point_node("p0", &comp->p0));
    xmlAddChild(class_node, conting_util_point_node("p1", &comp->p1));
    
    xmlAddChild(class_node, conting_util_affine_node("rotate", priv->rotate));

	xmlAddChild(class_node, conting_util_hash_node("points", priv->points,
				conting_bus_drawing_node, conting_bus_point_node, NULL));

    xmlAddChild(drawing_node, class_node);

    return CONTING_DRAWING_CLASS(parent_class)->xml_node(self, drawing_node);
}
#include <gdk/gdkkeysyms.h>
static gboolean
conting_bus_event_place(ContingDrawing *self,
		                GdkEvent *event)
{
	ContingBusPrivate *priv;
	ArtPoint p;
	gdouble affine[6];

	g_return_val_if_fail(self != NULL && CONTING_IS_BUS(self), FALSE);

    priv = CONTING_BUS_GET_PRIVATE(self);

	conting_one_line_window_to_world(conting_drawing_get_one_line(self),
			event->button.x, event->button.y,
			&p.x, &p.y);

	switch (event->type) {
		case GDK_MOTION_NOTIFY:
			art_affine_translate(affine, p.x, p.y);
			conting_drawing_affine_absolute(self, affine);
			break;
		case GDK_KEY_PRESS:
			if (event->key.keyval == GDK_r) {
				gdouble rotate[6];
				art_affine_rotate(rotate, 90.0);
				art_affine_multiply(priv->rotate, priv->rotate, rotate);

//				g_signal_emit_by_name(self, "move");

				conting_drawing_update(self);
			} else if (event->key.keyval == GDK_Escape) {
				conting_drawing_delete(self);
			}
			break;
		default:
			break;
	}
	return TRUE;
}
static void
conting_bus_get_points_bounds(ContingBus *self,
			                        ArtDRect *bounds)
{
	ContingBusPrivate *priv;
	GSList *n;
	ArtPoint *p;

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

	p = g_hash_table_lookup(priv->points, priv->links->data);
	bounds->x0 = bounds->x1 = p->x;
	bounds->y0 = bounds->y1 = p->y;

	for (n = g_slist_next(priv->links); n != NULL; n = g_slist_next(n)) {
		p = g_hash_table_lookup(priv->points, n->data);

		if (p->x < bounds->x0)
			bounds->x0 = p->x;
		else if (p->x > bounds->x1)
			bounds->x1 = p->x;

		if (p->y < bounds->y0)
			bounds->y0 = p->y;
		else if (p->y > bounds->y1)
			bounds->y1 = p->y;
	}
}

static gboolean
conting_bus_event(ContingDrawing *self,
		                GdkEvent *event)
{
	ContingBusPrivate *priv;
    ContingComponent *comp;
	ArtPoint p;

	g_return_val_if_fail(self != NULL && CONTING_IS_BUS(self), FALSE);

    priv = CONTING_BUS_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

	if (!conting_drawing_is_placed(self))
		return conting_bus_event_place(self, event);

	conting_one_line_window_to_world(conting_drawing_get_one_line(self),
			event->button.x, event->button.y,
			&p.x, &p.y);

	switch (event->type) {
		case GDK_BUTTON_PRESS:
			conting_drawing_set_selected(self, TRUE);
			conting_drawing_grab(self);
			priv->dragging_point = p;
			priv->dragging = TRUE;
			break;
		case GDK_MOTION_NOTIFY:
			if (priv->dragging && priv->start_resize != NULL) {
				ArtPoint pi;
				ArtDRect p_bounds;

                conting_drawing_w2i(self, &pi, &p);

				/* Cluttered code, checks if the size of the bar is at
				 * minimum 20, and if it doesn't goes outside linked lines */
				if ((priv->start_resize == &comp->p0
					   && fabs(pi.y - comp->p1.y) > 20)
						|| (priv->start_resize == &comp->p1
							&& fabs(pi.y - comp->p0.y) > 20)) {
					if (priv->links) {
						conting_bus_get_points_bounds(
								CONTING_BUS(self), &p_bounds);
						g_print("p_bounds: (%lf, %lf); (%lf, %lf)\n",
								p_bounds.x0, p_bounds.y0,
								p_bounds.x1, p_bounds.y1);
						g_print("p: (%lf, %lf)\n", p.x, p.y);
						if ((priv->start_resize == &comp->p0
									&& p_bounds.y0 > pi.y)
								|| (priv->start_resize == &comp->p1
									&& p_bounds.y1 < pi.y)) {
							priv->start_resize->y = pi.y;
						}
					} else {
						priv->start_resize->y = pi.y;
					}
				}

				conting_drawing_update(self);
			} else if (priv->dragging) {
				gdouble affine[6];
				art_affine_translate(affine,
						p.x - priv->dragging_point.x,
						p.y - priv->dragging_point.y);
				conting_drawing_affine(self, affine);
				g_signal_emit_by_name(self, "move");
				priv->dragging_point = p;
			} else {
				ArtPoint pi;

                conting_drawing_w2i(self, &pi, &p);

				if (fabs(pi.y - comp->p0.y) < TOLERANCE) {
					priv->start_resize = &comp->p0;
				} else if (fabs(pi.y - comp->p1.y) < TOLERANCE) {
					priv->start_resize = &comp->p1;
				} else {
					priv->start_resize = NULL;
				}
			}
			break;
		case GDK_BUTTON_RELEASE:
			if (priv->dragging) {
				priv->dragging = FALSE;
				priv->start_resize = NULL;
				conting_drawing_ungrab(self);
			}
			break;
		case GDK_KEY_PRESS:
			if (event->key.keyval == GDK_r) {
				gdouble rotate[6];
				art_affine_rotate(rotate, 90.0);
				art_affine_multiply(priv->rotate, priv->rotate, rotate);

				g_signal_emit_by_name(self, "move");

				conting_drawing_update(self);
			} else if (event->key.keyval == GDK_Delete) {
				conting_drawing_delete(self);
			}
			break;
		default:
			return FALSE;
	}

	return TRUE;
}


static void
conting_bus_link_deleted(ContingDrawing *drawing,
		                       gpointer user_data)
{
	ContingBusPrivate *priv;

	g_return_if_fail(user_data != NULL && CONTING_IS_BUS(user_data));

	priv = CONTING_BUS_GET_PRIVATE(user_data);

	g_print("link %p deleted from %p\n", drawing, user_data);
	priv->links = g_slist_remove(priv->links, drawing);
	conting_bus_disconnect_link(CONTING_BUS(user_data), drawing);
}

static gboolean
conting_bus_link(ContingComponent *self,
		               ContingDrawing *drawing,
					   gdouble world_x, gdouble world_y,
					   ArtPoint *pw)
{
	ContingBusPrivate *priv;
    ContingComponent *comp;
	ArtPoint pi;

	g_return_val_if_fail(self != NULL && CONTING_IS_BUS(self), FALSE);

    priv = CONTING_BUS_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

	if (g_slist_find(priv->links, drawing))
		return FALSE;

	pi.x = world_x;
	pi.y = world_y;

    conting_drawing_w2i(CONTING_DRAWING(self), &pi, &pi);
	
	g_print("link: (%lf, %lf); (%lf, %lf) : (%lf, %lf)\n",
			comp->p0.x, comp->p0.y, comp->p1.x, comp->p1.y,
			pi.x, pi.y);

	if (pi.y < comp->p0.y || pi.y > comp->p1.y
			|| pi.x < comp->p0.x || pi.x > comp->p1.x) {
		return FALSE;
	}

	if (fabs(pi.x - comp->p0.x) < fabs(pi.x - comp->p1.x)) {
//		art_affine_translate(affine, priv->p0.x - pi.x, 0);
		pi.x = comp->p0.x;
	} else {
//		art_affine_translate(affine, priv->p1.x - pi.x, 0);
		pi.x = comp->p1.x;
	}

    conting_drawing_i2w(CONTING_DRAWING(self), pw, &pi);

	ArtPoint *p = g_new(ArtPoint, 1);
	*p = pi;

	priv->links = g_slist_append(priv->links, drawing);
	g_hash_table_insert(priv->points, drawing, p); 
	g_signal_connect(G_OBJECT(drawing), "delete",
			G_CALLBACK(conting_bus_link_deleted), self);

	return TRUE;
}
static void
conting_bus_get_i2w_affine(ContingDrawing *self,
		                     gdouble affine[6])
{
	ContingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

	CONTING_DRAWING_CLASS(parent_class)->get_i2w_affine(self, affine);

	art_affine_multiply(affine, priv->rotate, affine);
}
static void
conting_bus_get_w2i_affine(ContingDrawing *self,
		                     gdouble affine[6])
{
	ContingBusPrivate *priv;
    gdouble invert_drawing[6];
    gdouble invert_rotate[6];

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

    CONTING_DRAWING_CLASS(parent_class)->get_w2i_affine(self, invert_drawing);

    art_affine_invert(invert_rotate, priv->rotate);

    art_affine_multiply(affine, invert_drawing, invert_rotate);
}

static void
conting_bus_class_init(gpointer g_class, gpointer class_data)
{
    ContingDrawingClass *drawing_class;
	ContingComponentClass *component_class;
	GObjectClass *gobject_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_bus_draw;
	drawing_class->event = conting_bus_event;
	drawing_class->get_i2w_affine = conting_bus_get_i2w_affine;
	drawing_class->get_w2i_affine = conting_bus_get_w2i_affine;
	drawing_class->delete = conting_bus_delete;
    drawing_class->xml_node = conting_bus_xml_node;
    drawing_class->place_xml = conting_bus_place_xml;

	component_class = CONTING_COMPONENT_CLASS(g_class);
	component_class->link = conting_bus_link;
	component_class->get_link_point = conting_bus_get_link_point;

	gobject_class = G_OBJECT_CLASS(g_class);
	gobject_class->finalize = conting_bus_finalize;

	g_type_class_add_private(g_class, sizeof(ContingBusPrivate));

	parent_class = g_type_class_peek_parent(g_class);
}

GType conting_bus_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingBusClass),
            NULL,
            NULL,
            conting_bus_class_init,
            NULL,
            NULL,
            sizeof(ContingBus),
            0,
            conting_bus_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_COMPONENT,
                "ContingBus",
                &type_info, 0);
    }

    return type;
}
