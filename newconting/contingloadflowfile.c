#include <stdlib.h>
#include <stdio.h>
#include "contingitemdata.h"

#include <glib.h>

static void
skip_lines(FILE *fp, int n_lines)
{
	int ch;

	while (n_lines > 0 && (ch = fgetc(fp)) != EOF)
		if (ch == '\n')
			n_lines--;
}

static ContingItemData *
read_item_data_relat1(const gchar *line)
{
	ContingItemData *item_data;

	gint no_bus;
	gchar type[4];
	gdouble value, angle, gen_active, gen_reactive, li, ls;
	gdouble load_active, load_reactive;
	
	sscanf(line, "%d %s %lf %lf %lf %lf %lf %lf %lf %lf",
			&no_bus, type, &value, &angle, &gen_active, &gen_reactive,
			&li, &ls, &load_active, &load_reactive);

	item_data = g_object_new(CONTING_TYPE_ITEM_DATA,
		"type", CONTING_ITEM_TYPE_FLOW_BUS,
		NULL);

	conting_item_data_set_attr(item_data,
		"number", G_TYPE_INT, no_bus,
		"voltage", G_TYPE_DOUBLE, value,
		"angle", G_TYPE_DOUBLE, angle,
		"gen-active", G_TYPE_DOUBLE, gen_active,
		"gen-reactive", G_TYPE_DOUBLE, gen_reactive,
		"li", G_TYPE_DOUBLE, li,
		"ls", G_TYPE_DOUBLE, ls,
		"load-active", G_TYPE_DOUBLE, load_active,
		"load-reactive", G_TYPE_DOUBLE, load_reactive,
		NULL);

	return item_data;
}

static ContingItemData *
read_item_data_relat2(const gchar *line)
{
	ContingItemData *item_data;

	gchar dummy[3];
	gint start1, end1, start2, end2;
	gdouble p1, q1, s1, p2, q2, s2;
	gdouble loss_p, loss_q;

	sscanf(line, "%d %d %lf %lf %lf %s %d %d %lf %lf %lf %s %lf %lf",
			&start1, &end1, &p1, &q1, &s1, dummy,
			&start2, &end2, &p2, &q2, &s2, dummy,
			&loss_p, &loss_q);

	item_data = g_object_new(CONTING_TYPE_ITEM_DATA,
			"type", CONTING_ITEM_TYPE_FLOW_BRANCH,
			NULL);

	conting_item_data_set_attr(item_data,
			"start-1", G_TYPE_INT, start1,
			"end-1", G_TYPE_INT, end1,
			"p-1", G_TYPE_DOUBLE, p1,
			"q-1", G_TYPE_DOUBLE, q1,
			"s-1", G_TYPE_DOUBLE, s1,
			"start-2", G_TYPE_INT, start2,
			"end-2", G_TYPE_INT, end2,
			"p-2", G_TYPE_DOUBLE, p2,
			"q-2", G_TYPE_DOUBLE, q2,
			"s-2", G_TYPE_DOUBLE, s2,
			"loss-p", G_TYPE_DOUBLE, loss_p,
			"loss-q", G_TYPE_DOUBLE, loss_q,
			NULL);

	printf("%d %d\t%d %d\n", start1, end1, start2, end2);

	return item_data;
}
static ContingItemData *
read_item_data_trans(const gchar *line)
{
	ContingItemData *item_data;

	gint start, end;
	gdouble initial, min, max, final;

	sscanf(line, "%d %d %lf %lf %lf %lf",
			&start, &end, &initial, &min, &max, &final);

	item_data = g_object_new(CONTING_TYPE_ITEM_DATA,
			"type", CONTING_ITEM_TYPE_FLOW_TRANS,
			NULL);

	conting_item_data_set_attr(item_data,
			"start", G_TYPE_INT, start,
			"end", G_TYPE_INT, end,
			"initial", G_TYPE_DOUBLE, initial,
			"min", G_TYPE_DOUBLE, min,
			"max", G_TYPE_DOUBLE, max,
			"final", G_TYPE_DOUBLE, final,
			NULL);

	return item_data;
}

GList *
conting_load_flow_file_relat1(const gchar *filename)
{
	FILE *fp = fopen(filename, "r");
	gchar buf[256];
	GList *result = NULL;

	if (fp == NULL)
		return NULL;

	skip_lines(fp, 9);

	while (fgets(buf, 256, fp)) {
		ContingItemData *item_data;
		
		if (buf[0] == '-')
			break;

		item_data = read_item_data_relat1(buf);

		result = g_list_append(result, item_data);
		puts(buf);
	}

	return result;
}


GList *
conting_load_flow_file_relat2(const gchar *filename)
{
	ContingItemData *item_data;

	FILE *fp = fopen(filename, "r");
	gchar buf[256];
	GList *result = NULL;

	if (fp == NULL)
		return NULL;

	skip_lines(fp, 8);

	while (fgets(buf, 256, fp)) {
		if (buf[0] == '-')
			break;
		
		item_data = read_item_data_relat2(buf);
		result = g_list_append(result, item_data);
		puts(buf);
	}

	skip_lines(fp, 9);

	while (fgets(buf, 256, fp)) {
		if (buf[0] == '-')
			break;
		item_data = read_item_data_trans(buf);
		result = g_list_append(result, item_data);
		puts(buf);
	}

	return result;
}

GList *
conting_load_flow_file_read(const gchar *relat1, const gchar *relat2)
{
	GList *result1, *result2;

	result1 = conting_load_flow_file_relat1(relat1);
	result2 = conting_load_flow_file_relat2(relat2);

	return g_list_concat(result1, result2);
}

/*
int
main(int argc, char *argv[])
{
	GList *result;
	g_type_init();

	result = conting_load_flow_file_read("relat1.txt", "relat2.txt");

	while (result != NULL) {
		g_print("%p ", result->data);
		conting_item_data_print(result->data);
		result = g_list_next(result);
	}

	return 0;
}
*/
