#include "contingconfigdialog.h"

/* PARENT CLASS POINTER */
static gpointer parent_class = NULL;

/* PROPERTIES ENUM */
enum {
	CONTING_CONFIG_DIALOG_PROP_0,
	CONTING_CONFIG_DIALOG_PROP_CONFIG,
	CONTING_CONFIG_DIALOG_PROP_EXPAND
};

/* CLASS PRIVATE DATA ACCESSOR MACRO */
#define CONTING_CONFIG_DIALOG_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_CONFIG_DIALOG, ContingConfigDialogPrivate))

/* CLASS PRIVATE DATA TYPE AND STRUCTURE */
typedef struct ContingConfigDialogPrivate_ ContingConfigDialogPrivate;
struct ContingConfigDialogPrivate_ {
	ContingConfig *config;
	
	GHashTable *name_value;

	/* APPEARANCE */
	gboolean expand_all;
};

GtkDialog *
conting_config_dialog_new(ContingConfig *config)
{
	GtkDialog *self;
	
	self = g_object_new(CONTING_TYPE_CONFIG_DIALOG, "config", config, NULL);

	gtk_dialog_add_button(self, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE);

	return self;
}

GHashTable *
conting_config_dialog_get_values(ContingConfigDialog *self)
{
    ContingConfigDialogPrivate *priv;

    g_return_val_if_fail(self != NULL && CONTING_IS_CONFIG_DIALOG(self), NULL);

    priv = CONTING_CONFIG_DIALOG_GET_PRIVATE(self);

	return priv->name_value;
}

static void
conting_config_dialog_set_config(ContingConfigDialog *self,
		ContingConfig *config)
{
    ContingConfigDialogPrivate *priv;
	const GList *n;

    g_return_if_fail(self != NULL && CONTING_IS_CONFIG_DIALOG(self));
	g_return_if_fail(config != NULL && CONTING_IS_CONFIG(config));

    priv = CONTING_CONFIG_DIALOG_GET_PRIVATE(self);

	g_hash_table_remove_all(priv->name_value);

	priv->config = config;
	g_object_ref(priv->config);

	for (n = conting_config_get_items(config); n != NULL; n = g_list_next(n)) {
		ContingConfigItem *item = n->data;
		GValue *value = g_new0(GValue, 1);

		g_value_init(value, G_PARAM_SPEC_VALUE_TYPE(item->spec));
		g_param_value_set_default(item->spec, value);

		g_hash_table_insert(priv->name_value,
				g_strdup(g_param_spec_get_name(item->spec)), value);
	}
}

static GValue *
conting_config_get_value(ContingConfigDialog *self, GParamSpec *spec)
{
    ContingConfigDialogPrivate *priv;

    g_return_val_if_fail(self != NULL && CONTING_IS_CONFIG_DIALOG(self), NULL);

    priv = CONTING_CONFIG_DIALOG_GET_PRIVATE(self);

	return g_hash_table_lookup(priv->name_value,
			g_param_spec_get_name(spec));
}

static void
spinbutton_cb(GtkSpinButton *spinbutton, gpointer user_data)    
{
	GValue *value = user_data;

	switch (G_VALUE_TYPE(value)) {
		case G_TYPE_INT:
			g_value_set_int(value,
					gtk_spin_button_get_value_as_int(spinbutton));
			break;
		case G_TYPE_UINT:
			g_value_set_uint(value,
					gtk_spin_button_get_value_as_int(spinbutton));
			break;
		case G_TYPE_FLOAT:
			g_value_set_float(value,
					gtk_spin_button_get_value(spinbutton));
			break;
		case G_TYPE_DOUBLE:
			g_value_set_double(value,
					gtk_spin_button_get_value(spinbutton));
			break;
	}
}
static GtkWidget *
conting_config_dialog_get_spin(ContingConfigDialog *self,
		gdouble value, gdouble min, gdouble max,
		gdouble incr, gint digits)
{
   	GtkAdjustment *spinner_adj;

	spinner_adj = (GtkAdjustment *) gtk_adjustment_new (value,
			min, max,
			incr, 5.0, 5.0);
   
	return gtk_spin_button_new (spinner_adj, incr, digits);
}


static GtkWidget *
conting_config_dialog_get_widget(ContingConfigDialog *self,
		ContingConfigItem *item)
{
	switch (G_PARAM_SPEC_VALUE_TYPE(item->spec)) {
		case G_TYPE_INT:
			{
				GtkWidget *spinbutton;
				GParamSpecInt *spec = (GParamSpecInt *) item->spec;
				GValue *value = conting_config_get_value(self, item->spec);

				spinbutton = conting_config_dialog_get_spin(self,
						g_value_get_int(value),
						spec->minimum, spec->maximum, 1.0, 0);
				g_signal_connect(G_OBJECT(spinbutton), "value-changed",
						G_CALLBACK(spinbutton_cb), value);
				return spinbutton;
			}
		case G_TYPE_FLOAT:
			{
				GtkWidget *spinbutton;
				GParamSpecFloat *spec = (GParamSpecFloat *) item->spec;
				GValue *value = conting_config_get_value(self, item->spec);
				
				spinbutton = conting_config_dialog_get_spin(self,
						g_value_get_float(value),
						spec->minimum, spec->maximum, 0.1, 2);
				g_signal_connect(G_OBJECT(spinbutton), "value-changed",
						G_CALLBACK(spinbutton_cb), value);
				return spinbutton;
			}
		case G_TYPE_DOUBLE:
			{
				GtkWidget *spinbutton;
				GParamSpecDouble *spec = (GParamSpecDouble *) item->spec;
				GValue *value = conting_config_get_value(self, item->spec);
				
				spinbutton = conting_config_dialog_get_spin(self,
						g_value_get_double(value),
						spec->minimum, spec->maximum, 0.0001, 8);
				g_signal_connect(G_OBJECT(spinbutton), "value-changed",
						G_CALLBACK(spinbutton_cb), value);
				return spinbutton;
			}
		default:
			return gtk_entry_new();
	}
}

static void
conting_config_dialog_create_widgets(ContingConfigDialog *self)
{
    ContingConfigDialogPrivate *priv;
	GtkWidget *expander, *label, *table;
	const GList *groups, *n, *m;
	gint row;

    g_return_if_fail(self != NULL && CONTING_IS_CONFIG_DIALOG(self));

    priv = CONTING_CONFIG_DIALOG_GET_PRIVATE(self);

	groups = conting_config_get_groups(priv->config);
	for (n = groups; n != NULL; n = g_list_next(n)) {
		ContingConfigGroup *group = n->data;
		gchar buff[256];

		sprintf(buff, "<b>%s</b>", group->name);
		expander = gtk_expander_new(buff);
		gtk_expander_set_use_markup(GTK_EXPANDER(expander), TRUE);
		gtk_expander_set_expanded(GTK_EXPANDER(expander),
				priv->expand_all || n == groups);
		gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(self)->vbox), expander);
		gtk_widget_show(expander);

		table = gtk_table_new(g_list_length(group->items), 2, FALSE);
		gtk_container_add(GTK_CONTAINER(expander), table);
		gtk_widget_show(table);

		row = 0;
		for (m = group->items; m != NULL; m = g_list_next(m)) {
			ContingConfigItem *item = m->data;
			GtkWidget *widget;

			label = gtk_label_new(g_param_spec_get_nick(item->spec));
			gtk_table_attach(GTK_TABLE(table), label, 0, 1, row, row + 1,
					0, 0, 5, 5);
			gtk_widget_show(label);
		
			widget = conting_config_dialog_get_widget(self, item);
			gtk_table_attach(GTK_TABLE(table), widget, 1, 2, row, row + 1,
					GTK_EXPAND | GTK_FILL, 0, 5, 5);
			gtk_widget_show(widget);

			row++;
		}
	}
}

static void
conting_config_dialog_finalize(GObject *self)
{
    ContingConfigDialogPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_CONFIG_DIALOG(self));

    priv = CONTING_CONFIG_DIALOG_GET_PRIVATE(self);

	if (priv->config) {
		g_object_unref(priv->config);
		priv->config = NULL;
	}

	g_hash_table_unref(priv->name_value);

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void
conting_config_dialog_get_property(GObject *self,
                             guint prop_id,
                             GValue *value,
                             GParamSpec *pspec)
{
    ContingConfigDialogPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_CONFIG_DIALOG(self));

    priv = CONTING_CONFIG_DIALOG_GET_PRIVATE(self);

    switch (prop_id) {
        case CONTING_CONFIG_DIALOG_PROP_CONFIG:
            g_value_set_object(value, priv->config);
            break;
        case CONTING_CONFIG_DIALOG_PROP_EXPAND:
            g_value_set_boolean(value, priv->expand_all);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

static void
conting_config_dialog_set_property(GObject *self,
                             guint prop_id,
                             const GValue *value,
                             GParamSpec *pspec)
{
    ContingConfigDialogPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_CONFIG_DIALOG(self));

    priv = CONTING_CONFIG_DIALOG_GET_PRIVATE(self);

    switch (prop_id) {
        case CONTING_CONFIG_DIALOG_PROP_CONFIG:
            conting_config_dialog_set_config(CONTING_CONFIG_DIALOG(self),
					g_value_get_object(value));
            break;
        case CONTING_CONFIG_DIALOG_PROP_EXPAND:
			priv->expand_all = g_value_get_boolean(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(self, prop_id, pspec);
            break;
    }
}

/* CONSTRUCTOR */
static GObject *
conting_config_dialog_constructor(GType type, guint n_construct_props,
		GObjectConstructParam *construct_param)
{
	GObject *self;
	ContingConfigDialogPrivate *priv;

	self = G_OBJECT_CLASS(parent_class)->constructor(type, n_construct_props,
			construct_param);

	priv = CONTING_CONFIG_DIALOG_GET_PRIVATE(self);
/*
	priv->prev_assoc = conting_data_get(priv->data, priv->drawing);
	*/

	conting_config_dialog_create_widgets(CONTING_CONFIG_DIALOG(self));

	return self;
}
/* INSTANCE INITIATOR */
static void
conting_config_dialog_instance_init(GTypeInstance *self, gpointer g_class)
{
    ContingConfigDialogPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_CONFIG_DIALOG(self));

    priv = CONTING_CONFIG_DIALOG_GET_PRIVATE(self);

	priv->config = NULL;
	priv->name_value = g_hash_table_new_full(g_str_hash, g_str_equal,
			g_free, g_free);

	priv->expand_all = TRUE;

	/*
    priv->data = NULL;
    priv->drawing = NULL;
	priv->prev_assoc = NULL;
	gtk_dialog_set_has_separator(GTK_DIALOG(self), FALSE);

	g_signal_connect(G_OBJECT(self), "response",
			G_CALLBACK(conting_config_dialog_response_cb), NULL);
	*/
}



/* CLASS INITIATOR */
static void
conting_config_dialog_class_init(gpointer g_class, gpointer class_data)
{
    GObjectClass *gobject_class;

    gobject_class = G_OBJECT_CLASS(g_class);
	gobject_class->constructor = conting_config_dialog_constructor;
	gobject_class->finalize = conting_config_dialog_finalize;
	gobject_class->get_property = conting_config_dialog_get_property;
	gobject_class->set_property = conting_config_dialog_set_property;


    g_object_class_install_property(gobject_class,
            CONTING_CONFIG_DIALOG_PROP_CONFIG,
            g_param_spec_object("config",
                                "Config Object",
                                "Configuration properties especification",
                                CONTING_TYPE_CONFIG,
                                G_PARAM_READABLE | G_PARAM_CONSTRUCT_ONLY
                                | G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class,
            CONTING_CONFIG_DIALOG_PROP_EXPAND,
            g_param_spec_boolean("expand",
                                "Expand all groups, or just the first",
                                "Expand all groups, or just the first",
                                TRUE,
                                G_PARAM_WRITABLE | G_PARAM_READABLE));


	parent_class = g_type_class_peek_parent(g_class);

    g_type_class_add_private(g_class, sizeof(ContingConfigDialogPrivate));
}


GType
conting_config_dialog_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingConfigDialogClass),
            NULL,
            NULL,
            conting_config_dialog_class_init,
            NULL,
            NULL,
            sizeof(ContingConfigDialog),
            0,
            conting_config_dialog_instance_init,
            NULL
        };

        type = g_type_register_static(GTK_TYPE_DIALOG,
                "ContingConfigDialog",
                &type_info, 0);

    }

    return type;
}
/*
static void
response_cb(GtkDialog *dialog, gint arg1, gpointer user_data)
{
	const GHashTable *hash = conting_config_dialog_get_values(dialog);
	g_hash_table_ref(hash);
	gtk_widget_destroy(GTK_WIDGET(dialog));

	g_print("controle = %d\n", g_value_get_int(
				g_hash_table_lookup(hash, "controle")));
}

static void
button_clicked(GtkButton *button, gpointer user_data)
{
	GtkDialog *dialog;
	ContingConfig *config = g_object_new(CONTING_TYPE_CONFIG, NULL);
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
	g_signal_connect(G_OBJECT(dialog), "response",
			G_CALLBACK(response_cb), NULL);


	gtk_dialog_run(dialog);
}

int
main(int argc, char *argv[])
{
	GtkWidget *window, *button;
	ContingConfig *config;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	button = gtk_button_new_with_label("click me");

	gtk_container_add(GTK_CONTAINER(window), button);
	g_signal_connect(G_OBJECT(button), "clicked",
			G_CALLBACK(button_clicked), NULL);

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
*/
