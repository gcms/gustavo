#ifndef CONTING_INFO_DIALOG_H
#define CONTING_INFO_DIALOG_H

#include <glib-object.h>



#define CONTING_TYPE_INFO_DIALOG            (conting_info_dialog_get_type())
#define CONTING_INFO_DIALOG(o)              (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_INFO_DIALOG, ContingInfoDialog))
#define CONTING_INFO_DIALOG_CLASS(k)        (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_INFO_DIALOG, ContingInfoDialogClass))
#define CONTING_IS_INFO_DIALOG(o)           (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_INFO_DIALOG))
#define CONTING_IS_INFO_DIALOG_CLASS(k)     (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_INFO_DIALOG))
#define CONTING_INFO_DIALOG_GET_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_INFO_DIALOG, ContingInfoDialogClass))

typedef struct ContingInfoDialog_ ContingInfoDialog;
typedef struct ContingInfoDialogClass_ ContingInfoDialogClass;

#include <gtk/gtk.h>

struct ContingInfoDialog_ {
    GtkDialog parent;  
};

struct ContingInfoDialogClass_ {
    GtkDialogClass parent;
};

GType
conting_info_dialog_get_type(void);

#include "contingdata.h"

GtkWidget *
conting_info_dialog_new(ContingData *data, ContingDrawing *drawing);



#endif /* CONTING_INFO_DIALOG_H */
