#include "contingvisitor.h"

GType
conting_visitor_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo info = {
			sizeof(ContingVisitorClass),
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
				"ContingVisitor",
				&info, 0);
	}

	return type;
}
void
conting_visitor_visit_line(ContingVisitor *self, ContingLine *line)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(line != NULL && CONTING_IS_LINE(line));

	CONTING_VISITOR_GET_CLASS(self)->visit_line(self, line);
}
void
conting_visitor_visit_bus(ContingVisitor *self, ContingBus *bus)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(bus != NULL && CONTING_IS_BUS(bus));

	CONTING_VISITOR_GET_CLASS(self)->visit_bus(self, bus);
}
void
conting_visitor_visit_trans2(ContingVisitor *self, ContingTrans2 *trans2)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(trans2 != NULL && CONTING_IS_TRANS2(trans2));

	CONTING_VISITOR_GET_CLASS(self)->visit_trans2(self, trans2);
}
void
conting_visitor_visit_trans3(ContingVisitor *self, ContingTrans3 *trans3)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(trans3 != NULL && CONTING_IS_TRANS3(trans3));

	CONTING_VISITOR_GET_CLASS(self)->visit_trans3(self, trans3);
}
void
conting_visitor_visit_load(ContingVisitor *self, ContingLoad *load)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(load != NULL && CONTING_IS_LOAD(load));

	CONTING_VISITOR_GET_CLASS(self)->visit_load(self, load);
}
void
conting_visitor_visit_gen(ContingVisitor *self, ContingGen *gen)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(gen != NULL && CONTING_IS_GEN(gen));

	CONTING_VISITOR_GET_CLASS(self)->visit_gen(self, gen);
}
void
conting_visitor_visit_ce(ContingVisitor *self, ContingCE *gen)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(gen != NULL && CONTING_IS_CE(gen));

	CONTING_VISITOR_GET_CLASS(self)->visit_ce(self, gen);
}
void
conting_visitor_visit_cs(ContingVisitor *self, ContingCS *gen)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(gen != NULL && CONTING_IS_CS(gen));

	CONTING_VISITOR_GET_CLASS(self)->visit_cs(self, gen);
}

