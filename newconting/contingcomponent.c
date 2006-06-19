#include "contingcomponent.h"

static gpointer parent_class = NULL;


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
conting_component_link_deleted(ContingComponent *comp,
		                       ContingDrawing *link);
static void
conting_component_link_deleted_stub(ContingDrawing *drawing,
		                            gpointer user_data)
{
	conting_component_link_deleted(CONTING_COMPONENT(user_data), drawing);
}

void
conting_component_connect_link(ContingComponent *comp,
		                   ContingDrawing *link, ArtPoint *p)
{
	ArtPoint *new_point;

	g_return_if_fail(comp != NULL && CONTING_IS_COMPONENT(comp));

	new_point = g_new(ArtPoint, 1);
	*new_point = *p;

	comp->links = g_list_append(comp->links, link);
	g_hash_table_insert(comp->points, link, new_point);

	g_signal_connect(G_OBJECT(link), "delete",
			G_CALLBACK(conting_component_link_deleted_stub), comp);
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
conting_component_get_i2w_affine(ContingDrawing *self,
                                 gdouble affine[6])
{
    ContingComponent *comp;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    comp = CONTING_COMPONENT(self);

    CONTING_DRAWING_CLASS(parent_class)->get_i2w_affine(self, affine);

    art_affine_multiply(affine, comp->rotate, affine);
}
static void
conting_component_get_w2i_affine(ContingDrawing *self,
                                 gdouble affine[6])
{
    ContingComponent *comp;
    gdouble invert_drawing[6];
    gdouble invert_rotate[6];

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    comp = CONTING_COMPONENT(self);

    CONTING_DRAWING_CLASS(parent_class)->get_w2i_affine(self, invert_drawing);

    art_affine_invert(invert_rotate, comp->rotate);

    art_affine_multiply(affine, invert_drawing, invert_rotate);
}

#include <gdk/gdkkeysyms.h>
static gboolean
conting_component_event(ContingDrawing *self,
                        GdkEvent *event)
{
    ContingComponent *comp;

    g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

    comp = CONTING_COMPONENT(self);

    switch (event->type) {
        case GDK_KEY_PRESS:
            if (event->key.keyval == GDK_r) {
                gdouble rotate[6];
                art_affine_rotate(rotate, 90.0);
                art_affine_multiply(comp->rotate, comp->rotate, rotate);

                if (!comp->placed)
                    g_signal_emit_by_name(self, "move");

                conting_drawing_update(self);

                return TRUE;
            }
            break;
        default:
            break;
    }

    return FALSE;
}

static gboolean
conting_component_get_link_point_impl(ContingComponent *self,
                                      ContingDrawing *line,
                                      ArtPoint *p)
{
	ContingComponent *comp;
	ArtPoint *point;

	g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

	comp = CONTING_COMPONENT(self);

	point = g_hash_table_lookup(comp->points, line);

	if (point == NULL)
		return FALSE;

	conting_drawing_i2w(CONTING_DRAWING(self), p, point);

	return TRUE;
}

void
conting_component_disconnect_link(ContingComponent *comp,
		                          ContingDrawing *drawing)
{
	g_return_if_fail(comp != NULL && CONTING_IS_COMPONENT(comp));
	g_return_if_fail(drawing != NULL && CONTING_IS_DRAWING(drawing));

	g_signal_handlers_disconnect_matched(drawing,
			G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, comp);
    g_hash_table_remove(comp->points, drawing);
	comp->links = g_list_remove(comp->links, drawing);
}

static void
conting_component_link_deleted(ContingComponent *comp,
		                       ContingDrawing *link)
{
	g_return_if_fail(comp != NULL && CONTING_IS_COMPONENT(comp));
	g_return_if_fail(link != NULL && CONTING_IS_DRAWING(link));

	conting_component_disconnect_link(comp, link);
}

static void
conting_component_delete(ContingDrawing *self)
{
	ContingComponent *comp;
	GList *n, *next;

	g_print("DELETING\n");

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

	comp = CONTING_COMPONENT(self);

	for (n = comp->links; n != NULL; n = next) {
		next = g_list_next(n);
		conting_component_disconnect_link(comp, CONTING_DRAWING(n->data));
	}

	g_signal_emit_by_name(self, "delete");

	if (CONTING_DRAWING_CLASS(parent_class)->delete)
		CONTING_DRAWING_CLASS(parent_class)->delete(self);
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
conting_component_finalize(GObject *self)
{
	ContingComponent *comp;

	g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

	comp = CONTING_COMPONENT(self);

	g_hash_table_destroy(comp->points);
	g_list_free(comp->links);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void
conting_component_instance_init(GTypeInstance *self, gpointer g_class)
{
    ContingComponent *comp;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    comp = CONTING_COMPONENT(self);

    comp->p0.x = comp->p0.y = comp->p1.x = comp->p1.y = 0;
    comp->placed = FALSE;

    art_affine_rotate(comp->rotate, 0.0);

	comp->points = g_hash_table_new_full(NULL, NULL, NULL, g_free);
	comp->links = NULL;
}

static void
conting_component_class_init(gpointer g_class, gpointer class_data)
{
	GObjectClass *object_class;
    ContingDrawingClass *drawing_class;
    ContingComponentClass *component_class;

	object_class = G_OBJECT_CLASS(g_class);
	object_class->finalize = conting_component_finalize;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->get_bounds = conting_component_get_bounds;
    drawing_class->place = conting_component_place;
    drawing_class->is_placed = conting_component_is_placed;
    drawing_class->answer = conting_component_answer;
    drawing_class->get_i2w_affine = conting_component_get_i2w_affine;
    drawing_class->get_w2i_affine = conting_component_get_w2i_affine;
    drawing_class->event = conting_component_event;
    drawing_class->delete = conting_component_delete;

    drawing_class->place_xml = conting_component_place_xml;

    component_class = CONTING_COMPONENT_CLASS(g_class);
    component_class->link = NULL;
    component_class->get_link_point = conting_component_get_link_point_impl;
    component_class->link_deleted = conting_component_link_deleted;

    parent_class = g_type_class_peek_parent(g_class);
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
