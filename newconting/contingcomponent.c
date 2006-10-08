#include "contingdrawing.h"
#include "contingutil.h"
#include "contingxml.h"
#include "contingserializable.h"
#include <assert.h>

static gpointer parent_class = NULL;
static gpointer parent_iface = NULL;

#define CONTING_COMPONENT_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_COMPONENT, ContingComponentPrivate))
typedef struct ContingComponentPrivate_ ContingComponentPrivate;
struct ContingComponentPrivate_ {
    gboolean grabbed;
    ArtPoint grabbed_pw;

    gdouble rotate[6];
};


#define TOLERANCE CONTING_DRAWING_TOLERANCE
#define SIZE ((TOLERANCE * 2) - 1)

void
conting_component_rotate(ContingComponent *self)
{
    ContingComponentPrivate *priv;
    gdouble rotate[6];

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    priv = CONTING_COMPONENT_GET_PRIVATE(self);
    
    
    art_affine_rotate(rotate, 90.0);
    art_affine_multiply(priv->rotate, priv->rotate, rotate);

    if (self->placed)
        g_signal_emit_by_name(self, "move");


    /*
    conting_drawing_update(self);

    for (n = comp->links; n != NULL; n = g_list_next(n))
        conting_drawing_update(CONTING_DRAWING(n->data));
        */
}

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
conting_component_draw_selection(ContingDrawing *self,
        ContingDrawingBoxDrawer drawer)
{
    ContingComponent *comp;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    comp = CONTING_COMPONENT(self);

    drawer(self, comp->p0.x, comp->p0.y);
    drawer(self, comp->p0.x, comp->p1.y);
    drawer(self, comp->p1.x, comp->p0.y);
    drawer(self, comp->p1.x, comp->p1.y);
}

static void
conting_component_draw(ContingDrawing *self, cairo_t *cr)
{
    ContingComponent *comp;
    ArtPoint pw0, pw1;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    comp = CONTING_COMPONENT(self);
    
    pw0 = comp->p0;
    pw1 = comp->p1;
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

    g_print("g_list_append()\n");
    comp->links = g_list_append(comp->links, link);
    g_hash_table_insert(comp->points, link, new_point);

    g_signal_connect(G_OBJECT(link), "delete",
            G_CALLBACK(conting_component_link_deleted_stub), comp);

    conting_util_bounds_add_point(&comp->min_bounds, p);
}
static void
conting_component_get_update_bounds(ContingDrawing *self,
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


    if (conting_drawing_is_selected(self)) {
        bounds->x0 -= TOLERANCE;
        bounds->y0 -= TOLERANCE;
        bounds->x1 += TOLERANCE;
        bounds->y1 += TOLERANCE;
    }

    if (comp->show) {
        GList *n;
        ArtPoint p;

        for (n = comp->links; n != NULL; n = g_list_next(n)) {
            conting_drawing_get_center(CONTING_DRAWING(n->data), &p, &p);
            conting_util_bounds_add_point(bounds, &p);
        }

        conting_util_expand_bounds(bounds, 9);
    }
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
}
static void
conting_component_place(ContingDrawing *self)
{
    ContingComponent *comp;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    comp = CONTING_COMPONENT(self);

    assert(!comp->placed);
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
    ContingComponentPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

       priv = CONTING_COMPONENT_GET_PRIVATE(self);

    CONTING_DRAWING_CLASS(parent_class)->get_i2w_affine(self, affine);

    art_affine_multiply(affine, priv->rotate, affine);
}
static void
conting_component_get_w2i_affine(ContingDrawing *self,
                                 gdouble affine[6])
{
    ContingComponentPrivate *priv;
    gdouble invert_drawing[6];
    gdouble invert_rotate[6];

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

    CONTING_DRAWING_CLASS(parent_class)->get_w2i_affine(self, invert_drawing);

    art_affine_invert(invert_rotate, priv->rotate);

    art_affine_multiply(affine, invert_drawing, invert_rotate);
}

#define IS_NEAR(p0, p1) ((p0) < (p1) + TOLERANCE && (p0) > (p1) - TOLERANCE)

#define conting_component_is_resizing(comp) \
    (CONTING_COMPONENT(comp)->h_resizing \
     || CONTING_COMPONENT(comp)->v_resizing)

#include <gdk/gdkkeysyms.h>
static gboolean
conting_component_event(ContingDrawing *self,
                        GdkEvent *event)
{
    ContingComponent *comp;
    ArtPoint p;

    g_return_val_if_fail(self != NULL && CONTING_IS_COMPONENT(self), FALSE);

    if (CONTING_DRAWING_CLASS(parent_class)->event(self, event))
        return TRUE;

    comp = CONTING_COMPONENT(self);

    conting_one_line_window_to_world(conting_drawing_get_one_line(self),
            event->button.x, event->button.y,
            &p.x, &p.y);

    switch (event->type) {
        /*
        case GDK_BUTTON_PRESS:
            {
                ArtPoint pi;

                conting_drawing_grab(self);
                comp->dragging_point = p;
                comp->dragging = TRUE;

                conting_drawing_w2i(self, &pi, &p);

                comp->resizing = 0;

                if (comp->resize_horizontal) {
                    if (IS_NEAR(pi.x, comp->p0.x)) {
                        comp->resizing |= CONTING_RESIZE_LEFT;
                    } else if (IS_NEAR(pi.x, comp->p1.x)) {
                        comp->resizing |= CONTING_RESIZE_RIGHT;
                    }
                }

                if (comp->resize_vertical) {
                    if (IS_NEAR(pi.y, comp->p0.y)) {
                        comp->resizing |= CONTING_RESIZE_UP;
                    } else if (IS_NEAR(pi.y, comp->p1.y)) {
                        comp->resizing |= CONTING_RESIZE_DOWN;
                    }
                }

                return TRUE;
            }
            break;
            */
        case GDK_MOTION_NOTIFY:
            if (!comp->placed) {
                gdouble affine[6];

                art_affine_translate(affine, p.x, p.y);
                conting_drawing_affine_absolute(self, affine);

                return TRUE;
            } else if (comp->placed && comp->dragging
                    && comp->resizing) {
                /*
                ArtPoint p0, p1;
                gdouble w, h;
                gdouble affine[6];
                */
                ArtPoint pi;

                conting_drawing_w2i(self, &pi, &p);

                if (comp->resizing & CONTING_RESIZE_LEFT) {
                    comp->p0.x = MIN(comp->min_bounds.x0, pi.x);
                } else if (comp->resizing & CONTING_RESIZE_RIGHT) {
                    comp->p1.x = MAX(comp->min_bounds.x1, pi.x);
                }

                if (comp->resizing & CONTING_RESIZE_UP) {
                    comp->p0.y = MIN(comp->min_bounds.y0, pi.y);
                } else if (comp->resizing & CONTING_RESIZE_DOWN) {
                    comp->p1.y = MAX(comp->min_bounds.y1, pi.y);
                }

/*
                w = fabs(comp->p1.x - comp->p0.x);
                h = fabs(comp->p1.y - comp->p0.y);

                p0.x = -w / 2;    p1.x = p0.x + w;
                p0.y = -h / 2;    p1.y = p0.y + h;

                art_affine_translate(affine,
                        comp->p0.x - p0.x,
                        comp->p0.y - p0.y);
                conting_drawing_affine(self, affine);

                comp->p0 = p0;
                comp->p1 = p1;
                */

                g_signal_emit_by_name(self, "move");
                
            } else if (comp->placed && comp->dragging) {
                gdouble affine[6];
                art_affine_translate(affine,
                        p.x - comp->dragging_point.x,
                        p.y - comp->dragging_point.y);
                conting_drawing_affine(self, affine);
                g_signal_emit_by_name(self, "move");
                comp->dragging_point = p;

                return TRUE;
            } else if (comp->resize_horizontal) {
                ArtPoint pi;
                assert(comp->placed && !comp->dragging);

                conting_drawing_w2i(self, &pi, &p);
                
                if (IS_NEAR(pi.x, comp->p0.x)
                       || IS_NEAR(pi.x, comp->p1.x)) {
                    conting_one_line_cursor(conting_drawing_get_one_line(self),
                            GDK_PLUS);
                } else {
                    conting_one_line_cursor(conting_drawing_get_one_line(self),
                            GDK_LEFT_PTR);
                }
            } else if (comp->resize_vertical) {
                ArtPoint pi;
                assert(comp->placed && !comp->dragging);

                conting_drawing_w2i(self, &pi, &p);

                if (IS_NEAR(pi.y, comp->p0.y)
                        || IS_NEAR(pi.y, comp->p1.y)) {
                    conting_one_line_cursor(conting_drawing_get_one_line(self),
                            GDK_PLUS);
                } else {
                    conting_one_line_cursor(conting_drawing_get_one_line(self),
                            GDK_LEFT_PTR);
                }
            }
            break;
        case GDK_2BUTTON_PRESS:
        case GDK_BUTTON_RELEASE:
            if (comp->dragging) {
                comp->dragging = FALSE;
                conting_drawing_ungrab(self);
            }
            return TRUE;
            break;
        case GDK_KEY_PRESS:
            if (event->key.keyval == GDK_r) {
                conting_component_rotate(comp);
                return TRUE;
            }

            if (event->key.keyval == GDK_s) {
                comp->show = TRUE;

                return TRUE;
            }

            comp->show = FALSE;
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
conting_component_link_deleted_impl(ContingComponent *comp,
                                    ContingDrawing *link)
{
    g_return_if_fail(comp != NULL && CONTING_IS_COMPONENT(comp));
    g_return_if_fail(link != NULL && CONTING_IS_DRAWING(link));

    conting_component_disconnect_link(comp, link);
}

static void
conting_component_link_deleted(ContingComponent *comp,
                               ContingDrawing *link)
{
    g_return_if_fail(comp != NULL && CONTING_IS_COMPONENT(comp));
    g_return_if_fail(link != NULL && CONTING_IS_DRAWING(link));

    CONTING_COMPONENT_GET_CLASS(comp)->link_deleted(comp, link);
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

    if (comp->dragging) {
        comp->dragging = FALSE;
        conting_drawing_ungrab(self);
    }

    if (CONTING_DRAWING_CLASS(parent_class)->delete)
        CONTING_DRAWING_CLASS(parent_class)->delete(self);
}

/**
 * TODO: CHANGE IT! IT's CRAPPY!
 */
static xmlNodePtr
conting_bus_point_node(gconstpointer data, gpointer user_data)
{
    xmlNodePtr node;
    char buff[256];
    const ArtPoint *p = data;

    node = xmlNewNode(NULL, BAD_CAST "point");

    sprintf(buff, "%lf %lf", p->x, p->y);
    xmlAddChild(node, xmlNewText(BAD_CAST buff));


    return node;
}
static xmlNodePtr
conting_bus_drawing_node(gconstpointer data, gpointer user_data)
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
static void
conting_component_write(ContingSerializable *self,
                           xmlNodePtr drawing_node,
                           xmlNodePtr *result)
{
    ContingComponent *comp;
    ContingComponentPrivate *priv;
    xmlNodePtr class_node;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    comp = CONTING_COMPONENT(self);
    priv = CONTING_COMPONENT_GET_PRIVATE(self);

    class_node = xmlNewNode(NULL, BAD_CAST "class");
    xmlNewProp(class_node, BAD_CAST "name",
            BAD_CAST g_type_name(CONTING_TYPE_COMPONENT));

    xmlAddChild(class_node, conting_util_point_node("p0", &comp->p0));
    xmlAddChild(class_node, conting_util_point_node("p1", &comp->p1));
    xmlAddChild(class_node, conting_util_affine_node("rotate", priv->rotate));

    xmlAddChild(class_node, conting_util_hash_node("points", comp->points,
                conting_bus_drawing_node, conting_bus_point_node, NULL));

    xmlAddChild(drawing_node, class_node);

    ((ContingSerializableClass *) parent_iface)->write(self, drawing_node,
            NULL);
}

/*
 * TODO: CHANGE IT
 */
static gpointer
conting_bus_load_drawing(xmlNodePtr node, gpointer user_data)
{
    GHashTable *id_drawing = user_data;
    xmlChar *id;
    gpointer result;

    g_print("%s\n", node->name);
    assert(xmlStrEqual(node->name, "drawing"));

    id = xmlGetProp(node, BAD_CAST "id");

    result = g_hash_table_lookup(id_drawing,
            GUINT_TO_POINTER(strtoul(id, NULL, 10)));
    assert(result && CONTING_IS_DRAWING(result));

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
conting_component_conn_foreach(gpointer key, gpointer value,
        gpointer user_data)
{
    ContingComponent *comp;

    g_return_if_fail(key != NULL && CONTING_IS_DRAWING(key));

    comp = CONTING_COMPONENT(user_data);

    comp->links = g_list_append(comp->links, key);

    g_signal_connect(G_OBJECT(key), "delete",
            G_CALLBACK(conting_component_link_deleted_stub), user_data);
}

static void
conting_component_read(ContingSerializable *self, xmlNodePtr drawing_node,
                            GHashTable *id_drawing)
{
    ContingComponent *comp;
    ContingComponentPrivate *priv;
    xmlNodePtr class_node;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    assert(xmlStrEqual(drawing_node->name, "drawing"));
    /* Load p0, p1 */
    comp = CONTING_COMPONENT(self);
    priv = CONTING_COMPONENT_GET_PRIVATE(self);

    for (class_node = drawing_node->children; class_node != NULL;
            class_node = class_node->next) {
        xmlChar *class_name;

        if (!xmlStrEqual(class_node->name, BAD_CAST "class"))
            continue;

        class_name = xmlGetProp(class_node, BAD_CAST "name");

        if (class_name && xmlStrEqual(class_name,
                    g_type_name(CONTING_TYPE_COMPONENT))) {
            xmlNodePtr attr;

            for (attr = class_node->children; attr; attr = attr->next) {
                xmlChar *name, *type;

                if (!xmlStrEqual(attr->name, BAD_CAST "attribute"))
                    continue;

                name = xmlGetProp(attr, BAD_CAST "name");
                type = xmlGetProp(attr, BAD_CAST "type");

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
                    g_print("loading links\n");
                    conting_util_load_hash(attr, comp->points,
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

    g_print("Component loaded!\n");

    comp->placed = TRUE;
    g_hash_table_foreach(comp->points, conting_component_conn_foreach, self);

    ((ContingSerializableClass *) parent_iface)->read(self,
            drawing_node, id_drawing);
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

static gboolean
conting_component_find_link_pred(ContingDrawing *self, gpointer user_data)
{
    gpointer *params = user_data;
    ContingDrawingPredicate pred = params[2];

    if (params[1] == self)
        return FALSE;

    return pred(self, params[0]);
}

static void
conting_component_find_link(ContingDrawing *self, ContingDrawingPredicate pred,
        gpointer user_data) {
    ContingComponent *comp;
    gpointer params[3];
    GList *n;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    comp = CONTING_COMPONENT(self);

    if (!pred(self, user_data))
        return;

    params[0] = user_data;
    params[1] = self;
    params[2] = pred;

    for (n = comp->links; n != NULL; n = g_list_next(n)) {
        ContingDrawing *drawing = n->data;
        if (pred(drawing, user_data)) {
            conting_drawing_find_link(drawing,
                    conting_component_find_link_pred, params);
        }
    }
}

static void
conting_component_grab(ContingDrawing *self, ArtPoint *pi)
{
    ContingComponentPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

    priv->grabbed = TRUE;
    conting_drawing_i2w(self, &priv->grabbed_pw, pi);

    CONTING_DRAWING_CLASS(parent_class)->grab(self, pi);
}

static void
conting_component_ungrab(ContingDrawing *self)
{
    ContingComponentPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

    if (priv->grabbed == TRUE)
        CONTING_DRAWING_CLASS(parent_class)->ungrab(self);

    priv->grabbed = FALSE;
}

static void
conting_component_motion(ContingDrawing *self, ArtPoint *pi)
{
    ContingComponentPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    priv = CONTING_COMPONENT_GET_PRIVATE(self);

    if (priv->grabbed) {
        /* TODO: Should I transform pi and grabbed_pw to world coords? */
        gdouble affine[6];
        ArtPoint pw;

        conting_drawing_i2w(self, &pw, pi);
        
        art_affine_translate(affine,
                pw.x - priv->grabbed_pw.x,
                pw.y - priv->grabbed_pw.y);

        conting_drawing_affine(self, affine);
        g_signal_emit_by_name(self, "move");
        priv->grabbed_pw = pw;
    }
}

static void
conting_component_motion_place(ContingDrawing *self, ArtPoint *pi)
{
    ArtPoint pw;
    gdouble affine[6];

    conting_drawing_i2w(self, &pw, pi);

    art_affine_translate(affine, pw.x, pw.y);
    conting_drawing_affine_absolute(self, affine);
}

static void
conting_component_instance_init(GTypeInstance *self, gpointer g_class)
{
    ContingComponent *comp;
    ContingComponentPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_COMPONENT(self));

    comp = CONTING_COMPONENT(self);
    priv = CONTING_COMPONENT_GET_PRIVATE(self);

    comp->p0.x = comp->p0.y = comp->p1.x = comp->p1.y = 0;
    comp->placed = FALSE;

    art_affine_rotate(priv->rotate, 0.0);

    comp->points = g_hash_table_new_full(NULL, NULL, NULL, g_free);
    comp->links = NULL;

    comp->dragging = FALSE;

    comp->resize_horizontal = comp->resize_vertical = FALSE;
    comp->resizing = 0;

    priv->grabbed = FALSE;
}

static void
conting_component_serializable_init(gpointer g_iface, gpointer iface_data)
{
    ContingSerializableClass *serial_class;

    serial_class = g_iface;
    serial_class->read = conting_component_read;
    serial_class->write = conting_component_write;

    parent_iface = g_type_interface_peek_parent(g_iface);
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
    drawing_class->draw = conting_component_draw;
    drawing_class->draw_selection = conting_component_draw_selection;
    drawing_class->get_bounds = conting_component_get_bounds;
    drawing_class->get_update_bounds = conting_component_get_update_bounds;
    drawing_class->place = conting_component_place;
    drawing_class->is_placed = conting_component_is_placed;
    drawing_class->answer = conting_component_answer;
    drawing_class->get_i2w_affine = conting_component_get_i2w_affine;
    drawing_class->get_w2i_affine = conting_component_get_w2i_affine;
    drawing_class->event = conting_component_event;
    drawing_class->delete = conting_component_delete;

    drawing_class->find_link = conting_component_find_link;

    drawing_class->grab = conting_component_grab;
    drawing_class->ungrab = conting_component_ungrab;
    drawing_class->motion = conting_component_motion;
    drawing_class->motion_place = conting_component_motion_place;


    component_class = CONTING_COMPONENT_CLASS(g_class);
    component_class->link = NULL;
    component_class->get_link_point = conting_component_get_link_point_impl;
    component_class->link_deleted = conting_component_link_deleted_impl;

    parent_class = g_type_class_peek_parent(g_class);

    g_type_class_add_private(g_class, sizeof(ContingComponentPrivate));
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

        static const GInterfaceInfo serial_info = {
            conting_component_serializable_init,
            NULL,
            NULL    
        };

        g_type_add_interface_static(type,
                CONTING_TYPE_SERIALIZABLE,
                &serial_info);

    }

    return type;
}
