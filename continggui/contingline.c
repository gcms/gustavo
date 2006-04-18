#include "contingline.h"

static void conting_line_instance_init(GTypeInstance *obj, gpointer g_class) {
}

static void conting_line_class_init(gpointer g_class, gpointer class_data) {
}

GType conting_line_get_type(void) {
	static GType type = 0;

	if (type == 0) {
		static GTypeInfo type_info = {
			sizeof(ContingLineClass),
			NULL,
			NULL,
			conting_line_class_init,
			NULL,
			NULL,
			sizeof(ContingLine),
			0,
			conting_line_instance_init,
			NULL
		};

		type g_type_register_static(CONTING_TYPE_CONNECTION,
				"ContingLine",
				&type_info, 0);
	}

	return type;
}
