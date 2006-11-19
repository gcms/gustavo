#include <gtk/gtk.h>
#include "contingsysteminfo.h"


GtkDialog *
conting_system_info_get_dialog(ContingData *data)
{
	GtkWidget *dialog;
	
	dialog = gtk_dialog_new_with_buttons("System information", NULL,
			GTK_DIALOG_MODAL,
            GTK_STOCK_OK, GTK_RESPONSE_OK,
            NULL);
            
    g_signal_connect_swapped (dialog,
                             "response", 
                             G_CALLBACK (gtk_widget_destroy),
                             dialog);

    return GTK_DIALOG(dialog);
}