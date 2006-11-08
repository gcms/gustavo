#include "contingdrawingoperationdefault.h"
void
conting_drawing_operation_default_set_mask(
		ContingDrawingOperationDefault *self, gint mask)
{
	self->mask = mask;

	/* TODO: emit a signal */
}

gint
conting_drawing_operation_default_get_mask(
		ContingDrawingOperationDefault *self)
{
	return self->mask;
}


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
	ContingDrawingOperationDefault *opr;
	cairo_t *cr;
	gboolean show = TRUE;

	g_return_if_fail(self != NULL && CONTING_IS_DRAWING_OPERATION(self));

	opr = CONTING_DRAWING_OPERATION_DEFAULT(self);

	/* TODO: maybe change the way it is done. */
	cr = conting_drawing_get_cairo(drawing);

	if (opr->enable)
	if (CONTING_IS_GEN(drawing)
			&& !(opr->mask & CONTING_DRAWING_OPERATION_DEFAULT_GEN)) {
		show = FALSE;
	} else if (CONTING_IS_LOAD(drawing)
			&& !(opr->mask & CONTING_DRAWING_OPERATION_DEFAULT_LOAD)) {
		show = FALSE;
	} else if (CONTING_IS_LINE(drawing)) {
		ContingComponent *comp0, *comp1, *result;

		conting_line_get_links(CONTING_LINE(drawing), &comp0, &comp1);

		result = CONTING_IS_SYMBOL(comp0) ? comp0
			: CONTING_IS_SYMBOL(comp1) ? comp1 : NULL;

		if (result == NULL) {
			show = TRUE;
		} else if (CONTING_IS_GEN(result)
				&& !(opr->mask & CONTING_DRAWING_OPERATION_DEFAULT_GEN)) {
			show = FALSE;
		} else if (CONTING_IS_LOAD(result)
				&& !(opr->mask & CONTING_DRAWING_OPERATION_DEFAULT_LOAD)) {
			show = FALSE;
		}
	}

	if (show)
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

static void
conting_drawing_operation_default_instance_init(GTypeInstance *self,
		gpointer g_class)
{
	ContingDrawingOperationDefault *opr;
	
	g_return_if_fail(self != NULL
			&& CONTING_IS_DRAWING_OPERATION_DEFAULT(self));	

	opr = CONTING_DRAWING_OPERATION_DEFAULT(self);

	opr->mask = CONTING_DRAWING_OPERATION_DEFAULT_GEN
		| CONTING_DRAWING_OPERATION_DEFAULT_LOAD;
	opr->enable = FALSE;
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
			NULL, /* class_init */
			NULL,
			NULL,
			sizeof(ContingDrawingOperationDefault),
			0,
			conting_drawing_operation_default_instance_init, /* instance_init */
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
