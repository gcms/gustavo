#ifndef CONTING_FILE_H
#define CONTING_FILE_H

#include <glib.h>
#include <glib-object.h>

gchar *
conting_file_string(const gchar *line,
		guint start, guint end);

gdouble
conting_file_float(const gchar *line,
		guint start, guint end);

gint
conting_file_int(const gchar *line,
		guint start, guint end);

#define CONTING_TYPE_FILE			(conting_file_get_type())
#define CONTING_FILE(o)				(G_TYPE_CHECK_INSTANCE_CAST ((o), \
			CONTING_TYPE_FILE, ContingFile))
#define CONTING_FILE_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST ((k), \
			CONTING_TYPE_FILE, ContingFileClass))
#define CONTING_IS_FILE(o)			(G_TYPE_CHECK_INSTANCE_TYPE ((o), \
			CONTING_TYPE_FILE))
#define CONTING_IS_FILE_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), \
			CONTING_TYPE_FILE))
#define CONTING_FILE_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_INTERFACE ((o), \
			CONTING_TYPE_FILE, ContingFileClass))


typedef struct ContingFile_ ContingFile;
typedef struct ContingFileClass_ ContingFileClass;

struct ContingFileClass_ {
	GTypeInterface parent;

	GList *(*get_item_data)(ContingFile *self, const gchar *filename);
};

GType
conting_file_get_type(void);

GList *
conting_file_get_item_data(ContingFile *self, const gchar *filename);

#endif /* CONTING_FILE_H */
