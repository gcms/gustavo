#include "contingfile.h"
#include "contingfilecdf.h"
#include "contingitemdata.h"

#include <stdio.h>
#include <string.h>

#define is_digit(c)		(((c) >= '0' && (c) <= '9') || (c) == ' ')

static gboolean
conting_file_cdf_follows(const gchar *filename)
{
	FILE *fp;
	gchar buf[256];
	gboolean result = TRUE;

	if ((fp = fopen(filename, "r")) == NULL)
		return FALSE;

	if (fgets(buf, 256, fp) == NULL) {
		result = FALSE;
		goto follows_end;
	}

	result = is_digit(buf[1]) && is_digit(buf[2]) && buf[3] == '/'
		&& is_digit(buf[4]) && is_digit(buf[5]) && buf[6] == '/';


follows_end:
	fclose(fp);
	
	return result;
}


static ContingItemData *
conting_file_cdf_read_bus(const char *line)
{
	GObject *item_data;
	gint number, type;
	gchar *name;
	gdouble voltage;
	gdouble angle;

	gdouble load_mw, load_mvar;
	gdouble gen_mw, gen_mvar;

	gdouble base_kv;

	item_data = g_object_new(CONTING_TYPE_ITEM_DATA,
			"type", CONTING_ITEM_TYPE_BUS,
			NULL);

	number = conting_file_int(line, 0, 3);
	name = conting_file_string(line, 5, 16);
	type = conting_file_int(line, 24, 25);
	voltage = conting_file_float(line, 27, 32);
	angle = conting_file_float(line, 33, 39);
	
	load_mw = conting_file_float(line, 40, 48);
	load_mvar = conting_file_float(line, 49, 58);

	gen_mw = conting_file_float(line, 59, 66);
	gen_mvar = conting_file_float(line, 67, 74);

	base_kv = conting_file_float(line, 76, 82);

	conting_item_data_set_attr(CONTING_ITEM_DATA(item_data),
			"number", G_TYPE_INT, number,
			"type", G_TYPE_INT, type,
			"name", G_TYPE_STRING, name,
			"voltage", G_TYPE_DOUBLE, voltage * base_kv,
			"angle", G_TYPE_DOUBLE, angle,
			"load mw", G_TYPE_DOUBLE, load_mw,
			"load mvar", G_TYPE_DOUBLE, load_mvar,
			"gen mw", G_TYPE_DOUBLE, gen_mw,
			"gen mvar", G_TYPE_DOUBLE, gen_mvar,
			NULL);

	g_free(name);

	return CONTING_ITEM_DATA(item_data);
}

static ContingItemData *
conting_file_cdf_read_branch(const char *line)
{
	GObject *item_data;

	item_data = g_object_new(CONTING_TYPE_ITEM_DATA,
			"type", CONTING_ITEM_TYPE_BRANCH,
			NULL);

	conting_item_data_set_attr(CONTING_ITEM_DATA(item_data),
			"tap bus number", G_TYPE_INT, conting_file_int(line, 0, 3),
			"z bus number", G_TYPE_INT, conting_file_int(line, 5, 8),
            "voltage ratio", G_TYPE_DOUBLE, conting_file_float(line, 76, 81),
        	"angle ratio", G_TYPE_DOUBLE, conting_file_float(line, 83, 89),
			"min tap", G_TYPE_DOUBLE, conting_file_float(line, 90, 96),
			"max tap", G_TYPE_DOUBLE, conting_file_float(line, 97, 103),
			NULL);

	return CONTING_ITEM_DATA(item_data);
}

static GList *
conting_file_cdf_get_item_data(ContingFile *self, const gchar *filename)
{
	FILE *fp;
	char buf[256];
	GList *item_data_list;
	ContingItemData *item_data;

	g_return_val_if_fail(self != NULL && CONTING_IS_FILE_CDF(self), NULL);


	item_data_list = NULL;

	fp = fopen(filename, "r");
	
	if (fp == NULL)
		return NULL;

	while (fgets(buf, 256, fp)) {
		if (strncmp(buf, "BUS", 3) == 0) {
			while (fgets(buf, 256, fp)) {
				if (strncmp(buf, "-999", 4) == 0)
					break;

				item_data = conting_file_cdf_read_bus(buf);
				if (item_data)
					item_data_list = g_list_append(item_data_list, item_data);
			}
		} else if (strncmp(buf, "BRANCH", 6) == 0) {
			while (fgets(buf, 256, fp)) {
				if (strncmp(buf, "-999", 4) == 0)
					break;

				item_data = conting_file_cdf_read_branch(buf);
				if (item_data)
					item_data_list = g_list_append(item_data_list, item_data);
			}
		}
	}

	fclose(fp);

	return item_data_list;
}

static void
conting_file_cdf_file_init(gpointer g_iface, gpointer iface_data)
{
	ContingFileClass *file_class;

	file_class = g_iface;
	file_class->get_item_data = conting_file_cdf_get_item_data;
	file_class->follows = conting_file_cdf_follows;
}

static void
conting_file_cdf_init(GTypeInstance *self, gpointer g_class)
{
}

GType
conting_file_cdf_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo type_info = {
			sizeof(ContingFileCDFClass),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			sizeof(ContingFileCDF),
			0,
			conting_file_cdf_init
		};

		static const GInterfaceInfo iface_info = {
			conting_file_cdf_file_init,
			NULL,
			NULL
		};

		type = g_type_register_static(G_TYPE_OBJECT,
				"ContingFileCDF",
				&type_info, 0);

		g_type_add_interface_static(type,
				CONTING_TYPE_FILE,
				&iface_info);
	}

	return type;
}

ContingFile *
conting_file_cdf_new(void)
{
	GObject *self = g_object_new(CONTING_TYPE_FILE_CDF, NULL);

	return CONTING_FILE(self);
}
