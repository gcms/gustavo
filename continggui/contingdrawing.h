#ifndef CONTING_DRAWING_H
#define CONTING_DRAWING_H

#include <glib-object.h>
#include <gdk/gdk.h>

#define CONTING_TYPE_DRAWING        (conting_drawing_get_type())
#define CONTING_DRAWING(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_DRAWING, ContingDrawing))
#define CONTING_DRAWING_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_DRAWING, ContingDrawingClass))
#define CONTING_IS_DRAWING(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_DRAWING))
#define CONTING_IS_DRAWING_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_DRAWING))
#define CONTING_DRAWING_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_DRAWING, ContingDrawingClass))

typedef struct ContingDrawing_ ContingDrawing;
struct ContingDrawing_ {
    GObject parent;
};

typedef struct ContingDrawingClass_ ContingDrawingClass;
struct ContingDrawingClass_ {
    GObjectClass parent;

	/*
	void (*get_rectangle)(ContingDrawing *self, GdkRectangle *rect);	
	*/
    void (*draw) (ContingDrawing *self, GdkDrawable *drawable, GdkGC *gc,
			const GdkPoint *base_position, const GdkPoint *cur_position);
    void (*draw_selected) (ContingDrawing *self, GdkDrawable *drawable,
			GdkGC *gc, const GdkPoint *position);

	void (*start_place)(ContingDrawing *self);

	void (*place)(ContingDrawing *self, gint x, gint y,
			ContingDrawing *other);

	gboolean (*is_placed)(ContingDrawing *self);

	gboolean (*answer)(ContingDrawing *self, gint x, gint y);

	/*
	gboolean (*violates)(ContingDrawing *self, ContingDrawing *other,
			gint x, gint y);

	gboolean (*can_link)(ContingDrawing *self, ContingDrawing *link_drawing,
			gint x, gint y);

	gboolean (*place_linked)(ContingDrawing *self, gint selfx, gint selfy,
			ContingDrawing *other, gint otherx, gint othery);
    gboolean (*place) (ContingDrawing *self);
    gboolean (*is_link_point)(ContingDrawing *self,
            const GdkPoint *drawing_pos, const GdkPoint *ev_pos);
    void (*select)(ContingDrawing *self);
    void (*get_points) (ContingDrawing *self,
            GdkPoint **link_points, gint *npoints);
    void (*get_rectangle) (ContingDrawing *self, GdkRectangle *rect);
    void (*draw) (ContingDrawing *self, GdkDrawable *drawable, GdkGC *gc,
            const GdkPoint *position);

    gboolean (*place) (ContingDrawing *self, const GdkPoint *coord);
	*/
};

GType conting_drawing_get_type(void);
/*
void conting_drawing_get_points(ContingDrawing *self,
        GdkPoint **links_points, gint *n_links);
*/

/*
void conting_drawing_get_rectangle(ContingDrawing *self,
        GdkRectangle *rect);
*/

void conting_drawing_draw(ContingDrawing *self,
		GdkDrawable *drawable, GdkGC *gc,
		const GdkPoint *base_position, const GdkPoint *cur_position);
void conting_drawing_draw_selected(ContingDrawing *self,
		GdkDrawable *drawable, GdkGC *gc, const GdkPoint *coord);
/*
gboolean conting_drawing_violates(ContingDrawing *self,
		ContingDrawing *other, gint x, gint y);
gboolean conting_drawing_can_link(ContingDrawing *self,
		ContingDrawing *link_drawing, gint x, gint y);
gboolean conting_drawing_place(ContingDrawing *self);
gboolean conting_drawing_place_linked(ContingDrawing *self,
		gint selfx, gint selfy,
		ContingDrawing *other, gint otherx, gint othery);
*/

void conting_drawing_start_place(ContingDrawing *self);

void conting_drawing_place(ContingDrawing *self, gint x, gint y,
		ContingDrawing *other);

gboolean conting_drawing_is_placed(ContingDrawing *self);

gboolean conting_drawing_answer(ContingDrawing *self, gint x, gint y);


#endif /* CONTING_DRAWING_H */
