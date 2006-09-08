#include "contingsymbol.h"
#include "contingserializable.h"
#include "contingxml.h"

static gpointer parent_class = NULL;
static gpointer parent_iface = NULL;

static void
conting_symbol_delete(ContingDrawing *self)
{
	ContingSymbol *symb;
	
    g_print("DELETING ContingSymbol\n");


	g_return_if_fail(self != NULL && CONTING_IS_SYMBOL(self));

	symb = CONTING_SYMBOL(self);

	symb->link0 = NULL;

	CONTING_DRAWING_CLASS(parent_class)->delete(self);
}

static void
conting_symbol_link_deleted(ContingComponent *self,
							ContingDrawing *drawing)
{
	ContingSymbol *symb;

	g_return_if_fail(self != NULL && CONTING_IS_SYMBOL(self));

	symb = CONTING_SYMBOL(self);

	if (drawing == symb->link0) {
		symb->link0 = NULL;
	}
}

static gboolean
conting_symbol_link(ContingComponent *self,
                    ContingDrawing *drawing,
                    gdouble world_x, gdouble world_y,
                    ArtPoint *pw)
{
    ContingSymbol *symb;
    ContingComponent *comp;
    ArtPoint pi;

    g_return_val_if_fail(self != NULL && CONTING_IS_SYMBOL(self), FALSE);

    symb = CONTING_SYMBOL(self);
    comp = CONTING_COMPONENT(self);

	if (g_list_find(comp->links, drawing))
		return FALSE;

	if (symb->link0)
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
	symb->link0 = drawing;

	/* pw is the paramter */
    conting_drawing_i2w(CONTING_DRAWING(self), pw, &pi);

	conting_component_connect_link(self, drawing, &pi);

    return TRUE;
}
static void
conting_symbol_write(ContingSerializable *self,
                  xmlNodePtr drawing_node,
				  xmlNodePtr *result)
{
	ContingSymbol *symb;
    ContingComponent *comp;
	xmlNodePtr class_node;

	g_return_if_fail(self != NULL && CONTING_IS_SYMBOL(self));

	symb = CONTING_SYMBOL(self);
    comp = CONTING_COMPONENT(self);

	class_node = xmlNewNode(NULL, BAD_CAST "class");
	xmlNewProp(class_node, BAD_CAST "name",
			BAD_CAST g_type_name(CONTING_TYPE_SYMBOL));

	if (symb->link0)
		xmlAddChild(class_node,
				conting_util_drawing_node("link0", symb->link0));

	xmlAddChild(drawing_node, class_node);

	((ContingSerializableClass *) parent_iface)->write(self, drawing_node, result);
}
static void
conting_symbol_read(ContingSerializable *self, xmlNodePtr drawing_node,
                 GHashTable *id_drawing)
{
    ContingSymbol *symb;
    ContingComponent *comp;
    xmlNodePtr class_node;

    g_return_if_fail(self != NULL && CONTING_IS_SYMBOL(self));

    symb = CONTING_SYMBOL(self);
    comp = CONTING_COMPONENT(self);


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
                    symb->link0 = conting_util_load_drawing(attr, id_drawing);
                }

                xmlFree(name);
                xmlFree(type);
            }
        }

        if (class_name)
            xmlFree(class_name);
    }
/*
    symb->placed = TRUE;
    */

    ((ContingSerializableClass *) parent_iface)->read(self,
            drawing_node, id_drawing);

}


static void
conting_symbol_instance_init(GTypeInstance *self, gpointer g_class)
{
	ContingSymbol *symb;

	g_return_if_fail(self != NULL && CONTING_IS_SYMBOL(self));

	symb = CONTING_SYMBOL(self);
	
	symb->link0 = NULL;
}

static void
conting_symbol_serializable_init(gpointer g_iface, gpointer iface_data)
{
	ContingSerializableClass *serial_class;

	serial_class = g_iface;
	serial_class->read = conting_symbol_read;
	serial_class->write = conting_symbol_write;

	parent_iface = g_type_interface_peek_parent(g_iface);
}

static void
conting_symbol_class_init(gpointer g_class, gpointer class_data)
{
	ContingDrawingClass *drawing_class;
	ContingComponentClass *component_class;

	drawing_class = CONTING_DRAWING_CLASS(g_class);
	drawing_class->delete = conting_symbol_delete;

	component_class = CONTING_COMPONENT_CLASS(g_class);
	component_class->link_deleted = conting_symbol_link_deleted;
	component_class->link = conting_symbol_link;

	parent_class = g_type_class_peek_parent(g_class);
}

GType
conting_symbol_get_type(void)
{
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingSymbolClass),
            NULL,
            NULL,
            conting_symbol_class_init,
            NULL,
            NULL,
            sizeof(ContingSymbol),
            0,
            conting_symbol_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_COMPONENT,
                "ContingSymbol",
                &type_info, 0);

		static const GInterfaceInfo serial_info = {
			conting_symbol_serializable_init,
			NULL,
			NULL	
		};

		g_type_add_interface_static(type,
				CONTING_TYPE_SERIALIZABLE,
				&serial_info);

    }

    return type;
}
