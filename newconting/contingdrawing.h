#ifndef CONTING_DRAWING_H
#define CONTING_DRAWING_H

#include <glib-object.h>
#include <gdk/gdk.h>
#include <libart_lgpl/libart.h>
#include <libxml/tree.h>

G_BEGIN_DECLS

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

    void (*draw)(ContingDrawing *self, GdkDrawable *drawable,
            const GdkRectangle *drawing_rect);

    void (*place)(ContingDrawing *self);
    gboolean (*is_placed)(ContingDrawing *self);

    gboolean (*answer)(ContingDrawing *self,
            gdouble world_x, gdouble world_y);

    void (*get_bounds)(ContingDrawing *self, ArtDRect *rect);

    void (*get_i2w_affine)(ContingDrawing *self, gdouble affine[6]);
    void (*get_w2i_affine)(ContingDrawing *self, gdouble affine[6]);

    void (*delete)(ContingDrawing *self);

    gboolean (*event)(ContingDrawing *self, GdkEvent *event);

    xmlNodePtr (*xml_node)(ContingDrawing *self, xmlNodePtr drawing_node);
    void (*place_xml)(ContingDrawing *self, xmlNodePtr drawing_node,
            GHashTable *id_drawing);
};

GType conting_drawing_get_type(void);

void conting_drawing_draw(ContingDrawing *self,
        GdkDrawable *drawable, const GdkRectangle *drawing_rect);

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

void conting_drawing_place(ContingDrawing *self);
gboolean conting_drawing_is_placed(ContingDrawing *self);
gboolean conting_drawing_answer(ContingDrawing *self,
        gdouble world_x, gdouble world_y);

gboolean conting_drawing_event(ContingDrawing *self, GdkEvent *event);

void conting_drawing_grab(ContingDrawing *self);
void conting_drawing_ungrab(ContingDrawing *self);
void conting_drawing_update(ContingDrawing *self);

void conting_drawing_set_selected(ContingDrawing *self, gboolean selected);
gboolean conting_drawing_is_selected(ContingDrawing *self);
void conting_drawing_delete(ContingDrawing *self);
xmlNodePtr conting_drawing_xml_node(ContingDrawing *self,
        xmlNodePtr drawing_node);
void conting_drawing_place_xml(ContingDrawing *self, xmlNodePtr node,
                               GHashTable *id_drawing);

G_END_DECLS

#endif /* CONTING_DRAWING_H */
