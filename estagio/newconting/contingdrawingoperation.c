#include "contingdrawing.h"
#include "contingdrawingoperation.h"

GType
conting_drawing_operation_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo info = {
			sizeof(ContingDrawingOperationClass),
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			0,
			0,
			NULL
		};

		type = g_type_register_static(G_TYPE_INTERFACE,
				"ContingDrawingOperation",
				&info, 0);
	}

	return type;
}
void
conting_drawing_operation_get_bounds(ContingDrawingOperation *self,
		ContingDrawing *drawing, ArtDRect *bounds)
{
	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_OPERATION(self));

	CONTING_DRAWING_OPERATION_GET_CLASS(self)->get_bounds(self, drawing,
			bounds);
}
void
conting_drawing_operation_draw(ContingDrawingOperation *self,
		ContingDrawing *drawing)
{
	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_OPERATION(self));
	
	CONTING_DRAWING_OPERATION_GET_CLASS(self)->draw(self, drawing);
}
