#include "contingline.h"
#include "contingcomponent.h"
#include "contingutil.h"
#include "contingxml.h"
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

static gpointer parent_class = NULL;

#define CONTING_LINE_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_LINE, ContingLinePrivate))

#define TOLERANCE 3
#define SIZE ((TOLERANCE * 2) - 1)

typedef struct ContingLinePrivate_ ContingLinePrivate;
struct ContingLinePrivate_ {
	GList *points;
	gint n_points;

	ContingComponent *comp0, *comp1;

	ArtPoint *link0, *link1;

    gboolean placing;
    gboolean placed;

	ArtPoint *dragging_point;
	GList *last_answer;
	ArtPoint placing_point;
};

static void
conting_line_draw(ContingDrawing *self,
                  GdkDrawable *drawable,
                  const GdkRectangle *drawing_rect)
{
    ContingLinePrivate *priv;
	gdouble affine[6];
	GList *n;
	ArtPoint pw0, pw1;
	gdouble width;
	cairo_t *cr;
/*
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
	*/
	g_object_get(conting_drawing_get_one_line(self),
			"ppu", &width,
			NULL);
	/*
	gdk_gc_set_line_attributes(gc, (gint) width, GDK_LINE_SOLID,
			GDK_CAP_NOT_LAST,
			GDK_JOIN_MITER);
			*/

    g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

    priv = CONTING_LINE_GET_PRIVATE(self);

	conting_drawing_get_i2w_affine(self, affine);

	if (!priv->placing && !priv->placed)
		return;
	cr = gdk_cairo_create(drawable);
	cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
	cairo_set_line_width(cr, width);

	pw0 = *((ArtPoint *) priv->points->data);
	art_affine_point(&pw0, &pw0, affine);
	conting_one_line_world_to_window(conting_drawing_get_one_line(self),
			pw0.x, pw0.y, &pw0.x, &pw0.y);
	if (conting_drawing_is_selected(self)) {
		cairo_move_to(cr, pw0.x - TOLERANCE, pw0.y - TOLERANCE);
		cairo_line_to(cr, pw0.x - TOLERANCE + SIZE, pw0.y - TOLERANCE);
		cairo_line_to(cr, pw0.x - TOLERANCE + SIZE, pw0.y - TOLERANCE + SIZE);
		cairo_line_to(cr, pw0.x - TOLERANCE, pw0.y - TOLERANCE + SIZE);
		cairo_line_to(cr, pw0.x - TOLERANCE, pw0.y - TOLERANCE);
		cairo_set_source_rgb(cr, 0, 0, 0);
		cairo_fill(cr);
		cairo_stroke(cr);
		/*
		gdk_draw_rectangle(drawable, gc, TRUE,
				pw0.x - TOLERANCE, pw0.y - TOLERANCE, SIZE, SIZE);
				*/
	}

	for (n = g_list_next(priv->points); n != NULL; n = g_list_next(n)) {
		pw1 = *((ArtPoint *) n->data);
		art_affine_point(&pw1, &pw1, affine);
		conting_one_line_world_to_window(conting_drawing_get_one_line(self),
				pw1.x, pw1.y, &pw1.x, &pw1.y);

		g_print("drawing: (%lf, %lf); (%lf, %lf)\n",
				pw0.x, pw0.y, pw1.x, pw1.y);
		cairo_move_to(cr, pw0.x, pw0.y);
		cairo_line_to(cr, pw1.x, pw1.y);
		cairo_stroke(cr);
		/*
		gdk_draw_line(drawable, gc,
				(gint) pw0.x, (gint) pw0.y,
				(gint) pw1.x, (gint) pw1.y);
				*/
		if (conting_drawing_is_selected(self)) {
			cairo_move_to(cr, pw1.x - TOLERANCE, pw1.y - TOLERANCE);
			cairo_line_to(cr, pw1.x - TOLERANCE + SIZE, pw1.y - TOLERANCE);
			cairo_line_to(cr, pw1.x - TOLERANCE + SIZE,
					pw1.y - TOLERANCE + SIZE);
			cairo_line_to(cr, pw1.x - TOLERANCE, pw1.y - TOLERANCE + SIZE);
			cairo_line_to(cr, pw1.x - TOLERANCE, pw1.y - TOLERANCE);
			cairo_set_source_rgb(cr, 0, 0, 0);
			cairo_fill(cr);
			cairo_stroke(cr);
			/*
			gdk_draw_rectangle(drawable, gc, TRUE,
					pw1.x - TOLERANCE, pw1.y - TOLERANCE, SIZE, SIZE);
					*/
		}


		pw0 = pw1;
	}

	if (priv->placing) {
		pw1 = priv->placing_point;
		art_affine_point(&pw1, &pw1, affine);
		conting_one_line_world_to_window(conting_drawing_get_one_line(self),
				pw1.x, pw1.y, &pw1.x, &pw1.y);

		cairo_move_to(cr, pw0.x, pw0.y);
		cairo_line_to(cr, pw1.x, pw1.y);
		cairo_stroke(cr);
		/*
		gdk_draw_line(drawable, gc,
				(gint) pw0.x, (gint) pw0.y,
				(gint) pw1.x, (gint) pw1.y);
				*/
	}

	cairo_destroy(cr);
}

static void
conting_line_get_bounds(ContingDrawing *self,
		                ArtDRect *bounds)
{
	ContingLinePrivate *priv;
	gdouble affine[6];
	ArtPoint pw;
	GList *n;

	g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

    priv = CONTING_LINE_GET_PRIVATE(self);

	if (!priv->placing && !priv->placed) {
		bounds->x0 = bounds->y0 = bounds->x1 = bounds->y1 = 0;
		return;
	}

	conting_drawing_get_i2w_affine(self, affine);

	pw = *((ArtPoint *) priv->points->data);
	art_affine_point(&pw, &pw, affine);
	bounds->x0 = bounds->x1 = pw.x;
	bounds->y0 = bounds->y1 = pw.y;

	for (n = g_list_next(priv->points); n != NULL; n = g_list_next(n)) {
		art_affine_point(&pw, (ArtPoint *) n->data, affine);

		if (pw.x < bounds->x0) {
			bounds->x0 = pw.x;
		} else if (pw.x > bounds->x1) {
			bounds->x1 = pw.x;
		}

		if (pw.y < bounds->y0) {
			bounds->y0 = pw.y;
		} else if (pw.y > bounds->y1) {
			bounds->y1 = pw.y;
		}
	}
	art_affine_point(&pw, &priv->placing_point, affine);

	if (pw.x < bounds->x0) {
		bounds->x0 = pw.x;
	} else if (pw.x > bounds->x1) {
		bounds->x1 = pw.x;
	}

	if (pw.y < bounds->y0) {
		bounds->y0 = pw.y;
	} else if (pw.y > bounds->y1) {
		bounds->y1 = pw.y;
	}

	/* selection anchors */
	bounds->x0 -= TOLERANCE;
	bounds->y0 -= TOLERANCE;
	bounds->x1 += TOLERANCE;
	bounds->y1 += TOLERANCE;
}

static void
conting_line_finalize(GObject *self)
{
	ContingLinePrivate *priv;
	GList *n;

	g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

	priv = CONTING_LINE_GET_PRIVATE(self);

	for (n = priv->points; n != NULL; n = g_list_next(n)) {
		g_free(n->data);
	}

	g_list_free(priv->points);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void
conting_line_get_center(ContingDrawing *self,
		                ArtPoint *pw_dst, const ArtPoint *pw_src)
{
	ContingLinePrivate *priv;
	gdouble width;
	GList *n, *next;

	g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

	priv = CONTING_LINE_GET_PRIVATE(self);

	width = 0;

	n = priv->points;
	for (next = g_list_next(n); n && next; next = g_list_next(next)) {
		ArtPoint p0, p1;
		gdouble w, h;

		conting_drawing_i2w(self, &p0, n->data);
		conting_drawing_i2w(self, &p1, next->data);

		w = fabs(p0.x - p1.x);
		h = fabs(p0.y - p1.y);

		width += sqrt(w * w + h * h);

		n = next;
	}

	width /= 2;

	n = priv->points;
	for (next = g_list_next(n); n && next; next = g_list_next(next)) {
		ArtPoint p0, p1;
		gdouble w, h, seg_w;

		conting_drawing_i2w(self, &p0, n->data);
		conting_drawing_i2w(self, &p1, next->data);

		w = fabs(p0.x - p1.x);
		h = fabs(p0.y - p1.y);

		seg_w = sqrt(w * w + h * h);
		width -= seg_w;

		if (width <= 0) {
			width = seg_w + width;

			/*
			 * Could multiply by (width / seg_w) to get more precise
			 * centering. Or multiply by 0.5 to always get the medium
			 * of a line segment.
			 */
			pw_dst->x = p0.x + (p1.x - p0.x) * (width / seg_w);
			pw_dst->y = p0.y + (p1.y - p0.y) * (width / seg_w);
			return;
		}

		n = next;
		

		n = next;
	}
	
}

static void
conting_line_instance_init(GTypeInstance *self,
		                   gpointer g_class)
{
	ContingLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

    priv = CONTING_LINE_GET_PRIVATE(self);

	priv->points = NULL;

	priv->placing = FALSE;
	priv->placed = FALSE;

	priv->comp0 = NULL;
	priv->comp1 = NULL;
	priv->link0 = NULL;
	priv->link1 = NULL;

	priv->dragging_point = NULL;
}


static void
conting_line_link_moved(ContingComponent *comp,
						 gpointer user_data)
{
	ContingLinePrivate *priv;
	ContingGroup *group;
	gdouble affine[6];
	ArtPoint pi;
	ArtDRect bounds;

	g_return_if_fail(user_data != NULL && CONTING_IS_LINE(user_data));
	g_return_if_fail(comp != NULL && CONTING_IS_COMPONENT(comp));

	priv = CONTING_LINE_GET_PRIVATE(user_data);

	if ((group = conting_drawing_get_group(CONTING_DRAWING(user_data))) != NULL
			&& conting_group_contains(group, CONTING_DRAWING(comp))) {
		return;
	}

	assert(conting_component_get_link_point(comp,
				CONTING_DRAWING(user_data), &pi));

	conting_drawing_get_i2w_affine(CONTING_DRAWING(user_data), affine);
	art_affine_invert(affine, affine);
	art_affine_point(&pi, &pi, affine);

	conting_drawing_get_bounds(CONTING_DRAWING(user_data), &bounds);

	if (priv->comp0 == comp) {
		*(priv->link0) = pi;
	} else if (priv->comp1 == comp) {
		*(priv->link1) = pi;
	} else {
		assert(FALSE);
	}

	conting_one_line_update(
			conting_drawing_get_one_line(CONTING_DRAWING(user_data)),
			&bounds);
	conting_drawing_update(CONTING_DRAWING(user_data));
}
static void
conting_line_link_deleted(ContingComponent *link,
		                  gpointer user_data)
{
	g_return_if_fail(user_data != NULL && CONTING_IS_LINE(user_data));
	conting_drawing_delete(CONTING_DRAWING(user_data));
}
static ArtPoint *
conting_line_add_point(ContingLine *self, gdouble ix, gdouble iy)
{
	ContingLinePrivate *priv;
	ArtPoint *p;

	g_return_val_if_fail(self != NULL && CONTING_IS_LINE(self), NULL);

	priv = CONTING_LINE_GET_PRIVATE(self);

	p = g_new(ArtPoint, 1);

	p->x = ix;
	p->y = iy;

	priv->points = g_list_append(priv->points, p);

	priv->n_points++;
	
	return p;
}
static void
conting_line_place(ContingDrawing *self)
{
	ContingLinePrivate *priv;
	gdouble affine[6], tmp[6];
	ArtPoint p, pcomp;
    GSList *n, *answers;
	ContingComponent *comp;

	g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

    priv = CONTING_LINE_GET_PRIVATE(self);

	conting_drawing_get_i2w_affine(self, affine);

	p = priv->placing_point;

	answers = conting_one_line_answer(conting_drawing_get_one_line(self),
			p.x, p.y);

	comp = NULL;
	for (n = answers; n != NULL; n = g_slist_next(n)) {
		g_print("CONTING_IS_COMPONENT(%p) ? %s\n", n->data,
				CONTING_IS_COMPONENT(n->data) ? "YES" : "NO");
		if (CONTING_IS_COMPONENT(n->data)
				&& conting_component_link(CONTING_COMPONENT(n->data), self,
					p.x, p.y, &pcomp)) {
			comp = CONTING_COMPONENT(n->data);
			break;
		}
	}

	art_affine_invert(tmp, affine);
	art_affine_point(&pcomp, &pcomp, tmp);
	art_affine_translate(tmp, pcomp.x - p.x, pcomp.y - p.y);

	if (!priv->placing) {
		if (comp != NULL) {
			art_affine_point(&p, &p, tmp);

			priv->placing = TRUE;

			priv->link0 = conting_line_add_point(CONTING_LINE(self),
					p.x, p.y);
			priv->comp0 = comp;

			g_signal_connect(G_OBJECT(priv->comp0), "move",
					G_CALLBACK(conting_line_link_moved), self);
			g_signal_connect_swapped(G_OBJECT(self), "move",
					G_CALLBACK(conting_line_link_moved), priv->comp0);
			g_signal_connect(G_OBJECT(priv->comp0), "delete",
					G_CALLBACK(conting_line_link_deleted), self);
		}
	} else {
		assert(!priv->placed);
		if (comp == NULL && answers == NULL) {
			conting_line_add_point(CONTING_LINE(self), p.x, p.y);
		} else if (comp != NULL) {
			art_affine_point(&p, &p, tmp);

			priv->placing = FALSE;
			priv->placed = TRUE;

			priv->link1 = conting_line_add_point(CONTING_LINE(self), p.x, p.y);
			priv->comp1 = comp;
			g_signal_connect(G_OBJECT(priv->comp1), "move",
					G_CALLBACK(conting_line_link_moved), self);
			g_signal_connect_swapped(G_OBJECT(self), "move",
					G_CALLBACK(conting_line_link_moved), priv->comp1);
			g_signal_connect(G_OBJECT(priv->comp1), "delete",
					G_CALLBACK(conting_line_link_deleted), self);

			conting_drawing_set_selected(self, TRUE);

		}
	}

	g_slist_free(answers);
}

static gboolean
conting_line_is_placed(ContingDrawing *self)
{
	ContingLinePrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_LINE(self), FALSE);

    priv = CONTING_LINE_GET_PRIVATE(self);

	return priv->placed;
}
static void
conting_line_delete(ContingDrawing *self)
{
	ContingLinePrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

	priv = CONTING_LINE_GET_PRIVATE(self);

	if (priv->dragging_point != NULL) {
		priv->dragging_point = NULL;
		conting_drawing_ungrab(self);
	}

	g_signal_handlers_disconnect_matched(self, G_SIGNAL_MATCH_FUNC,
			0, 0, 0, conting_line_link_moved, 0);
	if (priv->comp0) {
		g_signal_handlers_disconnect_matched(priv->comp0, G_SIGNAL_MATCH_DATA,
				0, 0, 0, 0, self);
	}
	if (priv->comp1) {
		g_signal_handlers_disconnect_matched(priv->comp1, G_SIGNAL_MATCH_DATA,
				0, 0, 0, 0, self);
	}
	g_signal_emit_by_name(self, "delete");

	CONTING_DRAWING_CLASS(parent_class)->delete(self);
}
static xmlNodePtr
conting_line_point_node(gconstpointer data, gpointer user_data)
{
	xmlNodePtr node;
	char buff[256];
	const ArtPoint *p = data;

	node = xmlNewNode(NULL, BAD_CAST "point");
	
	sprintf(buff, "%lf %lf", p->x, p->y);
	xmlAddChild(node, xmlNewText(BAD_CAST buff));

	return node;
}
static gpointer
conting_line_load_point(xmlNodePtr point_node, gpointer user_data)
{
    xmlChar *point_text;
    ArtPoint *p = g_new(ArtPoint, 1);
    assert(xmlStrEqual(point_node->name, "point"));

    point_text = xmlNodeListGetString(point_node->doc, point_node->children,
            TRUE);

    sscanf(point_text, "%lf %lf", &p->x, &p->y);

    xmlFree(point_text);

    return p;
}

static void
conting_line_place_xml(ContingDrawing *self, xmlNodePtr drawing_node,
        GHashTable *id_drawing)
{
    ContingLinePrivate *priv;
    xmlNodePtr class_node;
    ArtPoint link0, link1;
    GList *n;

    g_return_if_fail(self != NULL && CONTING_IS_LINE(self));

    priv = CONTING_LINE_GET_PRIVATE(self);

    for (class_node = drawing_node->children; class_node;
            class_node = class_node->next) {
        xmlChar *class_name;

        if (!xmlStrEqual(class_node->name, BAD_CAST "class"))
            continue;

        class_name = xmlGetProp(class_node, BAD_CAST "name");

        if (class_name && xmlStrEqual(class_name, "ContingLine")) {
            xmlNodePtr attr;
            xmlFree(class_name);

            for (attr = class_node->children; attr; attr = attr->next) {
                xmlChar *name, *type;

                if (!xmlStrEqual(attr->name, BAD_CAST "attribute"))
                    continue;

                name = xmlGetProp(attr, BAD_CAST "name");
                type = xmlGetProp(attr, BAD_CAST "type");

                if (xmlStrEqual(type, BAD_CAST "drawing")
                        && xmlStrEqual(name, BAD_CAST "comp0")) {
                    priv->comp0 = CONTING_COMPONENT(
                            conting_util_load_drawing(attr, id_drawing));
			        g_signal_connect(G_OBJECT(priv->comp0), "move",
					        G_CALLBACK(conting_line_link_moved), self);
        			g_signal_connect_swapped(G_OBJECT(self), "move",
		        			G_CALLBACK(conting_line_link_moved), priv->comp0);
        			g_signal_connect(G_OBJECT(priv->comp0), "delete",
		        			G_CALLBACK(conting_line_link_deleted), self);
                } else if (xmlStrEqual(type, BAD_CAST "drawing")
                        && xmlStrEqual(name, BAD_CAST "comp1")) {
                    priv->comp1 = CONTING_COMPONENT(
                            conting_util_load_drawing(attr, id_drawing));
			        g_signal_connect(G_OBJECT(priv->comp1), "move",
					        G_CALLBACK(conting_line_link_moved), self);
        			g_signal_connect_swapped(G_OBJECT(self), "move",
		        			G_CALLBACK(conting_line_link_moved), priv->comp1);
        			g_signal_connect(G_OBJECT(priv->comp1), "delete",
		        			G_CALLBACK(conting_line_link_deleted), self);
                } else if (xmlStrEqual(type, BAD_CAST "point")
                        && xmlStrEqual(name, BAD_CAST "link0")) {
                    conting_util_load_point(attr, &link0);
                } else if (xmlStrEqual(type, BAD_CAST "point")
                        && xmlStrEqual(name, BAD_CAST "link1")) {
                    conting_util_load_point(attr, &link1);
                } else if (xmlStrEqual(type, BAD_CAST "list")
                        && xmlStrEqual(name, BAD_CAST "points")) {
                    conting_util_load_list(attr, &priv->points,
                            conting_line_load_point, NULL);
                }
            }
        }
    }

    for (n = priv->points; n != NULL; n = g_list_next(n)) {
        ArtPoint *p = n->data;
        if (p->x == link0.x && p->y == link0.y) {
            priv->link0 = n->data;
        } else if (p->x == link1.x && p->y == link1.y) {
            priv->link1 = n->data;
        }
    }

    priv->placing = FALSE;
    priv->placed = TRUE;

    CONTING_DRAWING_CLASS(parent_class)->place_xml(self, drawing_node,
            id_drawing);
}

static xmlNodePtr
conting_line_xml_node(ContingDrawing *self, xmlNodePtr drawing_node)
{
	ContingLinePrivate *priv;
	xmlNodePtr class_node;

	g_return_val_if_fail(self != NULL && CONTING_IS_LINE(self), NULL);

	priv = CONTING_LINE_GET_PRIVATE(self);

	class_node = xmlNewNode(NULL, BAD_CAST "class");
	xmlNewProp(class_node, BAD_CAST "name",
			BAD_CAST g_type_name(CONTING_TYPE_LINE));

	xmlAddChild(class_node,
			conting_util_drawing_node("comp0", CONTING_DRAWING(priv->comp0)));
	xmlAddChild(class_node,
			conting_util_drawing_node("comp1", CONTING_DRAWING(priv->comp1)));
	xmlAddChild(class_node,
			conting_util_point_node("link0", priv->link0));
	xmlAddChild(class_node,
			conting_util_point_node("link1", priv->link1));
	xmlAddChild(class_node,
			conting_util_list_node("points", priv->points,
				conting_line_point_node, NULL));

	xmlAddChild(drawing_node, class_node);

	return CONTING_DRAWING_CLASS(parent_class)->xml_node(self, drawing_node);
}

#include <gdk/gdkkeysyms.h>
static gboolean
conting_line_event_place(ContingDrawing *self,
		                 GdkEvent *event)
{
	ContingLinePrivate *priv;
	gdouble affine[6];
	ArtPoint pw;

	g_return_val_if_fail(self != NULL && CONTING_IS_LINE(self), FALSE);
	g_return_val_if_fail(event != NULL, FALSE);

	priv = CONTING_LINE_GET_PRIVATE(self);

	conting_one_line_window_to_world(conting_drawing_get_one_line(self),
			event->button.x, event->button.y, &pw.x, &pw.y);

	conting_drawing_get_i2w_affine(self, affine);

/*
	art_affine_translate(affine, pw.x, pw.y);
	conting_drawing_affine_absolute(self, affine);
	*/

	switch (event->type) {
		case GDK_MOTION_NOTIFY:
			priv->placing_point = pw;
			if (priv->placing && (event->motion.state & GDK_SHIFT_MASK)) {
				ArtPoint last_point;
				gdouble invert[6];
				gdouble angle, s, c;

				last_point = *((ArtPoint *) g_list_last(priv->points)->data);
				art_affine_point(&last_point, &last_point, affine);

				angle = atan2(pw.y - last_point.y, pw.x - last_point.x);

				/* TODO: improve to allow all possibilities */
				s = sin(angle);
				c = cos(angle);

				art_affine_invert(invert, affine);
				if (fabs(s) <= 0.33) {
					art_affine_point(&last_point, &last_point, invert);
					priv->placing_point.y = last_point.y;
					/*
					art_affine_translate(affine, pw.x, last_point.y);
					*/
				} else if (fabs(s) >= 0.66) {
					art_affine_point(&last_point, &last_point, invert);
					priv->placing_point.x = last_point.x;
					/*
					art_affine_translate(affine, last_point.x, pw.y);
					*/
				} else {
					gdouble size;
				   
					size = MIN(fabs(pw.x - last_point.x),
							fabs(pw.y - last_point.y));
						
					last_point.x += (c < 0 ? -size : size);
					last_point.y += (s < 0 ? -size : size);

					art_affine_point(&last_point, &last_point, invert);
					priv->placing_point.x = last_point.x;
					priv->placing_point.y = last_point.y;
				}
				

				/*
				art_affine_point(&priv->placing_point, &priv->placing_point,
						affine);
				conting_drawing_affine_absolute(self, affine);
			    */
			}
			break;
		case GDK_KEY_PRESS:
			if (event->key.keyval == GDK_Escape) {
				conting_drawing_delete(self);
			}

		default:
			break;
	}

	return TRUE;
}
static gboolean
conting_line_event(ContingDrawing *self,
				   GdkEvent *event)
{
	ContingLinePrivate *priv;
	gdouble affine[6];
	ArtPoint pi;
	GList *n;

	g_return_val_if_fail(self != NULL && CONTING_IS_LINE(self), FALSE);
	g_return_val_if_fail(event != NULL, FALSE);

	priv = CONTING_LINE_GET_PRIVATE(self);

	if (!priv->placed)
		return conting_line_event_place(self, event);

	assert(priv->placed);

	conting_drawing_get_i2w_affine(self, affine);

	conting_one_line_window_to_world(conting_drawing_get_one_line(self),
			event->button.x, event->button.y, &pi.x, &pi.y);
	art_affine_invert(affine, affine);
	art_affine_point(&pi, &pi, affine);

	switch (event->type) {
		case GDK_BUTTON_PRESS:
			conting_drawing_set_selected(self, TRUE);
			conting_drawing_update(self);


			if (event->button.state & GDK_CONTROL_MASK) {
				ArtPoint *new_p = g_new(ArtPoint, 1);
				*new_p = pi;

				g_list_insert_before(priv->points, priv->last_answer, new_p);
				
			}
			for (n = priv->points; n != NULL; n = g_list_next(n)) {
				ArtPoint *p = n->data;

				if (fabs(p->x - pi.x) < TOLERANCE
						&& fabs(p->y - pi.y) < TOLERANCE) {
					g_print("pressed!\n");
					priv->dragging_point = p;
					conting_drawing_grab(self);
					break;
				}
			}
			break;
		case GDK_BUTTON_RELEASE:
			g_print("released!\n");
			if (priv->dragging_point) {
				priv->dragging_point = NULL;
				conting_drawing_ungrab(self);
			}
			break;
		case GDK_MOTION_NOTIFY:
			if (priv->dragging_point) {
				if (priv->dragging_point == priv->link0
						|| priv->dragging_point == priv->link1) {
				} else {
					*(priv->dragging_point) = pi;
				}
			}
			break;
		case GDK_KEY_PRESS:
			if (event->key.keyval == GDK_Delete) {
				conting_drawing_delete(self);
			}
			break;
		default:
			return FALSE;
	}
	return TRUE;
}

static gboolean
conting_line_answer(ContingDrawing *self,
					  gdouble world_x, gdouble world_y)
{
	ContingLinePrivate *priv;
	GList *n;
	ArtPoint pi, *p0, *p1;
	gdouble invert[6];

	g_return_val_if_fail(self != NULL && CONTING_IS_LINE(self), FALSE);

    priv = CONTING_LINE_GET_PRIVATE(self);

	assert(priv->placed);
	assert(priv->points);

	conting_drawing_get_i2w_affine(self, invert);
	art_affine_invert(invert, invert);
	pi.x = world_x;
	pi.y = world_y;
	art_affine_point(&pi, &pi, invert);

	g_print("%lf %lf) -> (%lf, %lf)\n",
			world_x, world_y,
			pi.x, pi.y);

	p0 = priv->points->data;
	for (n = g_list_next(priv->points); n != NULL; n = g_list_next(n)) {
		gdouble dx, dy;
		gdouble m, d;
		gdouble ix, iy;

		if (fabs(p0->x - pi.x) < TOLERANCE
				&& fabs(p0->y - pi.y) < TOLERANCE) {
			return TRUE;
		}
		
		p1 = n->data;

		dx = p1->x - p0->x;
		dy = p1->y - p0->y;

		ix = pi.x - p0->x;
		iy = p0->y - pi.y;

		m = dy / dx;

		d = fabs(m * ix + iy) / sqrt(m * m + 1);

		if (d < TOLERANCE && fabs(ix) < fabs(dx) && fabs(iy) < fabs(dy)) {
			priv->last_answer = n;
			return TRUE;
		}

		p0 = p1;
	}

	return FALSE;
}


static void conting_line_class_init(gpointer g_class, gpointer class_data) {
    ContingDrawingClass *drawing_class;
	GObjectClass *gobject_class;

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = conting_line_draw;
	drawing_class->get_bounds = conting_line_get_bounds;
	drawing_class->get_update_bounds = conting_line_get_bounds;
	drawing_class->place = conting_line_place;
	drawing_class->is_placed = conting_line_is_placed;
	drawing_class->answer = conting_line_answer;
	drawing_class->event = conting_line_event;
	drawing_class->delete = conting_line_delete;

	drawing_class->xml_node = conting_line_xml_node;
	drawing_class->place_xml = conting_line_place_xml;

	drawing_class->get_center = conting_line_get_center;

	gobject_class = G_OBJECT_CLASS(g_class);
	gobject_class->finalize = conting_line_finalize;

	g_type_class_add_private(g_class, sizeof(ContingLinePrivate));

	parent_class = g_type_class_peek_parent(g_class);
}

GType conting_line_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingLineClass),
            NULL,
            NULL,
            conting_line_class_init,
            NULL,
            NULL,
            sizeof(ContingLine),
            0,
            conting_line_instance_init,
            NULL
        };

        type = g_type_register_static(CONTING_TYPE_DRAWING,
                "ContingLine",
                &type_info, 0);
    }

    return type;
}
