#include "contingcomponent.h"


#define TOLERANCE 3
#define SIZE ((TOLERANCE * 2) - 1)

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
conting_component_get_bounds(ContingDrawing *self,
                             ArtDRect *bounds)
{
	ContingComponent *comp;
	gdouble affine[6];
	ArtPoint pw0, pw1;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    comp = CONTING_COMPONENT(self);

	conting_drawing_get_i2w_affine(self, affine);

	art_affine_point(&pw0, &comp->p0, affine);
	art_affine_point(&pw1, &comp->p1, affine);

	bounds->x0 = MIN(pw0.x, pw1.x);
	bounds->y0 = MIN(pw0.y, pw1.y);
	bounds->x1 = MAX(pw0.x, pw1.x);
	bounds->y1 = MAX(pw0.y, pw1.y);

	bounds->x0 -= TOLERANCE;
	bounds->x1 += TOLERANCE;
	bounds->y0 -= TOLERANCE;
	bounds->y1 += TOLERANCE;
}
static void
conting_component_place(ContingDrawing *self)
{
	ContingComponent *comp;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    comp = CONTING_COMPONENT(self);

	comp->placed = TRUE;

	conting_drawing_set_selected(self, TRUE);
}
static gboolean
conting_component_is_placed(ContingDrawing *self)
{
	ContingComponent *comp;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

    comp = CONTING_COMPONENT(self);

	return comp->placed;
}
static gboolean
conting_component_answer(ContingDrawing *self,
                         gdouble world_x, gdouble world_y)
{
    ArtDRect bounds;

    g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

    conting_drawing_get_bounds(self, &bounds);

	return world_x >= bounds.x0 && world_x <= bounds.x1
		&& world_y >= bounds.y0 && world_y <= bounds.y1;
}
static void
conting_component_place_xml(ContingDrawing *self, xmlNodePtr drawing_node,
                            GHashTable *id_drawing)
{
    ContingComponent *comp;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    /* Load p0, p1 */
    
    comp = CONTING_COMPONENT(self);
    comp->placed = TRUE;
}

static void
conting_component_instance_init(GTypeInstance *self, gpointer g_class)
{
    ContingComponent *comp;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    comp = CONTING_COMPONENT(self);

    comp->p0.x = comp->p0.y = comp->p1.x = comp->p1.y = 0;
    comp->placed = FALSE;
}

static void
conting_component_class_init(gpointer g_class, gpointer class_data)
{
    ContingDrawingClass *drawing_class;
    ContingComponentClass *component_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->get_bounds = conting_component_get_bounds;
    drawing_class->place = conting_component_place;
    drawing_class->is_placed = conting_component_is_placed;
    drawing_class->answer = conting_component_answer;

    drawing_class->place_xml = conting_component_place_xml;

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
            conting_component_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_DRAWING,
                "ContingComponent",
                &type_info, 0);
    }

    return type;
}
