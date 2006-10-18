#include "contingdrawing.h"
#include "contingbusbase.h"


/* INSTANCE INIT */
static void
conting_bus_base_init(ContingBusBase *self)
{
}

/* CLASS INIT */
static void
conting_bus_base_class_init(ContingBusBaseClass *g_class)
{
}

/* CLASS DEFINITION */
G_DEFINE_ABSTRACT_TYPE(ContingBusBase, conting_bus_base,
		CONTING_TYPE_COMPONENT);
