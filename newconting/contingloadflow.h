#ifndef CONTING_LOAD_FLOW_H
#define CONTING_LOAD_FLOW_H

#include <glib.h>
#include "contingsettings.h"
#include "contingstringpool.h"

#define CONTING_LOAD_FLOW_BASEDIR \
	conting_settings_get_value_default("load flow", "basedir", "FluxoCA")
#define CONTING_LOAD_FLOW_INPUT \
	conting_settings_get_value_default("load flow", "input", "dad.txt")
#define CONTING_LOAD_FLOW_CONFIG_F \
	conting_settings_get_value_default("load flow", "config", "valores.txt")
#define CONTING_LOAD_FLOW_MAIN \
	conting_settings_get_value_default("load flow", "main", "celg25112006")
#define CONTING_LOAD_FLOW_RELAT1 \
	conting_settings_get_value_default("load flow", "relat1", "relat1.txt")
#define CONTING_LOAD_FLOW_RELAT2 \
	conting_settings_get_value_default("load flow", "relat2", "relat2.txt")

#define CONTING_LOAD_FLOW_BASEDIR_PATH conting_string_pool_add(				   \
		g_build_filename(conting_string_pool_add(g_get_current_dir()),		   \
			CONTING_LOAD_FLOW_BASEDIR, NULL))
#define CONTING_LOAD_FLOW_INPUT_PATH conting_string_pool_add(                  \
		g_build_filename(CONTING_LOAD_FLOW_BASEDIR, CONTING_LOAD_FLOW_INPUT,   \
			NULL))
#define CONTING_LOAD_FLOW_CONFIG_PATH conting_string_pool_add(                 \
		g_build_filename(CONTING_LOAD_FLOW_BASEDIR, CONTING_LOAD_FLOW_CONFIG_F,\
			NULL))
#define CONTING_LOAD_FLOW_MAIN_PATH conting_string_pool_add(                  \
		g_build_filename(conting_string_pool_add(g_get_current_dir()),        \
			CONTING_LOAD_FLOW_BASEDIR, CONTING_LOAD_FLOW_MAIN, NULL))
#define CONTING_LOAD_FLOW_RELAT1_PATH conting_string_pool_add(                \
		g_build_filename(CONTING_LOAD_FLOW_BASEDIR, CONTING_LOAD_FLOW_RELAT1, \
			NULL))
#define CONTING_LOAD_FLOW_RELAT2_PATH conting_string_pool_add(                \
		g_build_filename(CONTING_LOAD_FLOW_BASEDIR, CONTING_LOAD_FLOW_RELAT2, \
			NULL))


#include <glib-object.h>
#include <glib.h>

#define CONTING_TYPE_LOAD_FLOW_CONFIG     (conting_load_flow_config_get_type())
#define CONTING_LOAD_FLOW_CONFIG(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_LOAD_FLOW_CONFIG, ContingLoadFlowConfig))
#define CONTING_LOAD_FLOW_CONFIG_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_LOAD_FLOW_CONFIG, ContingLoadFlowConfigClass))
#define CONTING_IS_LOAD_FLOW_CONFIG(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_LOAD_FLOW_CONFIG))
#define CONTING_IS_LOAD_FLOW_CONFIG_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_LOAD_FLOW_CONFIG))
#define CONTING_LOAD_FLOW_CONFIG_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_LOAD_FLOW_CONFIG, ContingLoadFlowConfigClass))

typedef struct ContingLoadFlowConfig_ ContingLoadFlowConfig;
typedef struct ContingLoadFlowConfigClass_ ContingLoadFlowConfigClass;


struct ContingLoadFlowConfig_
{
	GObject parent;
};


struct ContingLoadFlowConfigClass_
{
	GObjectClass parent;
};
GType
conting_load_flow_config_get_type(void);
void
conting_load_flow_config_end_section(ContingLoadFlowConfig *self);
void
conting_load_flow_config_add_int(ContingLoadFlowConfig *self, gint v);
void
conting_load_flow_config_add_double(ContingLoadFlowConfig *self, gdouble f);
#define conting_load_flow_config_add_float conting_load_flow_config_add_double

const gchar *
conting_load_flow_config_get_text(ContingLoadFlowConfig *self);
#define conting_load_flow_config_new() CONTING_LOAD_FLOW_CONFIG(g_object_new( \
			CONTING_TYPE_LOAD_FLOW_CONFIG, NULL));

#endif /* CONTING_LOAD_FLOW_H */
