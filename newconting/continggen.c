#include "continggen.h"
#include "contingutil.h"
#include "contingxml.h"
#include <string.h>
#include <math.h>

#define CONTING_GEN_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_GEN, ContingGenPrivate))

#define TOLERANCE 3
#define SIZE ((TOLERANCE * 2) - 1)

static gpointer parent_class = NULL;

typedef struct ContingGenPrivate_ ContingGenPrivate;
struct ContingGenPrivate_ {
    gboolean dragging;
    ArtPoint dragging_point;

    ContingDrawing *link0;
};

static void
conting_gen_draw(ContingDrawing *self,
                    GdkDrawable *drawable,
                    const GdkRectangle *drawing_rect)
{
    ContingGenPrivate *priv;
    ContingComponent *comp;
	
    gdouble affine[6];

    ArtPoint pw0, pw1;
    GdkRectangle rect;

	cairo_t *cr;

    g_return_if_fail(self != NULL && CONTING_IS_GEN(self));

    priv = CONTING_GEN_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

    conting_drawing_get_i2w_affine(self, affine);

    pw0 = comp->p0;
    pw1 = comp->p1;

    art_affine_point(&pw0, &pw0, affine);
    art_affine_point(&pw1, &pw1, affine);

    conting_one_line_world_to_window(conting_drawing_get_one_line(self),
            pw0.x, pw0.y, &pw0.x, &pw0.y);
    conting_one_line_world_to_window(conting_drawing_get_one_line(self),
            pw1.x, pw1.y, &pw1.x, &pw1.y);

    rect.x = (pw0.x < pw1.x ? pw0.x : pw1.x);
    rect.y = (pw0.y < pw1.y ? pw0.y : pw1.y);
    rect.width = fabs(pw1.x - pw0.x);
    rect.height = fabs(pw1.y - pw0.y);

	cr = gdk_cairo_create(drawable);
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);

	cairo_arc(cr,
			rect.x + ((gdouble) rect.width / 2.0),
			rect.y + ((gdouble) rect.height / 2.0),
			(gdouble) rect.width / 2.0, 0, 2 * M_PI);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_stroke(cr);

	cairo_arc(cr,
			rect.x + ((gdouble) rect.width / 3.0),
			rect.y + ((gdouble) rect.height / 2.0),
			(gdouble) rect.width / 8.0, 0, M_PI);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_stroke(cr);

	cairo_arc(cr,
			rect.x + (2 * (gdouble) rect.width / 3.0),
			rect.y + ((gdouble) rect.height / 2.0),
			(gdouble) rect.width / 8.0, M_PI, 2 * M_PI);
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_stroke(cr);

	CONTING_DRAWING_CLASS(parent_class)->draw(self, drawable, drawing_rect);
}

static void
conting_gen_finalize(GObject *self)
{
    ContingGenPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_GEN(self));

    priv = CONTING_GEN_GET_PRIVATE(self);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void
conting_gen_instance_init(GTypeInstance *self,
                           gpointer g_class)
{
    ContingGenPrivate *priv;
    ContingComponent *comp;

    g_return_if_fail(self != NULL && CONTING_IS_GEN(self));

    priv = CONTING_GEN_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

    comp->p0.x = -5;
    comp->p0.y = -5;
    comp->p1.x = 5;
    comp->p1.y = 5;

    /*
    priv->placed = FALSE;
    */
    priv->dragging = FALSE;

    priv->link0 = NULL;
}
static void conting_gen_delete(ContingDrawing *self)
{
    ContingGenPrivate *priv;

    g_print("DELETING\n");

    g_return_if_fail(self != NULL && CONTING_IS_GEN(self));

    priv = CONTING_GEN_GET_PRIVATE(self);

	priv->link0 = NULL;

	CONTING_DRAWING_CLASS(parent_class)->delete(self);
}


static void
conting_gen_link_deleted(ContingComponent *comp,
                         ContingDrawing *drawing)
{
    ContingGenPrivate *priv;

    g_return_if_fail(comp != NULL && CONTING_IS_GEN(comp));

    priv = CONTING_GEN_GET_PRIVATE(comp);

    g_print("link %p deleted from %p\n", drawing, comp);
	if (drawing == priv->link0) {
		priv->link0 = NULL;
	} else {
		return;
	}

	CONTING_COMPONENT_CLASS(parent_class)->link_deleted(comp, drawing);
}

static gboolean
conting_gen_link(ContingComponent *self,
                    ContingDrawing *drawing,
                    gdouble world_x, gdouble world_y,
                    ArtPoint *pw)
{
    ContingGenPrivate *priv;
    ContingComponent *comp;
    ArtPoint pi;

    g_return_val_if_fail(self != NULL && CONTING_IS_GEN(self), FALSE);

    priv = CONTING_GEN_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

	g_print("g_list_find()\n");
	if (g_list_find(comp->links, drawing))
		return FALSE;

	if (priv->link0)
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

	pi.y = comp->p0.y + (comp->p1.y - comp->p0.y) / 2.0;
	pi.x = comp->p1.x;
	priv->link0 = drawing;

	/* pw is the paramter */
    conting_drawing_i2w(CONTING_DRAWING(self), pw, &pi);

	conting_component_connect_link(self, drawing, &pi);

    return TRUE;
}

static xmlNodePtr
conting_gen_xml_node(ContingDrawing *self,
		                xmlNodePtr drawing_node)
{
	ContingGenPrivate *priv;
    ContingComponent *comp;
	xmlNodePtr class_node;

	g_return_val_if_fail(self != NULL && CONTING_IS_GEN(self), NULL);

	priv = CONTING_GEN_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

	class_node = xmlNewNode(NULL, BAD_CAST "class");
	xmlNewProp(class_node, BAD_CAST "name",
			BAD_CAST g_type_name(CONTING_TYPE_GEN));

	if (priv->link0)
		xmlAddChild(class_node,
				conting_util_drawing_node("link0", priv->link0));

	xmlAddChild(drawing_node, class_node);

	return CONTING_DRAWING_CLASS(parent_class)->xml_node(self, drawing_node);
}
static void
conting_gen_place_xml(ContingDrawing *self, xmlNodePtr drawing_node,
                         GHashTable *id_drawing)
{
    ContingGenPrivate *priv;
    ContingComponent *comp;
    xmlNodePtr class_node;

    g_return_if_fail(self != NULL && CONTING_IS_GEN(self));

    priv = CONTING_GEN_GET_PRIVATE(self);
    comp = CONTING_COMPONENT(self);

    g_print("conting_gen_place_xml()\n");

    for (class_node = drawing_node->children; class_node;
            class_node = class_node->next) {
        xmlChar *class_name;

        if (!xmlStrEqual(class_node->name, BAD_CAST "class"))
            continue;

        
        class_name = xmlGetProp(class_node, BAD_CAST "name");

        if (class_name && xmlStrEqual(class_name, "ContingGen")) {
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

static void
conting_gen_get_bus(ContingDrawing *self, ContingDrawing *linked,
		ContingComponent **comp)
{
	ContingGenPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_GEN(self));

	priv = CONTING_GEN_GET_PRIVATE(self);

	*comp = NULL;
}


static void
conting_gen_class_init(gpointer g_class, gpointer class_data)
{
    ContingDrawingClass *drawing_class;
    ContingComponentClass *component_class;
    GObjectClass *gobject_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_gen_draw;
    drawing_class->delete = conting_gen_delete;
	drawing_class->xml_node = conting_gen_xml_node;
	drawing_class->place_xml = conting_gen_place_xml;

	drawing_class->get_bus = conting_gen_get_bus;

    component_class = CONTING_COMPONENT_CLASS(g_class);
    component_class->link = conting_gen_link;
    component_class->link_deleted = conting_gen_link_deleted;

    gobject_class = G_OBJECT_CLASS(g_class);
    gobject_class->finalize = conting_gen_finalize;

    g_type_class_add_private(g_class, sizeof(ContingGenPrivate));

    parent_class = g_type_class_peek_parent(g_class);
}

GType conting_gen_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingGenClass),
            NULL,
            NULL,
            conting_gen_class_init,
            NULL,
            NULL,
            sizeof(ContingGen),
            0,
            conting_gen_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_COMPONENT,
                "ContingGen",
                &type_info, 0);
    }

    return type;
}
