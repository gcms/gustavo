#include "contingfile.h"
#include "contingfilepeco.h"
#include "contingitemdata.h"

#include <stdio.h>
#include <string.h>

#define is_digit(c)		(((c) >= '0' && (c) <= '9') || (c) == ' ')

static gboolean
conting_file_peco_follows(const gchar *filename)
{
	FILE *fp;
	gchar buf[256];
	int i;
	gboolean result = TRUE;

	if ((fp = fopen(filename, "r")) == NULL)
		return FALSE;

	if (fgets(buf, 256, fp) == NULL) {
		result = FALSE;
		goto follows_end;
	}

	for (i = 0; i < 3; i++) {
		if (!is_digit(buf[i]))
			result = FALSE;
	}

	result = buf[7] >= '0' && buf[7] <= '9';

follows_end:
	fclose(fp);

	return result;
}

static ContingItemData *
conting_file_peco_read_bus(const gchar *line)
{
	GObject *item_data;
	gint number, type;
	gchar *name;

	item_data = g_object_new(CONTING_TYPE_ITEM_DATA,
			"type", CONTING_ITEM_TYPE_BUS,
			NULL);

	number = conting_file_int(line, 0, 3);
	name = conting_file_string(line, 9, 20);
	type = conting_file_int(line, 7, 7);

	switch (type) {
		case 0:
			type = CONTING_BUS_TYPE_PQ;
			break;
		case 1:
			type = CONTING_BYS_TYPE_GEN_PV;
			break;
		case 2:
			type = CONTING_BUS_TYPE_SLACK;
			break;
	}

	conting_item_data_set_attr(CONTING_ITEM_DATA(item_data),
			"number", G_TYPE_INT, number,
			"type", G_TYPE_INT, type,
			"name", G_TYPE_STRING, name,
			NULL);

	g_free(name);

	return CONTING_ITEM_DATA(item_data);
}

static ContingItemData *
conting_file_peco_read_branch(const char *line)
{
	GObject *item_data;

	item_data = g_object_new(CONTING_TYPE_ITEM_DATA,
			"type", CONTING_ITEM_TYPE_BRANCH,
			NULL);

	conting_item_data_set_attr(CONTING_ITEM_DATA(item_data),
			"tap bus number", G_TYPE_INT, conting_file_int(line, 0, 3),
			"z bus number", G_TYPE_INT, conting_file_int(line, 8, 11),
			NULL);

	return CONTING_ITEM_DATA(item_data);
}

static void
conting_file_peco_add_bus(FILE *fp, GList **item_data_list) {
	ContingItemData *item_data;
	char buf[256];

	while (fgets(buf, 256, fp)) {
		if (strncmp(buf, "9999", 4) == 0)
			break;

		item_data = conting_file_peco_read_bus(buf);
		if (item_data)
			*item_data_list = g_list_append(*item_data_list, item_data);
	}
}

static void
conting_file_peco_add_branch(FILE *fp, GList **item_data_list) {
	ContingItemData *item_data;
	char buf[256];

	while (fgets(buf, 256, fp)) {
		if (strncmp(buf, "9999", 4) == 0)
			break;

		item_data = conting_file_peco_read_branch(buf);
		if (item_data)
			*item_data_list = g_list_append(*item_data_list, item_data);
	}
}


static GList *
conting_file_peco_get_item_data(ContingFile *self, const gchar *filename)
{
	FILE *fp;
	GList *item_data_list;

	g_return_val_if_fail(self != NULL && CONTING_IS_FILE_PECO(self), NULL);


	item_data_list = NULL;

	fp = fopen(filename, "r");
	
	if (fp == NULL)
		return NULL;

	conting_file_peco_add_bus(fp, &item_data_list);
	conting_file_peco_add_branch(fp, &item_data_list);

	fclose(fp);

	return item_data_list;
}

static void
conting_file_peco_file_init(gpointer g_iface, gpointer iface_data)
{
	ContingFileClass *file_class;

	file_class = g_iface;
	file_class->get_item_data = conting_file_peco_get_item_data;
	file_class->follows = conting_file_peco_follows;
}

static void
conting_file_peco_init(GTypeInstance *self, gpointer g_class)
{
}

GType
conting_file_peco_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo type_info = {
			sizeof(ContingFilePECOClass),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			sizeof(ContingFilePECO),
			0,
			conting_file_peco_init
		};

		static const GInterfaceInfo iface_info = {
			conting_file_peco_file_init,
			NULL,
			NULL
		};

		type = g_type_register_static(G_TYPE_OBJECT,
				"ContingFilePECO",
				&type_info, 0);

		g_type_add_interface_static(type,
				CONTING_TYPE_FILE,
				&iface_info);
	}

	return type;
}

ContingFile *
conting_file_peco_new(void)
{
	GObject *self = g_object_new(CONTING_TYPE_FILE_PECO, NULL);

	return CONTING_FILE(self);
}
