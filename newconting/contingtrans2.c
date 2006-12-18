#include "contingdrawing.h"
#include "contingtrans2.h"
#include "contingutil.h"
#include "contingxml.h"
#include "contingserializable.h"


#include <string.h>
#include <assert.h>
#include <math.h>

enum {
	CONTING_TRANS2_PROP_0,
	CONTING_TRANS2_PROP_COLOR0,
	CONTING_TRANS2_PROP_COLOR1
};

#define CONTING_TRANS2_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_TRANS2, ContingTrans2Private))

#define TOLERANCE 3
#define SIZE ((TOLERANCE * 2) - 1)

static gpointer parent_class = NULL;
static gpointer parent_iface = NULL;

typedef struct ContingTrans2Private_ ContingTrans2Private;
struct ContingTrans2Private_ {
    gboolean dragging;
    ArtPoint dragging_point;

    ContingDrawing *link0, *link1;
	GdkColor color0, color1;
};

static void
conting_trans2_draw_ltc(ContingDrawing *self, cairo_t *cr)
{
    ContingTrans2Private *priv;
    ContingComponent *comp;
	ArtPoint p0, p1;

	g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

	priv = CONTING_TRANS2_GET_PRIVATE(self);
	comp = CONTING_COMPONENT(self);

	p0.x = comp->p0.x;
   	p1.x = comp->p1.x;

	p0.y = comp->p0.y + fabs(comp->p1.y - comp->p0.y) / 2.0
		- fabs(comp->p1.y - comp->p0.y) / 3.0;
	p1.y = comp->p0.y + fabs(comp->p1.y - comp->p0.y) / 2.0
		+ fabs(comp->p1.y - comp->p0.y) / 3.0;

	cairo_move_to(cr, p0.x, p0.y);
	cairo_line_to(cr, p1.x, p1.y);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_stroke(cr);
}

static void
conting_trans2_draw(ContingDrawing *self, cairo_t *cr)
{
    ContingTrans2Private *priv;
    ContingComponent *comp;
    ArtPoint pw0, pw1;
    GdkRectangle rect;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

    pw0 = comp->p0;
    pw1 = comp->p1;
    pw1.y = pw1.y - (pw1.y - pw0.y) / 3;

    rect.x = (pw0.x < pw1.x ? pw0.x : pw1.x);
    rect.y = (pw0.y < pw1.y ? pw0.y : pw1.y);
    rect.width = fabs(pw1.x - pw0.x);
    rect.height = fabs(pw1.y - pw0.y);

	cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);

	cairo_arc(cr,
			rect.x + ((gdouble) rect.width / 2.0),
			rect.y + ((gdouble) rect.height / 2.0),
			(gdouble) rect.width / 2.0, 0, 2 * M_PI);
	cairo_set_source_rgb(cr,
			priv->color0.red / (gdouble) G_MAXUINT16,
			priv->color0.green / (gdouble) G_MAXUINT16,
			priv->color0.blue / (gdouble) G_MAXUINT16);
	cairo_stroke(cr);

    pw0 = comp->p0;
    pw1 = comp->p1;
    pw0.y = pw0.y + (pw1.y - pw0.y) / 3;

    rect.x = (pw0.x < pw1.x ? pw0.x : pw1.x);
    rect.y = (pw0.y < pw1.y ? pw0.y : pw1.y);
    rect.width = fabs(pw1.x - pw0.x);
    rect.height = fabs(pw1.y - pw0.y);


	cairo_arc(cr,
			rect.x + ((gdouble) rect.width / 2.0),
			rect.y + ((gdouble) rect.height / 2.0),
			(gdouble) rect.width / 2.0, 0, 2 * M_PI);
	cairo_set_source_rgb(cr,
			priv->color1.red / (gdouble) G_MAXUINT16,
			priv->color1.green / (gdouble) G_MAXUINT16,
			priv->color1.blue / (gdouble) G_MAXUINT16);
	cairo_stroke(cr);

	if (conting_drawing_get_attr(self, "min tap")
			|| conting_drawing_get_attr(self, "max tap")) {
		cairo_save(cr);
		conting_trans2_draw_ltc(self, cr);
		cairo_restore(cr);
	}

	CONTING_DRAWING_CLASS(parent_class)->draw(self, cr);
}

static void
conting_trans2_accept(ContingDrawing *self, ContingVisitor *visitor)
{
	g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

	conting_visitor_visit_trans2(visitor, CONTING_TRANS2(self));
}

void
conting_trans2_get_buses(ContingTrans2 *self,
		ContingBus **bus0, ContingBus **bus1)
{
	ContingTrans2Private *priv;
	ContingDrawing *stub;

	g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

	priv = CONTING_TRANS2_GET_PRIVATE(self);

	/* TODO: check if is it correct.
	 * That means, if conting_line_get_buses() applied on link0
	 * returns bus1 and bus0 in this order */
	g_return_if_fail(priv->link0 != NULL && CONTING_IS_LINE(priv->link0));
	conting_line_get_buses(CONTING_LINE(priv->link0), bus1, bus0);
}


static void
conting_trans2_set_property(GObject *self,
                         guint prop_id,
                         const GValue *value,
                         GParamSpec *pspec)
{
    ContingTrans2Private *priv;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);

    switch (prop_id) {
        case CONTING_TRANS2_PROP_COLOR0:
            priv->color0 = *((GdkColor *) g_value_get_pointer(value));
            break;
        case CONTING_TRANS2_PROP_COLOR1:
            priv->color1 = *((GdkColor *) g_value_get_pointer(value));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

static void
conting_trans2_get_property(GObject *self,
                         guint prop_id,
                         GValue *value,
                         GParamSpec *pspec)
{
    ContingTrans2Private *priv;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);

    switch (prop_id) {
        case CONTING_TRANS2_PROP_COLOR0:
            g_value_set_pointer(value, &priv->color0);
            break;
        case CONTING_TRANS2_PROP_COLOR1:
            g_value_set_pointer(value, &priv->color1);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}
static void
conting_trans2_finalize(GObject *self)
{
    ContingTrans2Private *priv;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void
conting_trans2_instance_init(GTypeInstance *self,
                           gpointer g_class)
{
    ContingTrans2Private *priv;
    ContingComponent *comp;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

    comp->p0.x = -5;
    comp->p0.y = -8.5;
    comp->p1.x = 5;
    comp->p1.y = 8.5;

    /*
    priv->placed = FALSE;
    */
    priv->dragging = FALSE;

    priv->link0 = priv->link1 = NULL;
}
static void conting_trans2_delete(ContingDrawing *self)
{
    ContingTrans2Private *priv;

    g_print("DELETING\n");

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);

	priv->link0 = priv->link1 = NULL;

	CONTING_DRAWING_CLASS(parent_class)->delete(self);
}


static void
conting_trans2_link_deleted(ContingComponent *comp,
                            ContingDrawing *drawing)
{
    ContingTrans2Private *priv;

    g_return_if_fail(comp != NULL && CONTING_IS_TRANS2(comp));

    priv = CONTING_TRANS2_GET_PRIVATE(comp);

    g_print("link %p deleted from %p\n", drawing, comp);
	if (drawing == priv->link0) {
		priv->link0 = NULL;
	} else if (drawing == priv->link1) {
		priv->link1 = NULL;
	} else {
		return;
	}

	CONTING_COMPONENT_CLASS(parent_class)->link_deleted(comp, drawing);
}

static gboolean
conting_trans2_link(ContingComponent *self,
                    ContingDrawing *drawing,
                    gdouble world_x, gdouble world_y,
                    ArtPoint *pw)
{
    ContingTrans2Private *priv;
    ContingComponent *comp;
    ArtPoint pi;

    g_return_val_if_fail(self != NULL && CONTING_IS_TRANS2(self), FALSE);

    priv = CONTING_TRANS2_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

	if (g_list_find(comp->links, drawing))
		return FALSE;
	g_print("didn't find!\n");

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
	g_print("within bounds!\n");
	g_print("link0 = %p\tlink1 = %p\n", priv->link0, priv->link1);

    if (fabs(pi.y - comp->p0.y) < fabs(pi.y - comp->p1.y)
			&& priv->link0 == NULL) {
//        art_affine_translate(affine, comp->p0.x - pi.x, 0);
        pi.y = comp->p0.y;
		priv->link0 = drawing;
    } else if (priv->link1 == NULL) {
//        art_affine_translate(affine, comp->p1.x - pi.x, 0);
        pi.y = comp->p1.y;
		priv->link1 = drawing;
    } else {
		return FALSE;
	}
	pi.x = 0;

	/* pw is the paramter */
    conting_drawing_i2w(CONTING_DRAWING(self), pw, &pi);

	conting_component_connect_link(self, drawing, &pi);

    return TRUE;
}

static void
conting_trans2_xml_node(ContingSerializable *self,
		                xmlNodePtr drawing_node,
						xmlNodePtr *result)
{
	ContingTrans2Private *priv;
    ContingComponent *comp;
	xmlNodePtr class_node;

	g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

	priv = CONTING_TRANS2_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

	class_node = xmlNewNode(NULL, BAD_CAST "class");
	xmlNewProp(class_node, BAD_CAST "name",
			BAD_CAST g_type_name(CONTING_TYPE_TRANS2));

	if (priv->link0)
		xmlAddChild(class_node,
				conting_util_drawing_node("link0", priv->link0));
	if (priv->link1)
		xmlAddChild(class_node,
				conting_util_drawing_node("link1", priv->link1));

	xmlAddChild(drawing_node, class_node);

	((ContingSerializableClass *) parent_iface)->write(self, drawing_node, result);
}
static void
conting_trans2_place_xml(ContingSerializable *self, xmlNodePtr drawing_node,
                         GHashTable *id_drawing)
{
    ContingTrans2Private *priv;
    ContingComponent *comp;
    xmlNodePtr class_node;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS2(self));

    priv = CONTING_TRANS2_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

    g_print("conting_trans2_place_xml()\n");

    for (class_node = drawing_node->children; class_node;
            class_node = class_node->next) {
        xmlChar *class_name;

        if (!xmlStrEqual(class_node->name, BAD_CAST "class"))
            continue;

        
        class_name = xmlGetProp(class_node, BAD_CAST "name");

        if (class_name && xmlStrEqual(class_name, "ContingTrans2")) {
            xmlNodePtr attr;

            for (attr = class_node->children; attr; attr = attr->next) {
                xmlChar *name, *type;

                if (!xmlStrEqual(attr->name, BAD_CAST "attribute"))
                    continue;

                name = xmlGetProp(attr, BAD_CAST "name");
                type = xmlGetProp(attr, BAD_CAST "type");

                printf("type = %s\tname = %s\n", type, name);

                if (xmlStrEqual(type, BAD_CAST "drawing")
                        && xmlStrEqual(name, BAD_CAST "link0")) {
                    priv->link0 = conting_util_load_drawing(attr, id_drawing);
                } else if (xmlStrEqual(type, BAD_CAST "drawing")
                        && xmlStrEqual(name, BAD_CAST "link1")) {
                    priv->link1 = conting_util_load_drawing(attr, id_drawing);
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

    ((ContingSerializableClass *) parent_iface)->read(self,
            drawing_node, id_drawing);

}

static void
conting_trans2_serializable_init(gpointer g_iface, gpointer iface_data)
{
	ContingSerializableClass *serial_class;

	serial_class = g_iface;
	serial_class->write = conting_trans2_xml_node;
	serial_class->read = conting_trans2_place_xml;

	parent_iface = g_type_interface_peek_parent(g_iface);
}

static void
conting_trans2_class_init(gpointer g_class, gpointer class_data)
{
    ContingDrawingClass *drawing_class;
    ContingComponentClass *component_class;
    GObjectClass *gobject_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_trans2_draw;
    drawing_class->delete = conting_trans2_delete;

	drawing_class->accept = conting_trans2_accept;
	
    component_class = CONTING_COMPONENT_CLASS(g_class);
    component_class->link = conting_trans2_link;
    component_class->link_deleted = conting_trans2_link_deleted;

    gobject_class = G_OBJECT_CLASS(g_class);
    gobject_class->finalize = conting_trans2_finalize;
    gobject_class->get_property = conting_trans2_get_property;
    gobject_class->set_property = conting_trans2_set_property;
	
	g_object_class_install_property(gobject_class,
			CONTING_TRANS2_PROP_COLOR0,
			g_param_spec_pointer("color0",
								 "0 color",
								 "0 color",
								 G_PARAM_READABLE | G_PARAM_WRITABLE));
	
	g_object_class_install_property(gobject_class,
			CONTING_TRANS2_PROP_COLOR1,
			g_param_spec_pointer("color1",
								 "1 color",
								 "1 color",
								 G_PARAM_READABLE | G_PARAM_WRITABLE));
			


    g_type_class_add_private(g_class, sizeof(ContingTrans2Private));

    parent_class = g_type_class_peek_parent(g_class);
}

GType conting_trans2_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingTrans2Class),
            NULL,
            NULL,
            conting_trans2_class_init,
            NULL,
            NULL,
            sizeof(ContingTrans2),
            0,
            conting_trans2_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_COMPONENT,
                "ContingTrans2",
                &type_info, 0);

		static const GInterfaceInfo serial_info = {
			conting_trans2_serializable_init,
			NULL,
			NULL	
		};

		g_type_add_interface_static(type,
				CONTING_TYPE_SERIALIZABLE,
				&serial_info);

    }

    return type;
}
