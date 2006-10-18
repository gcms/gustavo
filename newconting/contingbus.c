#include "contingdrawing.h"
#include "contingbus.h"
#include "contingutil.h"
#include "contingxml.h"
#include <string.h>
#include <math.h>
#include <assert.h>

/* PARENT CLASS POINTER */
static gpointer parent_class = NULL;


/* CLASS PROPERTIES */
enum {
	CONTING_BUS_PROP_0,
	CONTING_BUS_PROP_COLOR
};

/* CLASS PRIVATE DATA ACCESSOR MACRO */
#define CONTING_BUS_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_BUS, ContingBusPrivate))

/* CLASS PRIVATE DATA TYPE AND STRUCTURE */
typedef struct ContingBusPrivate_ ContingBusPrivate;
struct ContingBusPrivate_ {
	GdkColor color;
};


/* PUBLIC METHOD IMPLEMENTATION */
static void
conting_bus_draw(ContingDrawing *self, cairo_t *cr)
{
    ContingBusPrivate *priv;
    ContingComponent *comp;

    g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

    priv = CONTING_BUS_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

	cairo_set_source_rgb(cr,
			(gdouble) priv->color.red / (gdouble) G_MAXUINT16,
			(gdouble) priv->color.green / (gdouble) G_MAXUINT16,
			(gdouble) priv->color.blue / (gdouble) G_MAXUINT16);
	
	cairo_move_to(cr, comp->p0.x, comp->p0.y);
	cairo_line_to(cr, comp->p1.x, comp->p0.y);
	cairo_line_to(cr, comp->p1.x, comp->p1.y);
	cairo_line_to(cr, comp->p0.x, comp->p1.y);
	cairo_line_to(cr, comp->p0.x, comp->p0.y);

	cairo_fill(cr);
	cairo_stroke(cr);
	
	CONTING_DRAWING_CLASS(parent_class)->draw(self, cr);
}


/* TODO: remove it, useless, as it does nothing */
/* PUBLIC METHOD IMPLEMENTATION */
static void
conting_bus_finalize(GObject *self)
{
	ContingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

/* INSTANCE INIT */
static void
conting_bus_instance_init(GTypeInstance *self, gpointer g_class)
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

	comp->min_bounds.x0 = comp->p0.x;
	comp->min_bounds.x1 = comp->p1.x;
	comp->min_bounds.y0 = comp->p0.y;
	comp->min_bounds.y1 = comp->p1.y;

	comp->min_w = fabs(comp->p1.x - comp->p0.x);
	comp->min_h = fabs(comp->p1.y - comp->p0.y);
	
	comp->resize_vertical = TRUE;
}

/* TODO: remove it, as conting_bus_finalize() it does nothing */
/* PUBLIC METHOD IMPLEMENTATION */
static void
conting_bus_delete(ContingDrawing *self)
{
	ContingBusPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	priv = CONTING_BUS_GET_PRIVATE(self);

	CONTING_DRAWING_CLASS(parent_class)->delete(self);
}

/*
#include <gdk/gdkkeysyms.h>
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
*/

/* TODO: remove it. It still performs some functionality, but in practice
 * it doesn't happens as the current implementation of
 * conting_drawing_event() on ContingDrawing class answers TRUE to all
 * currently generated events. */
/* PUBLIC METHOD IMPLEMENTATION */
static gboolean
conting_bus_event(ContingDrawing *self,
		                GdkEvent *event)
{
	ContingBusPrivate *priv;
    ContingComponent *comp;
	ArtPoint p;

	g_return_val_if_fail(self != NULL && CONTING_IS_BUS(self), FALSE);

    if (CONTING_DRAWING_CLASS(parent_class)->event(self, event)) {
        return TRUE;
    }

    priv = CONTING_BUS_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

	conting_one_line_window_to_world(conting_drawing_get_one_line(self),
			event->button.x, event->button.y,
			&p.x, &p.y);

	switch (event->type) {
		case GDK_ENTER_NOTIFY:
			{
				ContingData *data;
				ContingItemData *item_data;
				const gchar *bus_name;

				g_object_get(G_OBJECT(conting_drawing_get_one_line(self)),
						"data", &data,
						NULL);

				item_data = conting_data_get(data, self);

				if (item_data == NULL)
					return TRUE;

				conting_item_data_get_attr(item_data,
						"name", &bus_name,
						NULL);

				g_print("%s\n", bus_name);

				return TRUE;
			}
			break;
		case GDK_LEAVE_NOTIFY:
			g_print("BUS LEAVE\n");
			break;
		case GDK_MOTION_NOTIFY:
			/*
			if (priv->dragging && priv->start_resize != NULL) {
				ArtPoint pi;
				ArtDRect p_bounds;

                conting_drawing_w2i(self, &pi, &p);
*/
				/* Cluttered code, checks if the size of the bar is at
				 * minimum 20, and if it doesn't goes outside linked lines */
			/*
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
			*/
			break;
		case GDK_2BUTTON_PRESS:
		case GDK_BUTTON_RELEASE:
			/*
			if (priv->dragging) {
				priv->dragging = FALSE;
				priv->start_resize = NULL;
				conting_drawing_ungrab(self);
			}
			*/
			break;
		default:
			return FALSE;
	}

	return TRUE;
}



/**
 * Attempts to link drawing to a component. The point (world_x, world_y) is
 * where the drawing wishes to connect to the component. The returned
 * point pw, is the actual point used in the link. All points are specified
 * as world coordinates.
 * 
 * @param self this component
 * @param drawing the drawing that wishes to link to this component
 * @param world_x x coordinate of the linking point
 * @param world_y y coordinate of the linking point
 * @param pw  return value to store the point that was actually used to link
 * component to drawing
 * @return TRUE if the link occurred.
 */

/* PUBLIC METHOD IMPLEMENTATION */
static gboolean
conting_bus_link(ContingComponent *self, ContingDrawing *drawing,
		gdouble world_x, gdouble world_y,
		ArtPoint *pw)
{
	ContingBusPrivate *priv;
    ContingComponent *comp;
	ArtPoint pi;

	g_return_val_if_fail(self != NULL && CONTING_IS_BUS(self), FALSE);

    priv = CONTING_BUS_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

	/* if it's already linked */
	if (conting_component_is_linked(comp, drawing))
		return FALSE;

	pi.x = world_x;
	pi.y = world_y;

    conting_drawing_w2i(CONTING_DRAWING(self), &pi, &pi);
	
	/*
	g_print("link: (%lf, %lf); (%lf, %lf) : (%lf, %lf)\n",
			comp->p0.x, comp->p0.y, comp->p1.x, comp->p1.y,
			pi.x, pi.y);
			*/

	if (pi.y < comp->p0.y || pi.y > comp->p1.y
			|| pi.x < comp->p0.x || pi.x > comp->p1.x) {
		return FALSE;
	}

	if (fabs(pi.x - comp->p0.x) < fabs(pi.x - comp->p1.x)) {
		pi.x = comp->p0.x;
	} else {
		pi.x = comp->p1.x;
	}

    conting_drawing_i2w(CONTING_DRAWING(self), pw, &pi);

	/* link it using pi as the linking point */
	conting_component_connect_link(self, drawing, &pi);

	return TRUE;
}

/* PUBLIC METHOD IMPLEMENTATION */
static void
conting_bus_accept(ContingDrawing *self, ContingVisitor *visitor)
{
	g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

	conting_visitor_visit_bus(visitor, CONTING_BUS(self));
}

/* PUBLIC METHOD IMPLEMENTATION */
static void
conting_bus_get_property(GObject *self,
                         guint prop_id,
                         GValue *value,
                         GParamSpec *pspec)
{
    ContingBusPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

    priv = CONTING_BUS_GET_PRIVATE(self);

    switch (prop_id) {
        case CONTING_BUS_PROP_COLOR:
            g_value_set_pointer(value, &priv->color);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

/* PUBLIC METHOD IMPLEMENTATION */
static void
conting_bus_set_property(GObject *self,
                         guint prop_id,
                         const GValue *value,
                         GParamSpec *pspec)
{
    ContingBusPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_BUS(self));

    priv = CONTING_BUS_GET_PRIVATE(self);

    switch (prop_id) {
        case CONTING_BUS_PROP_COLOR:
            priv->color = *((GdkColor *) g_value_get_pointer(value));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}


/* CLASS INIT */
static void
conting_bus_class_init(gpointer g_class, gpointer class_data)
{
    ContingDrawingClass *drawing_class;
	ContingComponentClass *component_class;
	GObjectClass *gobject_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_bus_draw;
	drawing_class->event = conting_bus_event;
	drawing_class->delete = conting_bus_delete;

	drawing_class->accept = conting_bus_accept;

	component_class = CONTING_COMPONENT_CLASS(g_class);
	component_class->link = conting_bus_link;

	gobject_class = G_OBJECT_CLASS(g_class);
	gobject_class->finalize = conting_bus_finalize;

	gobject_class->set_property = conting_bus_set_property;
	gobject_class->get_property = conting_bus_get_property;
	g_object_class_install_property(gobject_class,
			CONTING_BUS_PROP_COLOR,
			g_param_spec_pointer("color",
								 "Bus color",
								 "bus color",
								 G_PARAM_READABLE | G_PARAM_WRITABLE));
			

	g_type_class_add_private(g_class, sizeof(ContingBusPrivate));

	parent_class = g_type_class_peek_parent(g_class);
}

/* GET TYPE */
GType
conting_bus_get_type(void) {
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

        type = g_type_register_static(CONTING_TYPE_BUS_BASE,
                "ContingBus",
                &type_info, 0);
    }

    return type;
}
