#include "contingdrawing.h"
#include "contingtrans3.h"
#include "contingutil.h"
#include "contingxml.h"
#include "contingserializable.h"

#include <string.h>
#include <math.h>
#include <assert.h>

#define CONTING_TRANS3_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_TRANS3, ContingTrans3Private))

#define TOLERANCE 3
#define SIZE ((TOLERANCE * 2) - 1)

static gpointer parent_class = NULL;
static gpointer parent_iface = NULL;

typedef struct ContingTrans3Private_ ContingTrans3Private;
struct ContingTrans3Private_ {
    ContingDrawing *link0, *link1, *link2;
};

static void
conting_trans3_draw(ContingDrawing *self, cairo_t *cr)
{
    ContingTrans3Private *priv;
    ContingComponent *comp;
    ArtPoint pw0, pw1;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS3(self));

    priv = CONTING_TRANS3_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

    pw0 = comp->p0;
    pw1 = comp->p1;
    pw1.y = pw0.y + 2 * fabs(pw1.y - pw0.y) / 3.0;
    pw1.x = pw0.x + 2 * fabs(pw1.x - pw0.x) / 3.0;

    cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);

    cairo_arc(cr,
            pw0.x + (pw1.x - pw0.x) / 2.0,
            pw0.y + (pw1.y - pw0.y) / 2.0,
            (pw1.x - pw0.x) / 2.0, 0, 2 * M_PI);
    cairo_stroke(cr);

    pw0 = comp->p0;
    pw1 = comp->p1;
    pw0.x = pw0.x + (pw1.x - pw0.x) / 3.0;
    pw1.y = pw1.y - (pw1.y - pw0.y) / 3.0;

    cairo_arc(cr,
            pw0.x + (pw1.x - pw0.x) / 2.0,
            pw0.y + (pw1.y - pw0.y) / 2.0,
            (pw1.x - pw0.x) / 2.0, 0, 2 * M_PI);
    cairo_stroke(cr);

    /* Low winding */
    
    pw0 = comp->p0;
    pw1 = comp->p1;
    pw0.x = pw0.x + (pw1.x - pw0.x) / 6;
    pw0.y = pw0.y + (pw1.y - pw0.y) / 3;
    pw1.x = pw1.x - (pw1.x - pw0.x) / 6;

    cairo_arc(cr,
            pw0.x + (pw1.x - pw0.x) / 2.0,
            pw0.y + (pw1.y - pw0.y) / 2.0,
            (pw1.x - pw0.x) / 2.0, 0, 2 * M_PI);
    cairo_stroke(cr);

    CONTING_DRAWING_CLASS(parent_class)->draw(self, cr);
}

static void
conting_trans3_accept(ContingDrawing *self, ContingVisitor *visitor)
{
	g_return_if_fail(self != NULL && CONTING_IS_TRANS3(self));

	conting_visitor_visit_trans3(visitor, CONTING_TRANS3(self));
}

static void
conting_trans3_finalize(GObject *self)
{
    ContingTrans3Private *priv;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS3(self));

    priv = CONTING_TRANS3_GET_PRIVATE(self);

    G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void
conting_trans3_instance_init(GTypeInstance *self,
                           gpointer g_class)
{
    ContingTrans3Private *priv;
    ContingComponent *comp;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS3(self));

    priv = CONTING_TRANS3_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

    comp->p0.x = -8.5;
    comp->p0.y = -8.5;
    comp->p1.x = 8.5;
    comp->p1.y = 8.5;

    priv->link0 = priv->link1 = priv->link2 = NULL;
}
static void conting_trans3_delete(ContingDrawing *self)
{
    ContingTrans3Private *priv;

    g_print("DELETING\n");

    g_return_if_fail(self != NULL && CONTING_IS_TRANS3(self));

    priv = CONTING_TRANS3_GET_PRIVATE(self);

    priv->link0 = priv->link1 = NULL;

    CONTING_DRAWING_CLASS(parent_class)->delete(self);
}


static void
conting_trans3_link_deleted(ContingComponent *comp,
                            ContingDrawing *drawing)
{
    ContingTrans3Private *priv;

    g_return_if_fail(comp != NULL && CONTING_IS_TRANS3(comp));

    priv = CONTING_TRANS3_GET_PRIVATE(comp);

    g_print("link %p deleted from %p\n", drawing, comp);
    if (drawing == priv->link0) {
        priv->link0 = NULL;
    } else if (drawing == priv->link1) {
        priv->link1 = NULL;
    } else if (drawing == priv->link2) {
        priv->link2 = NULL;
    } else {
        return;
    }

    CONTING_COMPONENT_CLASS(parent_class)->link_deleted(comp, drawing);
}

static gboolean
conting_trans3_link(ContingComponent *self,
                    ContingDrawing *drawing,
                    gdouble world_x, gdouble world_y,
                    ArtPoint *pw)
{
    ContingTrans3Private *priv;
    ContingComponent *comp;
    ArtPoint pi;

    g_return_val_if_fail(self != NULL && CONTING_IS_TRANS3(self), FALSE);

    priv = CONTING_TRANS3_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

    if (g_list_find(comp->links, drawing))
        return FALSE;
    g_print("didn't find!\n");

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
    g_print("within bounds!\n");
    g_print("link0 = %p\tlink1 = %p\n", priv->link0, priv->link1);

    if (fabs(pi.y - comp->p0.y) < fabs(pi.y - comp->p1.y)) {
        pi.y = comp->p0.y + (comp->p1.y - comp->p0.y) / 3.0;
        if (fabs(pi.x - comp->p0.x) < fabs(pi.x - comp->p1.x)
                && priv->link0 == NULL) {
//        art_affine_translate(affine, comp->p0.x - pi.x, 0);
            pi.x = comp->p0.x;
            priv->link0 = drawing;
        } else if (priv->link1 == NULL) {
            pi.x = comp->p1.x;
            priv->link1 = drawing;
        } else {
			return FALSE;
		}
    } else if (priv->link2 == NULL) {
//        art_affine_translate(affine, comp->p1.x - pi.x, 0);
        pi.y = comp->p1.y;
        pi.x = comp->p0.x + (comp->p1.x - comp->p0.x) / 2.0;
        priv->link2 = drawing;
    } else {
        return FALSE;
    }

    /* pw is the paramter */
    conting_drawing_i2w(CONTING_DRAWING(self), pw, &pi);

    conting_component_connect_link(self, drawing, &pi);

    return TRUE;
}

static void
conting_trans3_xml_node(ContingSerializable *self,
                        xmlNodePtr drawing_node,
                        xmlNodePtr *result)
{
    ContingTrans3Private *priv;
    ContingComponent *comp;
    xmlNodePtr class_node;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS3(self));

    priv = CONTING_TRANS3_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

    class_node = xmlNewNode(NULL, BAD_CAST "class");
    xmlNewProp(class_node, BAD_CAST "name",
            BAD_CAST g_type_name(CONTING_TYPE_TRANS3));

    if (priv->link0)
        xmlAddChild(class_node,
                conting_util_drawing_node("link0", priv->link0));
    if (priv->link1)
        xmlAddChild(class_node,
                conting_util_drawing_node("link1", priv->link1));
    if (priv->link2)
        xmlAddChild(class_node,
                conting_util_drawing_node("link2", priv->link2));

    xmlAddChild(drawing_node, class_node);

    ((ContingSerializableClass *) parent_iface)->write(self, drawing_node, result);
}
static void
conting_trans3_place_xml(ContingSerializable *self, xmlNodePtr drawing_node,
                         GHashTable *id_drawing)
{
    ContingTrans3Private *priv;
    ContingComponent *comp;
    xmlNodePtr class_node;

    g_return_if_fail(self != NULL && CONTING_IS_TRANS3(self));

    priv = CONTING_TRANS3_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

    g_print("conting_trans3_place_xml()\n");

    for (class_node = drawing_node->children; class_node;
            class_node = class_node->next) {
        xmlChar *class_name;

        if (!xmlStrEqual(class_node->name, BAD_CAST "class"))
            continue;

        
        class_name = xmlGetProp(class_node, BAD_CAST "name");

        if (class_name && xmlStrEqual(class_name, "ContingTrans3")) {
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
                } else if (xmlStrEqual(type, BAD_CAST "drawing")
                        && xmlStrEqual(name, BAD_CAST "link2")) {
                    priv->link2 = conting_util_load_drawing(attr, id_drawing);
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
conting_trans3_serializable_init(gpointer g_iface, gpointer iface_data)
{
    ContingSerializableClass *serial_class;

    serial_class = g_iface;
    serial_class->write = conting_trans3_xml_node;
    serial_class->read = conting_trans3_place_xml;

    parent_iface = g_type_interface_peek_parent(g_iface);
}

static void
conting_trans3_class_init(gpointer g_class, gpointer class_data)
{
    ContingDrawingClass *drawing_class;
    ContingComponentClass *component_class;
    GObjectClass *gobject_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_trans3_draw;
    drawing_class->delete = conting_trans3_delete;

	drawing_class->accept = conting_trans3_accept;

    component_class = CONTING_COMPONENT_CLASS(g_class);
    component_class->link = conting_trans3_link;
    component_class->link_deleted = conting_trans3_link_deleted;

    gobject_class = G_OBJECT_CLASS(g_class);
    gobject_class->finalize = conting_trans3_finalize;

    g_type_class_add_private(g_class, sizeof(ContingTrans3Private));

    parent_class = g_type_class_peek_parent(g_class);
}

GType conting_trans3_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingTrans3Class),
            NULL,
            NULL,
            conting_trans3_class_init,
            NULL,
            NULL,
            sizeof(ContingTrans3),
            0,
            conting_trans3_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_BUS_BASE,
                "ContingTrans3",
                &type_info, 0);

        static const GInterfaceInfo serial_info = {
            conting_trans3_serializable_init,
            NULL,
            NULL    
        };

        g_type_add_interface_static(type,
                CONTING_TYPE_SERIALIZABLE,
                &serial_info);

    }

    return type;
}
