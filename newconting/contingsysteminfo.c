#include <gtk/gtk.h>
#include "contingsysteminfo.h"

GtkDialog *
conting_system_info_get_dialog(ContingModel *model)
{
	GtkWidget *dialog;
	gchar buff[256];
	
	dialog = gtk_dialog_new_with_buttons("System information", NULL,
			GTK_DIALOG_MODAL,
            GTK_STOCK_OK, GTK_RESPONSE_OK,
            NULL);

	sprintf(buff, "Numero de barras: %4d\n",
			g_list_length((GList *) conting_model_get_buses(model)));
	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),
			gtk_label_new(buff));

	sprintf(buff, "Numero de ramos: %4d\n",
			g_list_length((GList *) conting_model_get_branches(model)));
	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),
			gtk_label_new(buff));

	gtk_widget_show_all(GTK_DIALOG(dialog)->vbox);

            
    g_signal_connect_swapped (dialog,
                             "response", 
                             G_CALLBACK (gtk_widget_destroy),
                             dialog);

    return GTK_DIALOG(dialog);
}
