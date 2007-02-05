#ifndef CONTING_CONFIG_DIALOG_H
#define CONTING_CONFIG_DIALOG_H


#define CONTING_TYPE_CONFIG_DIALOG        (conting_config_dialog_get_type())
#define CONTING_CONFIG_DIALOG(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_CONFIG_DIALOG, ContingConfigDialog))
#define CONTING_CONFIG_DIALOG_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_CONFIG_DIALOG, ContingConfigDialogClass))
#define CONTING_IS_CONFIG_DIALOG(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_CONFIG_DIALOG))
#define CONTING_IS_CONFIG_DIALOG_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_CONFIG_DIALOG))
#define CONTING_CONFIG_DIALOG_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_CONFIG_DIALOG, ContingConfigDialogClass))

typedef struct ContingConfigDialog_ ContingConfigDialog;
typedef struct ContingConfigDialogClass_ ContingConfigDialogClass;

#include <gtk/gtk.h>

struct ContingConfigDialog_ {
    GtkDialog parent;
};

struct ContingConfigDialogClass_ {
	GtkDialogClass parent;
};

GType
conting_config_dialog_get_type(void);

#include "contingconfig.h"
GtkDialog *
conting_config_dialog_new(ContingConfig *config);
GHashTable *
conting_config_dialog_get_values(ContingConfigDialog *self);

#define conting_config_dialog_get_value(s, n) g_hash_table_lookup( \
		conting_config_dialog_get_values(s), n)
#define conting_config_dialog_get_int(s, n) g_value_get_int( \
			conting_config_dialog_get_value(s, n))
#define conting_config_dialog_get_double(s, n) g_value_get_double( \
			conting_config_dialog_get_value(s, n))

#endif /* CONTING_CONFIG_DIALOG_H */
