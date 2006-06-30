#ifndef CONTING_FILE_H
#define CONTING_FILE_H

typedef struct {
	int number;
	char name[12];
	int load_flow_area;
	int loss_zone;
	int type;
	double final_voltage;
	double final_angle;
	double load_mw;
	double load_mvar;
	double gen_mw;
	double gen_mvar;
	double base_kv;
	double desired_voltage;
	double max_mvar;
	double min_mvar;
	double shunt_conductance;
	double shunt_susceptance;
	int remote_ctrld_bus_number;
} bus_data_t;

char *
conting_file_string(const char *line,
		unsigned int start, unsigned int end);

double
conting_file_float(const char *line,
		unsigned int start, unsigned int end);

int
conting_file_int(const char *line,
		unsigned int start, unsigned int end);

#endif /* CONTING_FILE_H */
