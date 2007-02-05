#include <gtk/gtk.h>
#include "contingsysteminfo.h"

#include "contingloadflow.h"

GtkDialog *
conting_system_info_get_dialog(ContingData *data)
{
	GtkWidget *dialog;
	
	dialog = gtk_dialog_new_with_buttons("System information", NULL,
			GTK_DIALOG_MODAL,
            GTK_STOCK_OK, GTK_RESPONSE_OK,
            NULL);

	{
		ContingLoadFlowConfig *cfg = conting_load_flow_config_new();

		conting_load_flow_config_add_int(cfg, 3);
		conting_load_flow_config_add_float(cfg, 3.2);
		conting_load_flow_config_end_section(cfg);

		conting_load_flow_config_add_int(cfg, 1);
		conting_load_flow_config_add_int(cfg, 2);
		conting_load_flow_config_end_section(cfg);

		g_print("%s\n", conting_load_flow_config_get_text(cfg));

		g_object_unref(cfg);
	}
            
    g_signal_connect_swapped (dialog,
                             "response", 
                             G_CALLBACK (gtk_widget_destroy),
                             dialog);

    return GTK_DIALOG(dialog);
}
