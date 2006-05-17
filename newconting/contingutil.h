#ifndef CONTING_UTIL_H
#define CONTING_UTIL_H
#include <gtk/gtk.h>
#include <libart_lgpl/libart.h>

void conting_util_get_bounds(const ArtPoint *p1, const ArtPoint *p2,
		ArtDRect *bounds);

void conting_util_expand_bounds(ArtDRect *bounds, gdouble value);

void conting_util_union_bounds(const ArtDRect *bounds0,
		const ArtDRect *bounds1, ArtDRect *bounds);

void conting_util_correct_bounds(const ArtDRect *src, ArtDRect *dst);
void conting_util_bounds_to_rect(const ArtDRect *src, GdkRectangle *dst);
#endif /* CONTING_UTIL_H */
