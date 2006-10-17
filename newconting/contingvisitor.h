#ifndef CONTING_VISITOR_H
#define CONTING_VISITOR_H

#include <glib.h>
#include <glib-object.h>

#define CONTING_TYPE_VISITOR			(conting_visitor_get_type())
#define CONTING_VISITOR(o)				(G_TYPE_CHECK_INSTANCE_CAST ((o), \
			CONTING_TYPE_VISITOR, ContingVisitor))
#define CONTING_VISITOR_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST ((k), \
			CONTING_TYPE_VISITOR, ContingVisitorClass))
#define CONTING_IS_VISITOR(o)			(G_TYPE_CHECK_INSTANCE_TYPE ((o), \
			CONTING_TYPE_VISITOR))
#define CONTING_IS_VISITOR_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), \
			CONTING_TYPE_VISITOR))
#define CONTING_VISITOR_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_INTERFACE ((o), \
			CONTING_TYPE_VISITOR, ContingVisitorClass))


typedef struct ContingVisitor_ ContingVisitor;
typedef struct ContingVisitorClass_ ContingVisitorClass;

#include "contingdrawing.h"
#include "contingline.h"
#include "contingbus.h"
#include "contingtrans2.h"
#include "contingtrans3.h"
#include "contingload.h"
#include "continggen.h"

struct ContingVisitorClass_ {
	GTypeInterface parent;

	void (*visit_line)(ContingVisitor *self, ContingLine *line);
	void (*visit_bus)(ContingVisitor *self, ContingBus *bus);
	void (*visit_trans2)(ContingVisitor *self, ContingTrans2 *trans2);
	void (*visit_trans3)(ContingVisitor *self, ContingTrans3 *trans3);
	void (*visit_load)(ContingVisitor *self, ContingLoad *load);
	void (*visit_gen)(ContingVisitor *self, ContingGen *gen);
};

GType
conting_visitor_get_type(void);

void conting_visitor_visit_line(ContingVisitor *self, ContingLine *line);
void conting_visitor_visit_bus(ContingVisitor *self, ContingBus *bus);
void conting_visitor_visit_trans2(ContingVisitor *self, ContingTrans2 *trans2);
void conting_visitor_visit_trans3(ContingVisitor *self, ContingTrans3 *trans3);
void conting_visitor_visit_load(ContingVisitor *self, ContingLoad *load);
void conting_visitor_visit_gen(ContingVisitor *self, ContingGen *gen);


#define CONTING_TYPE_VISITOR_DRAW	(conting_visitor_draw_get_type())

GType conting_visitor_draw_get_type(void);
#endif /* CONTING_VISITOR_H */
