#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "contingoneline.h"
#include "contingbus.h"
#include "contingline.h"
#include "contingtrans2.h"
#include "contingtrans3.h"
#include "continggen.h"
#include "contingload.h"

#include "contingmodel.h"


#include "contingsysteminfo.h"

static ContingOneLine *oneline;
static GtkWidget *window;

static GtkWidget *menu_modes[CONTING_ONE_LINE_MODE_NUMBER];


/* SIGNAL CALLBACKS */
static void
toolbutton_clicked(GtkToolButton *button,
                   gpointer user_data);
static void
check_clicked(GtkToolButton *button,
                   gpointer user_data);

static void
show_name_clicked(GtkToolButton *button, gpointer user_data);

static void
show_number_clicked(GtkToolButton *button, gpointer user_data);

static void
show_voltage_clicked(GtkToolButton *button, gpointer user_data);

static void
hide_gen_clicked(GtkToolButton *button, gpointer user_data);

static void
hide_load_clicked(GtkToolButton *button, gpointer user_data);

static void
run_load_flow_clicked(GtkToolButton *button, gpointer user_data);

static void
oneline_mode_changed(ContingOneLine *oneline, ContingOneLineMode mode,
		gpointer user_data)
{
	puts("oneline_mode_changed");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_modes[mode]), 
			TRUE);
}

/**
 * Creates the toolbar for editing mode.
 * Includes:
 * - create components
 * - check data
 */
/* PRIVATE FUNCTION */
static GtkWidget *
conting_main_get_edit_toolbar(void)
{
    static GtkWidget *toolbar = NULL;
    GtkToolItem *toolbutton;

    if (toolbar != NULL)
        return toolbar;

    toolbar = gtk_toolbar_new();
    
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
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(toolbutton), -1);
	g_signal_connect(G_OBJECT(toolbutton), "clicked",
			G_CALLBACK(toolbutton_clicked), (gpointer) CONTING_TYPE_GEN);
    
	toolbutton = gtk_tool_button_new(
			gtk_image_new_from_file("images/load.png"), "Load");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), GTK_TOOL_ITEM(toolbutton), -1);
	g_signal_connect(G_OBJECT(toolbutton), "clicked",
			G_CALLBACK(toolbutton_clicked), (gpointer) CONTING_TYPE_LOAD);

    gtk_toolbar_insert(GTK_TOOLBAR(toolbar),
            gtk_separator_tool_item_new(), -1);
    
	toolbutton = gtk_tool_button_new_from_stock(GTK_STOCK_APPLY);
	gtk_tool_button_set_label(GTK_TOOL_BUTTON(toolbutton), "Check");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbutton, -1);
	g_signal_connect(G_OBJECT(toolbutton), "clicked",
			G_CALLBACK(check_clicked), NULL);

    g_object_ref(toolbar);
    return toolbar;
}

/**
 * Creates the toolbar for viewing mode.
 *
 * Includes:
 * - show labels/numbers
 */
/* PRIVATE FUNCTION */
static GtkWidget *
conting_main_get_view_toolbar(void)
{
    static GtkWidget *toolbar = NULL;
    GtkToolItem *toolbutton;

    if (toolbar != NULL)
        return toolbar;

    toolbar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),
            GTK_TOOLBAR_ICONS);

    
	toolbutton = gtk_tool_button_new_from_stock(GTK_STOCK_BOLD);
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(toolbutton), "Mostrar nomes");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbutton, -1);
    g_signal_connect(G_OBJECT(toolbutton), "clicked",
            G_CALLBACK(show_name_clicked), NULL);
    
	toolbutton = gtk_tool_button_new(
			gtk_image_new_from_file("images/number.png"), "Mostrar n�meros");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbutton, -1);
    g_signal_connect(G_OBJECT(toolbutton), "clicked",
            G_CALLBACK(show_number_clicked), NULL);

	/*
	toolbutton = gtk_tool_button_new(
			gtk_image_new_from_file("images/number.png"), "Show numbers");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbutton, -1);
    g_signal_connect(G_OBJECT(toolbutton), "clicked",
            G_CALLBACK(show_voltage_clicked), NULL);
			*/

	toolbutton = gtk_tool_button_new(
			gtk_image_new_from_file("images/gen.png"), "Esconder geradores");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbutton, -1);
    g_signal_connect(G_OBJECT(toolbutton), "clicked",
            G_CALLBACK(hide_gen_clicked), NULL);

	toolbutton = gtk_tool_button_new(
			gtk_image_new_from_file("images/load.png"), "Esconder cargas");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbutton, -1);
    g_signal_connect(G_OBJECT(toolbutton), "clicked",
            G_CALLBACK(hide_load_clicked), NULL);
    
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),
            gtk_separator_tool_item_new(), -1);

	toolbutton = gtk_tool_button_new(
			gtk_image_new_from_file("images/run.png"), "Executar LoadFlow");
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbutton, -1);
    g_signal_connect(G_OBJECT(toolbutton), "clicked",
            G_CALLBACK(run_load_flow_clicked), NULL);


    g_object_ref(toolbar);
    return toolbar;
}

/* SIGNAL CALLBACK */
static void
conting_main_change_mode(ContingOneLine *ol, ContingOneLineMode mode,
        gpointer user_data)
{
    GtkWidget *handle = user_data;
    GtkWidget *old_child, *child;


    old_child = GTK_BIN(handle)->child;

    switch (mode) {
        case CONTING_ONE_LINE_VIEW:
            child = conting_main_get_view_toolbar();
            break;
        case CONTING_ONE_LINE_EDIT:
            child = conting_main_get_edit_toolbar();
            break;
        default:
            return;
    }

    gtk_widget_hide(old_child);
    gtk_container_remove(GTK_CONTAINER(handle), old_child);

    gtk_container_add(GTK_CONTAINER(handle), child);
    gtk_widget_show(child);
    gtk_widget_show_all(handle);
}


/**
 * Creates a TreeView containing the list of errors specified in errors.
 */
/* PRIVATE FUNCTION */
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

/* PRIVATE FUNCTION */
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

/* PRIVATE FUNCTION */
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

/**
 * Checks the data associations of the oneline.
 */
/* PRIVATE FUNCTION */
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

/* SIGNAL CALLBACK */
static void
mode_menu_activate(GtkMenuItem *menuitem,
		gpointer user_data)
{
	GList *errors = NULL;
    ContingOneLineMode mode = (ContingOneLineMode) user_data;

    g_print("mode = %d\n", mode);

    /* TODO: instead of calling check_data in case of CONTING_ONE_LINE_VIEW
     * and showing a list of errors, just check if the mode could be
     * changed using conting_one_line_set_mode() return value and display
     * an error message if not possible */
    if (mode == CONTING_ONE_LINE_VIEW) {
        if (!check_data(&errors)) {
            show_error_dialog(errors);
        	g_list_foreach(errors, (GFunc) conting_error_free, NULL);
		    g_list_free(errors);
            return;
      	}
    }

    conting_one_line_set_mode(oneline, mode);
}

/* SIGNAL CALLBACK */
static void
launch_xpvm_activate(GtkMenuItem *menuitem,
		gpointer user_data)
{
}

/* SIGNAL CALLBACK */
static void
check_clicked(GtkToolButton *menuitem,
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

/** Sets default drawingoperation to hide the generators */
/* SIGNAL CALLBACK */
static void
hide_gen_clicked(GtkToolButton *button, gpointer user_data)
{
	ContingDrawingOperationDefault *default_opr;
	static gboolean show = FALSE;

	/* TODO: this is a stub, should be completely refactored.
	 * It knows too much about other objects internals */
	default_opr = conting_one_line_get_default_operation(oneline);

	if (show) {
		show = FALSE;
		conting_drawing_operation_default_add_mask(default_opr,
				CONTING_DRAWING_OPERATION_DEFAULT_GEN);
	} else {
		show = TRUE;
		conting_drawing_operation_default_remove_mask(default_opr,
				CONTING_DRAWING_OPERATION_DEFAULT_GEN);
	}

	/* FIXME: shouldn't do this. Instead, conting_one_line_operation_update()
	 * should be private to ContingOneLine, and defualt_opr would signal it
	 * when it changes. */
	conting_one_line_operation_update(oneline, default_opr);
}
/* SIGNAL CALLBACK */
static void
hide_load_clicked(GtkToolButton *button, gpointer user_data)
{
	ContingDrawingOperationDefault *default_opr;
	static gboolean show = FALSE;

	/* TODO: this is a stub, should be completely refactored.
	 * It knows too much about other objects internals */
	default_opr = conting_one_line_get_default_operation(oneline);

	if (show) {
		show = FALSE;
		conting_drawing_operation_default_add_mask(default_opr,
				CONTING_DRAWING_OPERATION_DEFAULT_LOAD);
	} else {
		show = TRUE;
		conting_drawing_operation_default_remove_mask(default_opr,
				CONTING_DRAWING_OPERATION_DEFAULT_LOAD);
	}

	/* FIXME: shouldn't do this. Instead, conting_one_line_operation_update()
	 * should be private to ContingOneLine, and defualt_opr would signal it
	 * when it changes. */
	conting_one_line_operation_update(oneline, default_opr);
}

#include "contingconfigdialog.h"
#include "contingconfig.h"
#include "contingloadflow.h"

/* SIGNAL CALLBACK */
static void
run_load_flow_clicked(GtkToolButton *button, gpointer user_data)
{
	GtkDialog *dialog;
	ContingConfig *config;
	GHashTable *values;
	ContingLoadFlowConfig *cfg;

	config = g_object_new(CONTING_TYPE_CONFIG, NULL);
	conting_config_new_group(config, "Convergencia");
		conting_config_add_item(config,
				g_param_spec_double("p-theta",
					"Convergencia iterações P-teta",
					"Tolerancia para convergencia iterações P-teta",
					0, 1, 0.0000100,
					G_PARAM_READABLE | G_PARAM_WRITABLE));
		conting_config_add_item(config,
				g_param_spec_double("q-v",
					"Convergencia iterações Q-V",
					"Tolerancia para convergencia iterações Q-V",
					0, 1, 0.0000100,
					G_PARAM_READABLE | G_PARAM_WRITABLE));
		conting_config_add_item(config,
				g_param_spec_double("ltc",
					"Incremento para ajuste de taps de LTCs",
					"Incremento para ajuste de taps de LTCs",
					0, 1, 0.0100000,
					G_PARAM_READABLE | G_PARAM_WRITABLE));
		conting_config_add_item(config,
				g_param_spec_double("violation",
					"Margem para violação de limites de tensão",
					"Margem para violação de limites de tensão",
					0, 1, 0.0500000,
					G_PARAM_READABLE | G_PARAM_WRITABLE));
		conting_config_add_item(config,
				g_param_spec_double("factor",
					"Fator multiplicativo para correção de tensão",
					"Fator multiplicativo para correção de tensão"
				    "pelo ajuste do tap",
					0, 1, 0.9900000,
					G_PARAM_READABLE | G_PARAM_WRITABLE));
	conting_config_new_group(config, "Iterações");
		conting_config_add_item(config,
				g_param_spec_int("reativa",
					"Iterações reativas antes de atuar controles",
					"Número de iterações reativas antes de atuar controles",
					0, 100, 2,
					G_PARAM_READABLE | G_PARAM_WRITABLE));
		conting_config_add_item(config,
				g_param_spec_int("controle",
					"Iterações na rotina de controle",
					"Limite máximo de iterações na rotina controle",
					0, 100, 10,
					G_PARAM_READABLE | G_PARAM_WRITABLE));
   
	dialog = conting_config_dialog_new(config);
	gtk_dialog_run(dialog);


	values = conting_config_dialog_get_values(CONTING_CONFIG_DIALOG(dialog));
	g_hash_table_ref(values);

	gtk_widget_destroy(GTK_WIDGET(dialog));
	/* HELPER MACROS */
#define values_get(h, n, t) g_value_get_ ##t(g_hash_table_lookup(h, n))
#define cfg_set(c, t, v) conting_load_flow_config_add_ ##t(c, v)
#define cfg_set_values(c, h, t, n) cfg_set(c, t, values_get(h, n, t))

	cfg = conting_load_flow_config_new();
	cfg_set_values(cfg, values, double, "p-theta");
	cfg_set_values(cfg, values, double, "q-v");
	cfg_set_values(cfg, values, double, "ltc");
	cfg_set_values(cfg, values, double, "violation");
	cfg_set_values(cfg, values, double, "factor");
    conting_load_flow_config_end_section(cfg);

	cfg_set_values(cfg, values, int, "reativa");
	cfg_set_values(cfg, values, int, "controle");
    conting_load_flow_config_end_section(cfg);

	g_hash_table_unref(values);

	conting_string_file_store(CONTING_LOAD_FLOW_CONFIG_PATH,
			conting_load_flow_config_get_text(cfg));
	g_object_unref(cfg);


	conting_one_line_load_flow_run_sync(oneline);
}

/**
 * Returns the string to be used as a label to drawing.
 */
/* CALLBACK FUNCTION */
static const gchar *
name_label(ContingDrawingOperation *self, ContingDrawing *drawing,
        gpointer user_data)
{
    ContingData *data;
    ContingItemData *item_data;
    const gchar *name = NULL;

    assert(oneline == conting_drawing_get_one_line(drawing));

	/* TODO: is it good? is it safe? */
	if (!CONTING_IS_BUS_BASE(drawing))
		return "";

    g_object_get(oneline, "data", &data, NULL);

    item_data = conting_data_get(data, drawing);
	if (item_data == NULL)
		return "";

    conting_item_data_get_attr(item_data, "name", &name, NULL);
	if (name == NULL)
		return "";

    return name;
}

/* SIGNAL CALLBACK */
static void
show_name_clicked(GtkToolButton *button, gpointer user_data)
{
    static gboolean showing = FALSE;
    static ContingDrawingOperation *opr = NULL;

    if (opr == NULL) {
        opr = g_object_new(CONTING_TYPE_DRAWING_OPERATION_LABEL,
				"label-func", name_label,
				"place", CONTING_DRAWING_OPERATION_LABEL_BOTTOM,
				NULL);
		/* conting_drawing_operation_label_new_with_func(name_label, NULL);*/
    }

    if (showing) {
        showing = FALSE;
        conting_one_line_remove_operation(oneline, opr);
    } else {
        showing = TRUE;
        conting_one_line_add_operation(oneline, opr);
    }
}

/* TODO: almost the same thing of name_label, refactor it. */
/* CALLBACK FUNCTION */
static const gchar *
number_label(ContingDrawingOperation *self, ContingDrawing *drawing,
        gpointer user_data)
{
    ContingData *data;
    ContingItemData *item_data;
    static gchar name[256];
    gint number = 0;

    assert(oneline == conting_drawing_get_one_line(drawing));

	/* TODO: is it good? is it safe? */
	if (!CONTING_IS_BUS_BASE(drawing))
		return "";

    g_object_get(oneline, "data", &data, NULL);

    item_data = conting_data_get(data, drawing);

    if (item_data == NULL)
        return "";

    conting_item_data_get_attr(item_data, "number", &number, NULL);
    if (number == 0)
        return "";

    sprintf(name, "%d", number);

    return name;
}

static void
show_number(gboolean showing)
{
	static ContingDrawingOperation *opr = NULL;

	if (opr == NULL) {
		opr = g_object_new(CONTING_TYPE_DRAWING_OPERATION_LABEL,
				"label-func", number_label,
				"place", CONTING_DRAWING_OPERATION_LABEL_TOP,
				"color-name", "purple",
				NULL);
	}

    if (showing) {
        conting_one_line_add_operation(oneline, opr);
    } else {
        conting_one_line_remove_operation(oneline, opr);
    }
}
static void
show_voltage(gboolean showing);
/* SIGNAL CALLBACK */
static void
show_number_clicked(GtkToolButton *button, gpointer user_data)
{
    static gboolean showing = FALSE;

	showing = !showing;

	if (showing)
		show_voltage(FALSE);

	show_number(showing);
}

/* TODO: almost the same thing of name_label, refactor it. */
/* CALLBACK FUNCTION */
static const gchar *
voltage_label(ContingDrawingOperation *self, ContingDrawing *drawing,
        gpointer user_data)
{
    ContingData *data;

	ContingModel *model;
    ContingItemData *item_data, *flow_data;
    gint number = 0;
	gdouble voltage, cur_voltage, cur_angle;
	static gchar name[256];

    assert(oneline == conting_drawing_get_one_line(drawing));

	/* TODO: is it good? is it safe? */
	if (!CONTING_IS_BUS_BASE(drawing))
		return "";

    g_object_get(oneline, "data", &data, NULL);

    item_data = conting_data_get(data, drawing);

    if (item_data == NULL)
        return "";
	
    conting_item_data_get_attr(item_data, "number", &number, NULL);
    if (number == 0)
        return "";

	conting_item_data_get_attr(item_data, "voltage", &voltage, NULL);

	model = conting_one_line_load_flow_model(oneline);

	flow_data = conting_model_get_item(model, CONTING_ITEM_TYPE_FLOW_BUS,
			GINT_TO_POINTER(number));

	if (flow_data == NULL)
		return "";

	conting_item_data_get_attr(flow_data,
			"voltage", &cur_voltage,
			"angle", &cur_angle,
			NULL);

    sprintf(name, "%.1lfkV %.1lf", voltage * cur_voltage, cur_angle);

    return name;
}

static void
show_voltage(gboolean showing)
{
	static ContingDrawingOperation *opr = NULL;

	if (opr == NULL) {
		opr = g_object_new(CONTING_TYPE_DRAWING_OPERATION_LABEL,
				"label-func", voltage_label,
				"place", CONTING_DRAWING_OPERATION_LABEL_TOP,
				"color-name", "green",
				NULL);
	}

    if (showing) {
        conting_one_line_add_operation(oneline, opr);
    } else {
        conting_one_line_remove_operation(oneline, opr);
    }
}
/* SIGNAL CALLBACK */
static void
show_voltage_clicked(GtkToolButton *button, gpointer user_data)
{
    static gboolean showing = FALSE;

	showing = !showing;
	if (showing)
		show_number(FALSE);

	show_voltage(showing);
}

/* SIGNAL CALLBACK */
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

/* SIGNAL CALLBACK */
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

/*SIGNAL CALLBACK */
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

/* SIGNAL CALLBACK */
static void
close_menu_activate(GtkMenuItem *menuitem,
                    gpointer user_data)
{
    gtk_widget_destroy(GTK_WIDGET(user_data));
}


/**
 * Creates components.
 */
/* SIGNAL CALLBACK */
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

/* SIGNAL CALLBACK */
static void
system_info_clicked(GtkToolButton *button,
		gpointer user_data)
{
	ContingData *data;
	GtkDialog *dialog;
	
	g_object_get(oneline,
			"data", &data,
			NULL);
			
	dialog = conting_system_info_get_dialog(conting_data_get_model(data));
	
	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));
	gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
	
	gtk_dialog_run(dialog);	
}			


/**
 * Sets zoom.
 */
/* SIGNAL CALLBACK */
static void
combo_box_changed(GtkComboBox *combo,
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

/**
 * Set widget event mask.
 */
/* SIGNAL CALLBACK */
static void
darea_realize(GtkWidget *widget, gpointer user_data) {
    gtk_widget_add_events(widget, GDK_EXPOSURE_MASK
            | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK
            | GDK_POINTER_MOTION_MASK | GDK_KEY_PRESS_MASK
            | GDK_KEY_RELEASE_MASK);
}
             

/* APPLICATION ENTRY POINT */
/* MAIN FUNCTION */
int
main(int argc, char *argv[]) {
    GtkWidget *swindow, *darea;
    GtkWidget *vbox, *hbox;

	GtkWidget *menubar, *menu, *submenu;

    GtkWidget *toolbar, *handle;
    GtkToolItem *toolbutton;
    GtkWidget *zoom;

    GtkListStore *list_store;
    GtkTreeIter iter;

	GSList *group;

    gtk_init(&argc, &argv);


    oneline = CONTING_ONE_LINE(g_object_new(CONTING_TYPE_ONE_LINE, NULL));
	g_signal_connect(G_OBJECT(oneline), "change-mode",
			G_CALLBACK(oneline_mode_changed), NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request(GTK_WIDGET(window), 400, 400);
    gtk_window_set_title(GTK_WINDOW(window), "Conting One-Line");
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

	submenu = gtk_radio_menu_item_new_with_mnemonic(group, "_Editar");
	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(submenu));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), submenu);
    g_signal_connect(G_OBJECT(submenu), "activate",
            G_CALLBACK(mode_menu_activate), (gpointer) CONTING_ONE_LINE_EDIT);
	menu_modes[CONTING_ONE_LINE_EDIT] = submenu;

	submenu = gtk_radio_menu_item_new_with_mnemonic(group, "_Caso base");
	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(submenu));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), submenu);
    g_signal_connect(G_OBJECT(submenu), "activate",
            G_CALLBACK(mode_menu_activate), (gpointer) CONTING_ONE_LINE_VIEW);
	menu_modes[CONTING_ONE_LINE_VIEW] = submenu;

	/* */
	menu = gtk_menu_item_new_with_mnemonic("_Ferramentas");
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu);
	submenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu), submenu);
	menu = submenu;
	submenu = gtk_image_menu_item_new_with_mnemonic("_Executar XPVM");
    g_signal_connect(G_OBJECT(submenu), "activate",
            G_CALLBACK(launch_xpvm_activate), window);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), submenu);

	 
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, TRUE, 0);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);

	/* Perm toolbar */
	toolbar = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),
            GTK_TOOLBAR_ICONS);
    gtk_toolbar_set_show_arrow(GTK_TOOLBAR(toolbar), FALSE);

	handle = gtk_handle_box_new();
	gtk_container_add(GTK_CONTAINER(handle), toolbar);
	
    toolbutton = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolbutton, -1);
    g_signal_connect(G_OBJECT(toolbutton), "clicked",
    		G_CALLBACK(system_info_clicked), NULL);
    
    gtk_box_pack_start(GTK_BOX(hbox), handle, FALSE, FALSE, 0);
    

    toolbar = conting_main_get_edit_toolbar();

    handle = gtk_handle_box_new();
    gtk_container_add(GTK_CONTAINER(handle), toolbar);
    gtk_box_pack_start(GTK_BOX(hbox), handle, TRUE, TRUE, 0);
    g_signal_connect(G_OBJECT(oneline), "change-mode",
            G_CALLBACK(conting_main_change_mode), handle);

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
