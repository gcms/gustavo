#include "continginfodialog.h"
#include "contingdata.h"

enum {
    CONTING_INFO_DIALOG_PROP_0,
    CONTING_INFO_DIALOG_PROP_DATA,
    CONTING_INFO_DIALOG_PROP_DRAWING
};

#define CONTING_INFO_DIALOG_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE((o), \
            CONTING_TYPE_INFO_DIALOG, ContingInfoDialogPrivate))

typedef struct ContingInfoDialogPrivate_ ContingInfoDialogPrivate;
struct ContingInfoDialogPrivate_ {
    ContingData *data;
    ContingDrawing *drawing;
};
static void
conting_info_dialog_get_property(GObject *self,
                             guint prop_id,
                             GValue *value,
                             GParamSpec *pspec)
{
    ContingInfoDialogPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_INFO_DIALOG(self));

    priv = CONTING_INFO_DIALOG_GET_PRIVATE(self);

    switch (prop_id) {
        case CONTING_INFO_DIALOG_PROP_DATA:
            g_value_set_object(value, priv->data);
            break;
        case CONTING_INFO_DIALOG_PROP_DRAWING:
            g_value_set_object(value, priv->drawing);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

static void
conting_info_dialog_set_property(GObject *self,
                             guint prop_id,
                             const GValue *value,
                             GParamSpec *pspec)
{
    ContingInfoDialogPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_INFO_DIALOG(self));

    priv = CONTING_INFO_DIALOG_GET_PRIVATE(self);

    switch (prop_id) {
        case CONTING_INFO_DIALOG_PROP_DATA:
            priv->data = g_value_get_object(value);
            break;
        case CONTING_INFO_DIALOG_PROP_DRAWING:
            priv->drawing = g_value_get_object(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

static void
conting_info_dialog_class_init(gpointer g_class, gpointer class_data)
{
    GObjectClass *gobject_class;

    gobject_class = G_OBJECT_CLASS(g_class);
    gobject_class->set_property = conting_info_dialog_set_property;
    gobject_class->get_property = conting_info_dialog_get_property;


    g_object_class_install_property(gobject_class,
            CONTING_INFO_DIALOG_PROP_DATA,
            g_param_spec_object("data",
                                "ContingData",
                                "ContingData associated with the drawing",
                                CONTING_TYPE_DATA,
                                G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY
                                | G_PARAM_WRITABLE));
                                
    g_object_class_install_property(gobject_class,
            CONTING_INFO_DIALOG_PROP_DRAWING,
            g_param_spec_object("drawing",
                                "ContingDrawing",
                                "ContingDrawing associated",
                                CONTING_TYPE_DRAWING,
                                G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY
                                | G_PARAM_WRITABLE));

    g_type_class_add_private(g_class, sizeof(ContingInfoDialogPrivate));
}

static void
conting_info_dialog_instance_init(GTypeInstance *self, gpointer g_class)
{
    ContingInfoDialogPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_INFO_DIALOG(self));

    priv = CONTING_INFO_DIALOG_GET_PRIVATE(self);

    priv->data = NULL;
    priv->drawing = NULL;
}


GType
conting_info_dialog_get_type(void)
{
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingInfoDialogClass),
            NULL,
            NULL,
            conting_info_dialog_class_init,
            NULL,
            NULL,
            sizeof(ContingInfoDialog),
            0,
            conting_info_dialog_instance_init,
            NULL
        };

        type = g_type_register_static(GTK_TYPE_WIDGET, "ContingInfoDialog",
                &type_info, 0);
    }

    return type;
}
