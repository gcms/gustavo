#ifndef CONTING_FILE_H
#define CONTING_FILE_H

#include <glib.h>

typedef struct {
	gint number;
	gchar name[12];
	gint load_flow_area;
	gint loss_zone;
	gint type;
	gdouble final_voltage;
	gdouble final_angle;
	gdouble load_mw;
	gdouble load_mvar;
	gdouble gen_mw;
	gdouble gen_mvar;
	gdouble base_kv;
	gdouble desired_voltage;
	gdouble max_mvar;
	gdouble min_mvar;
	gdouble shunt_conductance;
	gdouble shunt_susceptance;
	gint remote_ctrld_bus_number;
} bus_data_t;

typedef struct {
	gint tap_bus_number;
	gint z_bus_number;
	gint load_flow_area;
	gint loss_zone;
	gint circuit;
	gint type;
	gdouble branch_resistance;
	gdouble branch_reactance;
	gdouble line_charging;
	gint line_mva_1;
	gint line_mva_2;
	gint line_mva_3;
	gint control_bus_number;
	gint side;
	gdouble trans_final_ratio;
	gdouble trans_final_angle;
	gdouble min_tap_phase_shift;
	gdouble max_tap_phase_shift;
	gdouble step_size;
	gdouble min_voltage;
	gdouble max_voltage;
} branch_data_t;

gchar *
conting_file_string(const gchar *line,
		guint start, guint end);

gdouble
conting_file_float(const gchar *line,
		guint start, guint end);

gint
conting_file_int(const gchar *line,
		guint start, guint end);

void
conting_file_bus_data(bus_data_t *self, const gchar *line);

void
conting_file_branch_data(branch_data_t *self, const gchar *line);

#endif /* CONTING_FILE_H */
