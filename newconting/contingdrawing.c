#include "contingdrawing.h"
#include <string.h>

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

void
conting_drawing_get_affine(ContingDrawing *self,
		                   gdouble affine[6])
{
	ContingDrawingPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_DRAWING(self));

	priv = CONTING_DRAWING_GET_PRIVATE(self);

	memcpy(affine, priv->affine, 6 * sizeof(gdouble));
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

	art_affine_translate(priv->affine, 0.0, 0.0);
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
