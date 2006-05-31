#include "contingcomponent.h"

gboolean
conting_component_get_link_point(ContingComponent *self,
                                 ContingDrawing *line,
                                 ArtPoint *p)
{
	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

	return CONTING_COMPONENT_GET_CLASS(self)->get_link_point(self, line, p);
}
gboolean
conting_component_link(ContingComponent *self,
		               ContingDrawing *drawing,
					   gdouble world_x, gdouble world_y,
					   ArtPoint *pw)
{
	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

	return CONTING_COMPONENT_GET_CLASS(self)->link(self, drawing,
			world_x, world_y, pw);
}
static void
conting_component_class_init(gpointer g_class, gpointer class_data)
{
	ContingComponentClass *component_class;

	component_class = CONTING_COMPONENT_CLASS(g_class);
	component_class->link = NULL;
	component_class->get_link_point = NULL;
}

GType conting_component_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingComponentClass),
            NULL,
            NULL,
            conting_component_class_init,
            NULL,
            NULL,
            sizeof(ContingComponent),
            0,
            NULL,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_DRAWING,
                "ContingComponent",
                &type_info, 0);
    }

    return type;
}
