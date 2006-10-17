#include "contingdrawingoperation.h"

typedef struct ContingDrawingOperationDefault_ ContingDrawingOperationDefault;
typedef struct ContingDrawingOperationDefaultClass_ ContingDrawingOperationDefaultClass;

struct ContingDrawingOperationDefault_ {
	GObject parent;
};

struct ContingDrawingOperationDefaultClass_
{
	GObjectClass parent;
};

static void
conting_drawing_operation_default_get_bounds(ContingDrawingOperation *self,
		ContingDrawing *drawing, ArtDRect *bounds)
{
	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_OPERATION(self));

	conting_drawing_get_update_bounds(drawing, bounds);
}

static void
conting_drawing_operation_default_draw(ContingDrawingOperation *self,
		ContingDrawing *drawing)
{
	cairo_t *cr;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_OPERATION(self));

	cr = conting_drawing_get_cairo(drawing);

	conting_drawing_draw(drawing, cr);

	cairo_destroy(cr);
}

static void
conting_drawing_operation_default_drawing_operation_init(gpointer g_iface,
		gpointer iface_data)
{
	ContingDrawingOperationClass *opr_class;

	opr_class = g_iface;
	opr_class->get_bounds = conting_drawing_operation_default_get_bounds;
	opr_class->draw = conting_drawing_operation_default_draw;
}

GType
conting_drawing_operation_default_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo type_info = {
			sizeof(ContingDrawingOperationDefaultClass),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			sizeof(ContingDrawingOperationDefault),
			0,
			NULL,
			NULL,
		};

		static const GInterfaceInfo iface_info = {
			conting_drawing_operation_default_drawing_operation_init,
			NULL,
			NULL
		};

		type = g_type_register_static(G_TYPE_OBJECT,
				"ContingDrawingOperationDefault",
				&type_info, 0);

		g_type_add_interface_static(type,
				CONTING_TYPE_DRAWING_OPERATION,
				&iface_info);
	}

	return type;
}
