#include "contingfile.h"
#include "contingitemdata.h"

#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

gchar *
conting_file_string(const gchar *line,
		guint start, guint end)
{
	gchar *result;

	result = malloc((end - start + 2) * sizeof(gchar));
	assert(result);
	strncpy(result, line + start, end - start + 1);
	result[end - start + 1] = 0;

	return result;
}

gdouble
conting_file_float(const gchar *line,
		guint start, guint end)
{
	gdouble result;
	gchar *string = conting_file_string(line, start, end);

	sscanf(string, "%lf", &result);

	free(string);

	return result;
}

gint
conting_file_int(const gchar *line,
		guint start, guint end)
{
	gint result;
	gchar *string = conting_file_string(line, start, end);

	sscanf(string, "%d", &result);

	free(string);

	return result;
}

void
conting_file_bus_data(bus_data_t *self, const gchar *line)
{
	gchar *name;

	self->number = conting_file_int(line, 0, 3);

	name = conting_file_string(line, 5, 16);
	strcpy(self->name, name);
	free(name);

	self->load_flow_area = conting_file_int(line, 18, 19);
	self->loss_zone = conting_file_int(line, 20, 22);
	self->type = conting_file_int(line, 24, 25);
	self->final_voltage = conting_file_float(line, 27, 32);
	self->final_angle = conting_file_float(line, 33, 39);
	self->load_mw = conting_file_float(line, 40, 48);
	self->load_mvar = conting_file_float(line, 49, 58);
	self->gen_mw = conting_file_float(line, 59, 66);
	self->gen_mvar = conting_file_float(line, 67, 74);
	self->base_kv = conting_file_float(line, 76, 82);
	self->desired_voltage = conting_file_float(line, 84, 89);
	self->max_mvar = conting_file_float(line, 90, 97);
	self->min_mvar = conting_file_float(line, 98, 105);
	self->shunt_conductance = conting_file_float(line, 106, 113);
	self->shunt_susceptance = conting_file_float(line, 114, 121);
	self->remote_ctrld_bus_number = conting_file_int(line, 123, 126);
}

void
conting_file_branch_data(branch_data_t *self, const gchar *line)
{
	self->tap_bus_number = conting_file_int(line, 0, 3);
	self->z_bus_number = conting_file_int(line, 5, 8);
	self->load_flow_area = conting_file_int(line, 10, 11);
	self->loss_zone = conting_file_int(line, 13, 14);
	self->circuit = conting_file_int(line, 16, 16);
	self->type = conting_file_int(line, 18, 18);
	self->branch_resistance = conting_file_float(line, 20, 29);
	self->branch_reactance = conting_file_float(line, 31, 41);
	self->line_charging = conting_file_float(line, 43, 48);
	self->line_mva_1 = conting_file_float(line, 50, 54);
	self->line_mva_2 = conting_file_float(line, 56, 60);
	self->line_mva_3 = conting_file_float(line, 62, 66);
	self->control_bus_number = conting_file_int(line, 68, 71);
	self->side = conting_file_int(line, 73, 73);
	self->trans_final_ratio = conting_file_float(line, 76, 81);
	self->trans_final_angle = conting_file_float(line, 83, 89);
	self->min_tap_phase_shift = conting_file_float(line, 90, 96);
	self->max_tap_phase_shift = conting_file_float(line, 97, 103);
	self->step_size = conting_file_float(line, 105, 110);
	self->min_voltage = conting_file_float(line, 112, 118);
	self->max_voltage = conting_file_float(line, 119, 125);
}

static ContingItemData *
conting_file_read_bus(const char *line)
{
	GObject *item_data;
	bus_data_t bus_data;

	item_data = g_object_new(CONTING_TYPE_ITEM_DATA,
			"type", CONTING_ITEM_TYPE_BUS,
			NULL);

	assert(item_data);

	conting_file_bus_data(&bus_data, line);

	conting_item_data_set_attr(CONTING_ITEM_DATA(item_data),
			"number", G_TYPE_INT, bus_data.number,
			"name", G_TYPE_STRING, bus_data.name,
			"load flow area", G_TYPE_INT, bus_data.load_flow_area,
			"loss zone", G_TYPE_INT, bus_data.loss_zone,
			"type", G_TYPE_INT, bus_data.type,
			"final voltage", G_TYPE_DOUBLE, bus_data.final_voltage,
			"final angle", G_TYPE_DOUBLE, bus_data.final_angle,
			"load mw", G_TYPE_DOUBLE, bus_data.load_mw,
			"load mvar", G_TYPE_DOUBLE, bus_data.load_mvar,
			"gen mw", G_TYPE_DOUBLE, bus_data.gen_mw,
			"gen mvar", G_TYPE_DOUBLE, bus_data.gen_mvar,
			"base kv", G_TYPE_DOUBLE, bus_data.base_kv,
			"desired voltage", G_TYPE_DOUBLE, bus_data.desired_voltage,
			"max mvar", G_TYPE_DOUBLE, bus_data.max_mvar,
			"min mvar", G_TYPE_DOUBLE, bus_data.min_mvar,
			"shunt conductance", G_TYPE_DOUBLE, bus_data.shunt_conductance,
			"shunt susceptance", G_TYPE_DOUBLE, bus_data.shunt_susceptance,
			"remote ctrld bus number", G_TYPE_INT,
			bus_data.remote_ctrld_bus_number,
			NULL);

	return CONTING_ITEM_DATA(item_data);
}

static ContingItemData *
conting_file_read_branch(const char *line)
{
	GObject *item_data;

	branch_data_t branch_data;

	item_data = g_object_new(CONTING_TYPE_ITEM_DATA,
			"type", CONTING_ITEM_TYPE_BRANCH,
			NULL);

	conting_file_branch_data(&branch_data, line);

	/* Set attributes */
	conting_item_data_set_attr(CONTING_ITEM_DATA(item_data),
			"tap bus number", G_TYPE_INT, branch_data.tap_bus_number,
			"z bus number", G_TYPE_INT, branch_data.z_bus_number,
			NULL);

	return CONTING_ITEM_DATA(item_data);
}

GList *
conting_file_get_item_data(const char *filename)
{
	FILE *fp;
	char buf[256];
	GList *item_data_list;
	ContingItemData *item_data;

	item_data_list = NULL;

	fp = fopen(filename, "r");

	if (fp == NULL)
		return NULL;

	while (fgets(buf, 256, fp)) {
		if (strncmp(buf, "BUS", 3) == 0) {
			while (fgets(buf, 256, fp)) {

				if (strncmp(buf, "-999", 4) == 0)
					break;

				item_data = conting_file_read_bus(buf);
				assert(item_data);
				item_data_list = g_list_append(item_data_list, item_data);
			}
		} else if (strncmp(buf, "BRANCH", 6) == 0) {
			while (fgets(buf, 256, fp)) {

				if (strncmp(buf, "-999", 4) == 0)
					break;

				item_data = conting_file_read_branch(buf);
				assert(item_data);
				item_data_list = g_list_append(item_data_list, item_data);
			}
		}
	}

	return item_data_list;
}
