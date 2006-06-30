#include "contingfile.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

char *
conting_file_string(const char *line,
		unsigned int start, unsigned int end)
{
	char *result;

	result = malloc((end - start + 2) * sizeof(char));
	assert(result);
	strncpy(result, line + start, end - start + 1);

	return result;
}

double
conting_file_float(const char *line,
		unsigned int start, unsigned int end)
{
	double result;
	char *string = conting_file_string(line, start, end);

	sscanf(string, "%lf", &result);

	free(string);

	return result;
}

int
conting_file_int(const char *line,
		unsigned int start, unsigned int end)
{
	int result;
	char *string = conting_file_string(line, start, end);

	sscanf(string, "%d", &result);

	free(string);

	return result;
}

bus_data_t *
conting_file_bus_data(const char *line)
{
	bus_data_t *self;
	char *name;

	self = malloc(sizeof(bus_data_t));
	assert(self);

	self->number = conting_file_int(line, 0, 3);

	name = conting_file_string(line, 6, 16);
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

	return self;
}
