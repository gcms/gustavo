#include "contingvisitor.h"

static void
conting_visitor_class_init(gpointer g_class, gpointer class_data)
{
	ContingVisitorClass *visitor_class;

	visitor_class = CONTING_VISITOR_CLASS(g_class);

	visitor_class->visit_line = NULL;
	visitor_class->visit_bus = NULL;
	visitor_class->visit_trans2 = NULL;
	visitor_class->visit_trans3 = NULL;
	visitor_class->visit_load = NULL;
	visitor_class->visit_gen = NULL;
	visitor_class->visit_ce = NULL;
	visitor_class->visit_cs = NULL;
	visitor_class->visit_ground = NULL;
	visitor_class->visit_c_shunt = NULL;
}

GType
conting_visitor_get_type(void)
{
	static GType type = 0;

	if (type == 0) {
		static const GTypeInfo info = {
			sizeof(ContingVisitorClass),
			NULL,
			NULL,
			conting_visitor_class_init,
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

	g_return_if_fail(CONTING_VISITOR_GET_CLASS(self)->visit_line != NULL);

	CONTING_VISITOR_GET_CLASS(self)->visit_line(self, line);
}
void
conting_visitor_visit_bus(ContingVisitor *self, ContingBus *bus)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(bus != NULL && CONTING_IS_BUS(bus));

	g_return_if_fail(CONTING_VISITOR_GET_CLASS(self)->visit_bus != NULL);

	CONTING_VISITOR_GET_CLASS(self)->visit_bus(self, bus);
}
void
conting_visitor_visit_trans2(ContingVisitor *self, ContingTrans2 *trans2)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(trans2 != NULL && CONTING_IS_TRANS2(trans2));

	g_return_if_fail(CONTING_VISITOR_GET_CLASS(self)->visit_trans2 != NULL);

	CONTING_VISITOR_GET_CLASS(self)->visit_trans2(self, trans2);
}
void
conting_visitor_visit_trans3(ContingVisitor *self, ContingTrans3 *trans3)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(trans3 != NULL && CONTING_IS_TRANS3(trans3));

	g_return_if_fail(CONTING_VISITOR_GET_CLASS(self)->visit_trans3 != NULL);

	CONTING_VISITOR_GET_CLASS(self)->visit_trans3(self, trans3);
}
void
conting_visitor_visit_load(ContingVisitor *self, ContingLoad *load)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(load != NULL && CONTING_IS_LOAD(load));

	g_return_if_fail(CONTING_VISITOR_GET_CLASS(self)->visit_load != NULL);

	CONTING_VISITOR_GET_CLASS(self)->visit_load(self, load);
}
void
conting_visitor_visit_gen(ContingVisitor *self, ContingGen *gen)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(gen != NULL && CONTING_IS_GEN(gen));

	g_return_if_fail(CONTING_VISITOR_GET_CLASS(self)->visit_gen != NULL);

	CONTING_VISITOR_GET_CLASS(self)->visit_gen(self, gen);
}
void
conting_visitor_visit_ce(ContingVisitor *self, ContingCE *gen)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(gen != NULL && CONTING_IS_CE(gen));

	g_return_if_fail(CONTING_VISITOR_GET_CLASS(self)->visit_ce != NULL);

	CONTING_VISITOR_GET_CLASS(self)->visit_ce(self, gen);
}
void
conting_visitor_visit_cs(ContingVisitor *self, ContingCS *gen)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(gen != NULL && CONTING_IS_CS(gen));

	g_return_if_fail(CONTING_VISITOR_GET_CLASS(self)->visit_cs != NULL);

	CONTING_VISITOR_GET_CLASS(self)->visit_cs(self, gen);
}
void
conting_visitor_visit_ground(ContingVisitor *self, ContingGround *gen)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(gen != NULL && CONTING_IS_GROUND(gen));

	g_return_if_fail(CONTING_VISITOR_GET_CLASS(self)->visit_ground != NULL);

	CONTING_VISITOR_GET_CLASS(self)->visit_ground(self, gen);
}


void
conting_visitor_visit_c_shunt(ContingVisitor *self, ContingCShunt *gen)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(gen != NULL && CONTING_IS_C_SHUNT(gen));

	g_return_if_fail(CONTING_VISITOR_GET_CLASS(self)->visit_c_shunt != NULL);

	CONTING_VISITOR_GET_CLASS(self)->visit_c_shunt(self, gen);
}


void
conting_visitor_visit_r_shunt(ContingVisitor *self, ContingRShunt *gen)
{
	g_return_if_fail(self != NULL && CONTING_IS_VISITOR(self));
	g_return_if_fail(gen != NULL && CONTING_IS_R_SHUNT(gen));

	g_return_if_fail(CONTING_VISITOR_GET_CLASS(self)->visit_r_shunt != NULL);

	CONTING_VISITOR_GET_CLASS(self)->visit_r_shunt(self, gen);
}


