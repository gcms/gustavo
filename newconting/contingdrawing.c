#include "contingdrawing.h"
#include <string.h>

static gint move_signal_id = 0;
static gint delete_signal_id = 0;

enum {
	CONTING_DRAWING_PROP_0,
    CONTING_DRAWING_PROP_ONE_LINE
};

#define CONTING_DRAWING_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE((o), \
            CONTING_TYPE_DRAWING, ContingDrawingPrivate))

typedef struct ContingDrawingPrivate_ ContingDrawingPrivate;
struct ContingDrawingPrivate_ {
    ContingOneLine *oneline;
	gdouble affine[6];

	gboolean selected;
};

void
conting_drawing_draw(ContingDrawing *self,
                     GdkDrawable *drawable,
                     const GdkRectangle *drawing_rect)
{
    g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

    CONTING_DRAWING_GET_CLASS(self)->draw(self, drawable, drawing_rect);
}

ContingOneLine *
conting_drawing_get_one_line(ContingDrawing *self)
{
	ContingOneLine *oneline;

	g_object_get(G_OBJECT(self),
			"one-line", &oneline,
			NULL);

	return oneline;
}

void
conting_drawing_affine(ContingDrawing *self,
                       const gdouble affine[6])
{
	ContingDrawingPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	priv = CONTING_DRAWING_GET_PRIVATE(self);

	art_affine_multiply(priv->affine, priv->affine, affine);
}
void
conting_drawing_affine_absolute(ContingDrawing *self,
                       const gdouble affine[6])
{
	ContingDrawingPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	priv = CONTING_DRAWING_GET_PRIVATE(self);

	memcpy(priv->affine, affine, 6 * sizeof(double));
}

static void
conting_drawing_get_affine_impl(ContingDrawing *self,
		                   gdouble affine[6])
{
	ContingDrawingPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	priv = CONTING_DRAWING_GET_PRIVATE(self);

	memcpy(affine, priv->affine, 6 * sizeof(gdouble));
}

void conting_drawing_get_affine(ContingDrawing *self,
						        gdouble affine[6])
{
    g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	CONTING_DRAWING_GET_CLASS(self)->get_affine(self, affine);
}

void
conting_drawing_get_bounds(ContingDrawing *self,
                           ArtDRect *rect)
{
	g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	CONTING_DRAWING_GET_CLASS(self)->get_bounds(self, rect);
}
void
conting_drawing_place(ContingDrawing *self)
{
	g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	CONTING_DRAWING_GET_CLASS(self)->place(self);
}

gboolean
conting_drawing_is_placed(ContingDrawing *self)
{
	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING(self), FALSE);

	return CONTING_DRAWING_GET_CLASS(self)->is_placed(self);
}
gboolean
conting_drawing_answer(ContingDrawing *self,
		               gdouble world_x, gdouble world_y)
{
	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING(self), FALSE);

	return CONTING_DRAWING_GET_CLASS(self)->answer(self, world_x, world_y);
}
gboolean
conting_drawing_event(ContingDrawing *self,
		              GdkEvent *event)
{
	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING(self), FALSE);

	if (CONTING_DRAWING_GET_CLASS(self)->event)
		return CONTING_DRAWING_GET_CLASS(self)->event(self, event);

	return FALSE;
}

void
conting_drawing_grab(ContingDrawing *self)
{
	g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	conting_one_line_grab(conting_drawing_get_one_line(self), self);
}

void
conting_drawing_ungrab(ContingDrawing *self)
{
	g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	conting_one_line_ungrab(conting_drawing_get_one_line(self), self);
}

void
conting_drawing_update(ContingDrawing *self)
{
	ArtDRect bounds;
	g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	conting_drawing_get_bounds(self, &bounds);
	conting_one_line_update(conting_drawing_get_one_line(self), &bounds);
}
void
conting_drawing_set_selected(ContingDrawing *self,
		                     gboolean selected)
{
	ContingDrawingPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	priv = CONTING_DRAWING_GET_PRIVATE(self);

	if (priv->selected != selected) {
		priv->selected = selected;
		conting_drawing_update(self);
	}
}
gboolean
conting_drawing_is_selected(ContingDrawing *self)
{
	ContingDrawingPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_DRAWING(self), FALSE);

	priv = CONTING_DRAWING_GET_PRIVATE(self);

	return priv->selected;
}

void
conting_drawing_delete(ContingDrawing *self)
{
	g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	CONTING_DRAWING_GET_CLASS(self)->delete(self);
}

static void
conting_drawing_delete_impl(ContingDrawing *self)
{
	ContingDrawingPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	priv = CONTING_DRAWING_GET_PRIVATE(self);

	conting_one_line_delete_drawing(conting_drawing_get_one_line(self),
			self);
}

static void
conting_drawing_get_property(GObject *self,
                             guint prop_id,
                             GValue *value,
                             GParamSpec *pspec)
{
    ContingDrawingPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

    priv = CONTING_DRAWING_GET_PRIVATE(self);

    switch (prop_id) {
        case CONTING_DRAWING_PROP_ONE_LINE:
            g_value_set_object(value, priv->oneline);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

static void
conting_drawing_set_property(GObject *self,
                             guint prop_id,
                             const GValue *value,
                             GParamSpec *pspec)
{
    ContingDrawingPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

    priv = CONTING_DRAWING_GET_PRIVATE(self);

    switch (prop_id) {
        case CONTING_DRAWING_PROP_ONE_LINE:
            priv->oneline = g_value_get_object(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

static void
conting_drawing_instance_init(GTypeInstance *self,
							  gpointer g_class)
{
    ContingDrawingPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

    priv = CONTING_DRAWING_GET_PRIVATE(self);

	art_affine_identity(priv->affine);

	priv->selected = FALSE;
}

static void
conting_drawing_class_init(gpointer g_class,
                           gpointer class_data)
{
    GObjectClass *gobject_class;
    ContingDrawingClass *drawing_class;

    gobject_class = G_OBJECT_CLASS(g_class);
    gobject_class->set_property = conting_drawing_set_property;
    gobject_class->get_property = conting_drawing_get_property;

    g_object_class_install_property(G_OBJECT_CLASS(g_class),
            CONTING_DRAWING_PROP_ONE_LINE,
            g_param_spec_object("one-line",
                                "OneLine container",
                                "The one-line diagram of this item",
                                CONTING_TYPE_ONE_LINE,
                                G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY
							    | G_PARAM_WRITABLE));

    drawing_class = CONTING_DRAWING_CLASS(g_class);
    drawing_class->draw = NULL;
    drawing_class->get_bounds = NULL;
	drawing_class->place = NULL;
	drawing_class->is_placed = NULL;
	drawing_class->answer = NULL;
	drawing_class->event = NULL;
	drawing_class->delete = conting_drawing_delete_impl;
	drawing_class->get_affine = conting_drawing_get_affine_impl;

	move_signal_id = g_signal_newv(
			"move",
			G_TYPE_FROM_CLASS(g_class),
			G_SIGNAL_RUN_FIRST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
			NULL, /* class closure */
			NULL, /* accumulator */
			NULL, /* accu_data */
			g_cclosure_marshal_VOID__VOID,
			G_TYPE_NONE, /* return_type */
			0,
			NULL);

	delete_signal_id = g_signal_newv(
			"delete",
			G_TYPE_FROM_CLASS(g_class),
			G_SIGNAL_RUN_FIRST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
			NULL,
			NULL,
			NULL,
			g_cclosure_marshal_VOID__VOID,
			G_TYPE_NONE,
			0,
			NULL);

	g_type_class_add_private(g_class, sizeof(ContingDrawingPrivate));
}

GType
conting_drawing_get_type(void)
{
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingDrawingClass),
            NULL,
            NULL,
            conting_drawing_class_init,
            NULL,
            NULL,
            sizeof(ContingDrawing),
            0,
            conting_drawing_instance_init,
            NULL
        };

        type = g_type_register_static(G_TYPE_OBJECT, "ContingDrawing",
                &type_info, 0);
    }

    return type;
}
