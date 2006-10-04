#ifndef CONTING_COMPONENT_H
#include "contingcomponent.h"
#endif

#ifndef CONTING_DRAWING_H
#define CONTING_DRAWING_H

#include <glib-object.h>
#include <gdk/gdk.h>
#include <libart_lgpl/libart.h>


#define CONTING_DRAWING_TOLERANCE 2


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
typedef struct ContingDrawingClass_ ContingDrawingClass;

#ifndef CONTING_DATA_H
#include "contingdata.h"
#endif

struct ContingDrawing_ {
    GObject parent;
};

typedef gboolean (*ContingDrawingPredicate)(ContingDrawing *drawing,
		gpointer user_data);

typedef void (*ContingDrawingBoxDrawer)(ContingDrawing *drawing,
		gdouble world_x, gdouble world_y);

struct ContingDrawingClass_ {
    GObjectClass parent;

    void (*draw)(ContingDrawing *self, cairo_t *cr);
	void (*draw_selection)(ContingDrawing *self,
			ContingDrawingBoxDrawer drawer);

    void (*place)(ContingDrawing *self);
    gboolean (*is_placed)(ContingDrawing *self);

    gboolean (*answer)(ContingDrawing *self,
            gdouble world_x, gdouble world_y);

    void (*get_bounds)(ContingDrawing *self, ArtDRect *rect);
    void (*get_update_bounds)(ContingDrawing *self, ArtDRect *rect);

    void (*get_i2w_affine)(ContingDrawing *self, gdouble affine[6]);
    void (*get_w2i_affine)(ContingDrawing *self, gdouble affine[6]);

    void (*delete)(ContingDrawing *self);
    gboolean (*event)(ContingDrawing *self, GdkEvent *event);

	void (*get_center)(ContingDrawing *self,
		               ArtPoint *pw_dst, const ArtPoint *pw_src);


	void (*find_link)(ContingDrawing *self,
			ContingDrawingPredicate pred,
			gpointer user_data);

	void (*grab)(ContingDrawing *drawing, ArtPoint *pi);
	void (*ungrab)(ContingDrawing *drawing);
	void (*motion)(ContingDrawing *drawing, ArtPoint *pi);


	void (*motion_place)(ContingDrawing *drawing, ArtPoint *pi);
};

GType conting_drawing_get_type(void);

void conting_drawing_draw(ContingDrawing *self, cairo_t *cr);

#include "contingoneline.h"
#include "continggroup.h"

ContingOneLine *conting_drawing_get_one_line(ContingDrawing *self);
ContingGroup *conting_drawing_get_group(ContingDrawing *self);
void conting_drawing_affine(ContingDrawing *self, const gdouble affine[6]);
void conting_drawing_affine_absolute(ContingDrawing *self,
        const gdouble affine[6]);
void conting_drawing_get_i2w_affine(ContingDrawing *self, gdouble affine[6]);
void conting_drawing_get_w2i_affine(ContingDrawing *self, gdouble affine[6]);
void conting_drawing_i2w(ContingDrawing *self, ArtPoint *src, ArtPoint *dst);
void conting_drawing_w2i(ContingDrawing *self, ArtPoint *src, ArtPoint *dst);

void conting_drawing_get_bounds(ContingDrawing *self, ArtDRect *rect);
void conting_drawing_get_update_bounds(ContingDrawing *self, ArtDRect *rect);

void conting_drawing_place(ContingDrawing *self);
gboolean conting_drawing_is_placed(ContingDrawing *self);
gboolean conting_drawing_answer(ContingDrawing *self,
        gdouble world_x, gdouble world_y);

gboolean conting_drawing_event(ContingDrawing *self, GdkEvent *event);

void conting_drawing_grab(ContingDrawing *self, ArtPoint *pi);
void conting_drawing_ungrab(ContingDrawing *self);
void conting_drawing_update(ContingDrawing *self);

void conting_drawing_motion(ContingDrawing *self, ArtPoint *pi);
void conting_drawing_motion_place(ContingDrawing *self, ArtPoint *pi);

void conting_drawing_set_selected(ContingDrawing *self, gboolean selected);
gboolean conting_drawing_is_selected(ContingDrawing *self);
void conting_drawing_delete(ContingDrawing *self);

void conting_drawing_get_center(ContingDrawing *self,
		                        ArtPoint *pw_dst, const ArtPoint *pw_src);

void conting_drawing_find_link(ContingDrawing *self,
		ContingDrawingPredicate pred, gpointer user_data);

gpointer conting_drawing_get_attr(ContingDrawing *self, const gchar *attr);


#define CONTING_TYPE_DRAWING_EVENT	(conting_drawing_event_get_type())
GType
conting_drawing_event_get_type(void);

typedef enum {
    CONTING_DRAWING_NOTHING,
    CONTING_DRAWING_MOTION_NOTIFY,
    CONTING_DRAWING_BUTTON_PRESS,
    CONTING_DRAWING_2BUTTON_PRESS,
    CONTING_DRAWING_BUTTON_RELEASE,
    CONTING_DRAWING_KEY_PRESS,
    CONTING_DRAWING_KEY_RELEASE,
    CONTING_DRAWING_ENTER,
    CONTING_DRAWING_LEAVE
} ContingDrawingEventType;

typedef struct {
    ContingDrawingEventType type;
    gdouble x, y;
	guint state;
} ContingDrawingEventMotion;

typedef struct {
    ContingDrawingEventType type;
    gdouble x, y;
    guint state;
    guint button;
} ContingDrawingEventButton;

typedef struct {
    ContingDrawingEventType type;
    guint state;
    guint keyval;
} ContingDrawingEventKey;

typedef struct {
    ContingDrawingEventType type;
    gdouble x, y;
} ContingDrawingEventNotify;

typedef union {
    ContingDrawingEventType type;
    ContingDrawingEventMotion motion;
    ContingDrawingEventButton button;
    ContingDrawingEventKey key;
    ContingDrawingEventNotify notify;
} ContingDrawingEvent;

ContingDrawingEvent *
conting_drawing_event_new(void);

#endif /* CONTING_DRAWING_H */
