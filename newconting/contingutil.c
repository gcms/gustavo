#include "contingutil.h"
#include "contingdrawing.h"
#include <math.h>

void conting_util_get_bounds(const ArtPoint *p0, const ArtPoint *p1,
		ArtDRect *bounds) {
	bounds->x0 = MIN(p0->x, p1->x);
	bounds->y0 = MIN(p0->y, p1->y);

	bounds->x1 = MAX(p0->x, p1->x);
	bounds->y1 = MAX(p0->y, p1->y);
}

void conting_util_expand_bounds(ArtDRect *bounds, gdouble value) {
	bounds->x0 -= value;
	bounds->y0 -= value;

	bounds->x1 += value;
	bounds->y1 += value;
}
void conting_util_union_bounds(const ArtDRect *bounds0,
		const ArtDRect *bounds1, ArtDRect *bounds) {
	bounds->x0 = MIN(bounds0->x0, bounds1->x0);
	bounds->y0 = MIN(bounds0->y0, bounds1->y0);

	bounds->x1 = MAX(bounds0->x1, bounds1->x1);
	bounds->y1 = MAX(bounds0->y1, bounds1->y1);
}
void conting_util_correct_bounds(const ArtDRect *bounds,
		ArtDRect *dst) {
	ArtDRect new_bounds;
	
	new_bounds.x0 = MIN(bounds->x0, bounds->x1);
	new_bounds.y0 = MIN(bounds->y0, bounds->y1);

	new_bounds.x1 = MAX(bounds->x0, bounds->x1);
	new_bounds.y1 = MAX(bounds->y0, bounds->y1);

	*dst = new_bounds;
}
void conting_util_bounds_to_rect(const ArtDRect *src, GdkRectangle *dst) {
	dst->x = (gint) MIN(src->x0, src->x1);
	dst->y = (gint) MIN(src->y0, src->y1);

	dst->width = (gint) fabs(src->x1 - src->x0);
	dst->height = (gint) fabs(src->y1 - src->y0);
}

/*
 * b1 contains b2?
 */
gboolean conting_util_bounds_contains(const ArtDRect *b1, const ArtDRect *b2) {
	return b1->x0 <= b2->x0 && b1->y0 <= b2->y0
		&& b1->x1 >= b2->x1 && b1->y1 >= b2->y1;
}
void conting_util_bounds_add_point(ArtDRect *dr, const ArtPoint *p) {
	dr->x0 = MIN(dr->x0, p->x);
	dr->y0 = MIN(dr->y0, p->y);
	dr->x1 = MAX(dr->x1, p->x);
	dr->y1 = MAX(dr->y1, p->y);
}
