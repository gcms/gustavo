#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdlib.h>
#include <string.h>
#include "contingoneline.h"
#include "contingbus.h"
#include "contingline.h"
#include "contingtrans2.h"

static ContingOneLine *oneline;

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
             

int main(int argc, char *argv[]) {
    GtkWidget *window, *swindow, *darea;
    GtkWidget *vbox, *hbox;

	GtkWidget *menubar, *menu, *submenu;

    GtkWidget *toolbar, *handle;
    GtkToolItem *toolbutton;
    GtkWidget *zoom;

    GtkListStore *list_store;
    GtkTreeIter iter;

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
	menu = gtk_menu_item_new_with_label("Arquivo");
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu);
	submenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu), submenu);
	menu = submenu;
	submenu = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), submenu);
	submenu = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), submenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu),
			gtk_separator_menu_item_new());
	submenu = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), submenu);
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
    toolbutton = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
	toolbutton = gtk_tool_button_new(
			gtk_image_new_from_file("images/trans3.png"), "Trans3");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(toolbutton), -1);

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
