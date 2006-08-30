#ifndef CONTING_SERIALIZABLE_H
#define CONTING_SERIALIZABLE_H

#include <glib.h>
#include <glib-object.h>
#include <libxml/tree.h>

#define CONTING_TYPE_SERIALIZABLE			(conting_serializable_get_type())
#define CONTING_SERIALIZABLE(o)				(G_TYPE_CHECK_INSTANCE_CAST ((o), \
			CONTING_TYPE_SERIALIZABLE, ContingSerializable))
#define CONTING_SERIALIZABLE_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST ((k), \
			CONTING_TYPE_SERIALIZABLE, ContingSerializableClass))
#define CONTING_IS_SERIALIZABLE(o)			(G_TYPE_CHECK_INSTANCE_TYPE ((o), \
			CONTING_TYPE_SERIALIZABLE))
#define CONTING_IS_SERIALIZABLE_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), \
			CONTING_TYPE_SERIALIZABLE))
#define CONTING_SERIALIZABLE_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_INTERFACE ((o), \
			CONTING_TYPE_SERIALIZABLE, ContingSerializableClass))


typedef struct ContingSerializable_ ContingSerializable;
typedef struct ContingSerializableClass_ ContingSerializableClass;

struct ContingSerializableClass_ {
	GTypeInterface parent;

	void (*read)(ContingSerializable *self, xmlNodePtr node,
			GHashTable *id_drawing);
	void (*write)(ContingSerializable *self, xmlNodePtr node,
			xmlNodePtr *result);
};

GType
conting_serializable_get_type(void);

void
conting_serializable_read(ContingSerializable *self, xmlNodePtr node,
		GHashTable *id_drawing);

void
conting_serializable_write(ContingSerializable *self, xmlNodePtr node,
		xmlNodePtr *result);

#endif /* CONTING_SERIALIZABLE_H */
