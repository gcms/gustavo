#include "contingserializable.h"

GType
conting_serializable_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo info = {
			sizeof(ContingSerializableClass),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			0,
			0,
			NULL
		};

		type = g_type_register_static(G_TYPE_INTERFACE, "ContingSerializable",
				&info, 0);
	}

	return type;
}

void
conting_serializable_read(ContingSerializable *self, xmlNodePtr node,
		GHashTable *id_drawing)
{
	g_return_if_fail(self != NULL && CONTING_IS_SERIALIZABLE(self));

	CONTING_SERIALIZABLE_GET_CLASS(self)->read(self, node, id_drawing);
}
void
conting_serializable_write(ContingSerializable *self, xmlNodePtr drawing_node,
		xmlNodePtr *result)
{
	g_return_if_fail(self != NULL && CONTING_IS_SERIALIZABLE(self));

	CONTING_SERIALIZABLE_GET_CLASS(self)->write(self, drawing_node, result);
}
