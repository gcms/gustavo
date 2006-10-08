#include "contingdrawing.h"
#include "contingbusbase.h"


static void
conting_bus_base_init(ContingBusBase *self)
{
}
static void
conting_bus_base_class_init(ContingBusBaseClass *g_class)
{
}

G_DEFINE_ABSTRACT_TYPE(ContingBusBase, conting_bus_base,
		CONTING_TYPE_COMPONENT);
