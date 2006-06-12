#ifndef CONTING_UTIL_H
#define CONTING_UTIL_H
#include <gtk/gtk.h>
#include <libart_lgpl/libart.h>
#include <libxml/tree.h>
#include "contingdrawing.h"

typedef xmlNodePtr (*ContingSerializeFunc)(gconstpointer object);

#define conting_util_in_bounds(b, p) ((p)->x >= (b)->x0 && (p)->x <= (b)->x1 \
		&& (p)->y >= (b)->y0 && (p)->y <= (b)->y1)

void conting_util_get_bounds(const ArtPoint *p1, const ArtPoint *p2,
		ArtDRect *bounds);

void conting_util_expand_bounds(ArtDRect *bounds, gdouble value);

void conting_util_union_bounds(const ArtDRect *bounds0,
		const ArtDRect *bounds1, ArtDRect *bounds);

void conting_util_correct_bounds(const ArtDRect *src, ArtDRect *dst);
void conting_util_bounds_to_rect(const ArtDRect *src, GdkRectangle *dst);

gboolean conting_util_bounds_contains(const ArtDRect *b1, const ArtDRect *b2);

xmlNodePtr conting_util_affine_node(const char *name, gdouble affine[6]);
xmlNodePtr conting_util_point_node(const char *name, ArtPoint *p);
xmlNodePtr conting_util_drawing_node(const char *name, ContingDrawing *drawing);
xmlNodePtr conting_util_hash_node(const char *name,
		                          GHashTable *hash_table,
								  ContingSerializeFunc key_func,
								  ContingSerializeFunc val_func);
xmlNodePtr conting_util_list_node(const char *name,
		                          GList *list,
								  ContingSerializeFunc serialize);
#endif /* CONTING_UTIL_H */
