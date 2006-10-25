#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdlib.h>
#include <string.h>
#include "contingoneline.h"
#include "contingbus.h"
#include "contingline.h"
#include "contingtrans2.h"
#include "contingtrans3.h"
#include "continggen.h"
#include "contingload.h"

static ContingOneLine *oneline;

static GtkWidget *
get_error_list(GList *errors)
{
    GtkWidget *tree;
    GtkListStore *store;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GList *n;


    store = gtk_list_store_new(2, G_TYPE_UINT, G_TYPE_STRING);

    for (n = errors; n != NULL; n = g_list_next(n)) {
        GtkTreeIter iter;
        ContingError *error = n->data;
        guint bus_number;

        conting_item_data_get_attr(error->item_data,
                "number", &bus_number,
                NULL);

        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                0, bus_number,
                1, error->message,
                -1);
    }

    tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);

    renderer = gtk_cell_renderer_text_new();
    
    column = gtk_tree_view_column_new_with_attributes("Bus", renderer,
            "text", 0,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
    
    column = gtk_tree_view_column_new_with_attributes("Message", renderer,
            "text", 1,
            NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

    return tree;
}

static void
show_error_dialog(GList *errors)
{
    GtkWidget *dialog, *child;
    
    dialog = gtk_dialog_new_with_buttons("Errors", NULL, GTK_DIALOG_MODAL,
            GTK_STOCK_OK, GTK_RESPONSE_OK,
            NULL);

    g_signal_connect_swapped (dialog,
                             "response", 
                             G_CALLBACK (gtk_widget_destroy),
                             dialog);

    child = get_error_list(errors);
    
    gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), child);
 
    gtk_widget_show_all(dialog);
}

static void
show_ok_dialog(void)
{
    GtkWidget *dialog;

    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK, "%s", "Checks Done. No errors found.");
    g_signal_connect_swapped (dialog,
                             "response", 
                             G_CALLBACK (gtk_widget_destroy),
                             dialog);

    gtk_widget_show(dialog);
}

static gboolean
check_data(GList **errors) {
	ContingData *data;
    gboolean ok;

	g_object_get(oneline,
			"data", &data,
			NULL);

	ok = conting_data_check(data, errors);

	return ok;
}

static void
mode_menu_activate(GtkMenuItem *menuitem,
		gpointer user_data)
{
	GList *errors = NULL;
	
	if (check_data(&errors)) {
		conting_one_line_set_mode(oneline, (ContingOneLineMode) user_data);
	} else {
        show_error_dialog(errors);
		g_list_foreach(errors, (GFunc) conting_error_free, NULL);
		g_list_free(errors);
	}
}

static void
check_clicked(GtkMenuItem *menuitem,
		gpointer user_data)
{
	GList *errors = NULL;

    if (check_data(&errors)) {
        show_ok_dialog();
    } else {
        show_error_dialog(errors);
		g_list_foreach(errors, (GFunc) conting_error_free, NULL);
		g_list_free(errors);
    }
}

static void
save_menu_activate(GtkMenuItem *menuitem,
                   gpointer user_data)  
{
    GtkWidget *save;

    save = gtk_file_chooser_dialog_new("Save One Line Diagram",
            GTK_WINDOW(user_data),
            GTK_FILE_CHOOSER_ACTION_SAVE,
            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
            GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
            NULL);
	gtk_file_chooser_set_action(GTK_FILE_CHOOSER(save),
			GTK_FILE_CHOOSER_ACTION_SAVE);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(save),
			TRUE);

    if (gtk_dialog_run(GTK_DIALOG(save)) == GTK_RESPONSE_ACCEPT) {
        gchar *filename;

        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(save));
        conting_one_line_save(oneline, filename);
        g_free(filename);
    }

    gtk_widget_destroy(save);
}

static void
open_menu_activate(GtkMenuItem *menuitem,
                   gpointer user_data)  
{
    GtkWidget *open;
	GtkFileFilter *filter;

	filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter, "*.xml");

    open = gtk_file_chooser_dialog_new("Open One Line Diagram",
            GTK_WINDOW(user_data),
            GTK_FILE_CHOOSER_ACTION_OPEN,
            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
            NULL);
	gtk_file_chooser_set_action(GTK_FILE_CHOOSER(open),
			GTK_FILE_CHOOSER_ACTION_OPEN);
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(open),
			filter);

    if (gtk_dialog_run(GTK_DIALOG(open)) == GTK_RESPONSE_ACCEPT) {
        gchar *filename;

        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(open));
        conting_one_line_open(oneline, filename);
        g_free(filename);
    }

	g_print("UNREF\n");
    gtk_widget_destroy(open);
}

static void
load_menu_activate(GtkMenuItem *menuitem,
                   gpointer user_data)  
{
    GtkWidget *open;


    open = gtk_file_chooser_dialog_new("Open One Line Diagram",
            GTK_WINDOW(user_data),
            GTK_FILE_CHOOSER_ACTION_OPEN,
            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
            GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
            NULL);
	gtk_file_chooser_set_action(GTK_FILE_CHOOSER(open),
			GTK_FILE_CHOOSER_ACTION_OPEN);

    if (gtk_dialog_run(GTK_DIALOG(open)) == GTK_RESPONSE_ACCEPT) {
        gchar *filename;

        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(open));
        conting_one_line_load_data(oneline, filename);
        g_free(filename);
    }

    gtk_widget_destroy(open);
}

static void
close_menu_activate(GtkMenuItem *menuitem,
                    gpointer user_data)
{
    gtk_widget_destroy(GTK_WIDGET(user_data));
}


static void
toolbutton_clicked(GtkToolButton *button,
                   gpointer user_data)
{
	GObject *object;

	object = g_object_new((GType) user_data,
			"one-line", oneline,
			NULL);

	conting_one_line_create(oneline, CONTING_DRAWING(object));
}

static void combo_box_changed(GtkComboBox *combo,
                              gpointer user_data)
{
	gchar *text;
	gdouble ppu;

	text = gtk_combo_box_get_active_text(combo);

	ppu = (gdouble) strtol(text, NULL, 10) / 100.0;

	g_object_set(G_OBJECT(oneline),
			"ppu", ppu,
			NULL);
}

/*
static gboolean
zoom_key_press_event(GtkWidget *entry,
                     GdkEventKey *event,
                     gpointer user_data)
{
    if (event->keyval == GDK_Return) {
        GtkTreeIter iter;
        GtkComboBox *combo_box;
        GtkListStore *list_store;
        const gchar *text;

        gboolean next;
        
        combo_box = GTK_COMBO_BOX(user_data);
        list_store = GTK_LIST_STORE(gtk_combo_box_get_model(combo_box));

        text = gtk_entry_get_text(GTK_ENTRY(entry));

        next = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(list_store),
                &iter);
        while (next) {
            gchar *entry_text;

            gtk_tree_model_get(GTK_TREE_MODEL(list_store), &iter,
                    0, &entry_text,
                    -1);

            g_print("entry_text: \"%s\"\ttext: \"%s\"\n", entry_text, text);

            if (strcmp(entry_text, text) == 0) {
                g_object_set(G_OBJECT(oneline),
                        "ppu", atoi(text),
                        NULL);

                return TRUE;
            }
            
            next = gtk_tree_model_iter_next(GTK_TREE_MODEL(list_store),
                &iter);
        }
        
        if (atoi(text) < 100)
            return TRUE;        

        gtk_list_store_prepend(list_store, &iter);
        gtk_list_store_set(list_store, &iter, 0, g_strdup(text), -1);

        gtk_combo_box_set_active(combo_box, 0);
        
        g_object_set(G_OBJECT(oneline),
                "ppu", atoi(text),
                NULL);

        return TRUE;
    }

    return FALSE;
}
static void
zoom_realize(GtkWidget *zoom,
             gpointer user_data)
{
	gtk_widget_add_events(zoom, GDK_KEY_PRESS_MASK);
}
*/
static void darea_realize(GtkWidget *widget, gpointer user_data) {
    gtk_widget_add_events(widget, GDK_EXPOSURE_MASK
            | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK
            | GDK_POINTER_MOTION_MASK | GDK_KEY_PRESS_MASK
            | GDK_KEY_RELEASE_MASK);
}
             

int
main(int argc, char *argv[]) {
    GtkWidget *window, *swindow, *darea;
    GtkWidget *vbox, *hbox;

	GtkWidget *menubar, *menu, *submenu;

    GtkWidget *toolbar, *handle;
    GtkToolItem *toolbutton;
    GtkWidget *zoom;

    GtkListStore *list_store;
    GtkTreeIter iter;

	GSList *group;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(GTK_WIDGET(window), 400, 400);
    gtk_window_set_title(GTK_WINDOW(window), "Conting One-Line Designer");
    g_signal_connect(G_OBJECT(window), "delete-event",
            G_CALLBACK(gtk_widget_destroy), NULL);
    g_signal_connect(G_OBJECT(window), "destroy",
            G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);


	menubar = gtk_menu_bar_new();
	menu = gtk_menu_item_new_with_mnemonic("_Arquivo");
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu);
	submenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu), submenu);
	menu = submenu;
	submenu = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
	gtk_label_set_text_with_mnemonic(GTK_LABEL(GTK_BIN(submenu)->child),
			"_Abrir diagrama");
    g_signal_connect(G_OBJECT(submenu), "activate",
            G_CALLBACK(open_menu_activate), window);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), submenu);
	submenu = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, NULL);
	gtk_label_set_text_with_mnemonic(GTK_LABEL(GTK_BIN(submenu)->child),
			"_Salvar diagrama");
    g_signal_connect(G_OBJECT(submenu), "activate",
            G_CALLBACK(save_menu_activate), window);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), submenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			gtk_separator_menu_item_new());
	submenu = gtk_image_menu_item_new_with_mnemonic("_Carregar dados");
	g_signal_connect(G_OBJECT(submenu), "activate",
			G_CALLBACK(load_menu_activate), window);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), submenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			gtk_separator_menu_item_new());
	submenu = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
    g_signal_connect(G_OBJECT(submenu), "activate",
            G_CALLBACK(close_menu_activate), window);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), submenu);
	/* */
	menu = gtk_menu_item_new_with_mnemonic("_Visualizar");
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu);
	submenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu), submenu);
	menu = submenu;

	group = NULL;

	submenu = gtk_radio_menu_item_new_with_mnemonic(group, "_Edit");
	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(submenu));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), submenu);
    g_signal_connect(G_OBJECT(submenu), "activate",
            G_CALLBACK(mode_menu_activate), CONTING_ONE_LINE_EDIT);

	submenu = gtk_radio_menu_item_new_with_mnemonic(group, "_View");
	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(submenu));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), submenu);
    g_signal_connect(G_OBJECT(submenu), "activate",
            G_CALLBACK(mode_menu_activate), CONTING_ONE_LINE_VIEW);
	/* */
	 
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, TRUE, 0);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);

    toolbar = gtk_toolbar_new();
    gtk_toolbar_set_icon_size(GTK_TOOLBAR(toolbar),
            GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),
            GTK_TOOLBAR_ICONS);
	toolbutton = gtk_tool_button_new(
			gtk_image_new_from_file("images/bus.png"), "Bus");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbutton, -1);
    g_signal_connect(G_OBJECT(toolbutton), "clicked",
            G_CALLBACK(toolbutton_clicked), (gpointer) CONTING_TYPE_BUS);
	toolbutton = gtk_tool_button_new(
			gtk_image_new_from_file("images/line.png"), "Line");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(toolbutton), -1);
    g_signal_connect(G_OBJECT(toolbutton), "clicked",
            G_CALLBACK(toolbutton_clicked), (gpointer) CONTING_TYPE_LINE);
	toolbutton = gtk_tool_button_new(
			gtk_image_new_from_file("images/trans2.png"), "Trans2");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(toolbutton), -1);
    g_signal_connect(G_OBJECT(toolbutton), "clicked",
            G_CALLBACK(toolbutton_clicked), (gpointer) CONTING_TYPE_TRANS2);
	toolbutton = gtk_tool_button_new(
			gtk_image_new_from_file("images/trans3.png"), "Trans3");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(toolbutton), -1);
    g_signal_connect(G_OBJECT(toolbutton), "clicked",
            G_CALLBACK(toolbutton_clicked), (gpointer) CONTING_TYPE_TRANS3);
	toolbutton = gtk_tool_button_new(
			gtk_image_new_from_file("images/gen.png"), "Generator");
	g_signal_connect(G_OBJECT(toolbutton), "clicked",
			G_CALLBACK(toolbutton_clicked), (gpointer) CONTING_TYPE_GEN);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(toolbutton), -1);
	toolbutton = gtk_tool_button_new(
			gtk_image_new_from_file("images/load.png"), "Load");
	g_signal_connect(G_OBJECT(toolbutton), "clicked",
			G_CALLBACK(toolbutton_clicked), (gpointer) CONTING_TYPE_LOAD);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(toolbutton), -1);
	toolbutton = gtk_tool_button_new_from_stock(GTK_STOCK_APPLY);
	gtk_tool_button_set_label(GTK_TOOL_BUTTON(toolbutton), "Check");
	g_signal_connect(G_OBJECT(toolbutton), "clicked",
			G_CALLBACK(check_clicked), NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbutton, -1);

    handle = gtk_handle_box_new();
    gtk_container_add(GTK_CONTAINER(handle), toolbar);
    gtk_box_pack_start(GTK_BOX(hbox), handle, TRUE, TRUE, 0);

    list_store = gtk_list_store_new(1, G_TYPE_STRING);
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, "100%", -1);
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, "120%", -1);
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, "150%", -1);
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, "200%", -1);
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, "250%", -1);
    gtk_list_store_append(list_store, &iter);
    gtk_list_store_set(list_store, &iter, 0, "300%", -1);

    zoom = gtk_combo_box_entry_new_with_model(GTK_TREE_MODEL(list_store), 0);
	gtk_editable_set_editable(GTK_EDITABLE(GTK_BIN(zoom)->child), TRUE);
	/*
    g_signal_connect(G_OBJECT(GTK_BIN(zoom)->child), "key-press-event",
            G_CALLBACK(zoom_key_press_event), zoom);
    g_signal_connect(G_OBJECT(zoom), "realize",
            G_CALLBACK(zoom_realize), NULL);
			*/
    gtk_combo_box_set_active(GTK_COMBO_BOX(zoom), 2);

    handle = gtk_handle_box_new();
    gtk_container_add(GTK_CONTAINER(handle), zoom);
    gtk_box_pack_start(GTK_BOX(hbox), handle, FALSE, FALSE, 0);

    swindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swindow),
            GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_box_pack_start(GTK_BOX(vbox), swindow, TRUE, TRUE, 0);

    darea = gtk_drawing_area_new();
    gtk_widget_set_size_request(GTK_WIDGET(darea), 1000, 1000);
    g_signal_connect(G_OBJECT(darea), "realize",
            G_CALLBACK(darea_realize), NULL);

    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(swindow), darea);

    oneline = CONTING_ONE_LINE(g_object_new(CONTING_TYPE_ONE_LINE, NULL));
    conting_one_line_set_widget(oneline, darea);

	g_signal_connect(G_OBJECT(zoom), "changed",
			G_CALLBACK(combo_box_changed), NULL);
/*
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);

    bus_button = gtk_button_new_with_label("BUS");
    gtk_box_pack_start_defaults(GTK_BOX(hbox), bus_button);

    line_button = gtk_button_new_with_label("LINE");
    gtk_box_pack_start_defaults(GTK_BOX(hbox), line_button);

    trans2_button = gtk_button_new_with_label("TRANS2");
    gtk_box_pack_start_defaults(GTK_BOX(hbox), trans2_button);
*/    
    gtk_widget_show_all(window);

    gtk_main();
    
    return 0;
}
