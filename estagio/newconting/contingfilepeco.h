#ifndef CONTING_FILE_PECO_H
#define CONTING_FILE_PECO_H

#define CONTING_TYPE_FILE_PECO			(conting_file_peco_get_type())
#define CONTING_FILE_PECO(o)				(G_TYPE_CHECK_INSTANCE_CAST ((o), \
			CONTING_TYPE_FILE_PECO, ContingFilePECO))
#define CONTING_FILE_PECO_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST ((k), \
			CONTING_TYPE_FILE_PECO, ContingFilePECOClass))
#define CONTING_IS_FILE_PECO(o)			(G_TYPE_CHECK_INSTANCE_TYPE ((o), \
			CONTING_TYPE_FILE_PECO))
#define CONTING_IS_FILE_PECO_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), \
			CONTING_TYPE_FILE_PECO))
#define CONTING_FILE_PECO_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), \
			CONTING_TYPE_FILE_PECO, ContingFilePECOClass))

typedef struct ContingFilePECO_ ContingFilePECO;
typedef struct ContingFilePECOClass_ ContingFilePECOClass;

struct ContingFilePECO_
{
	GObject parent;
};

struct ContingFilePECOClass_
{
	GObjectClass parent;
};

GType
conting_file_peco_get_type(void);

ContingFile *
conting_file_peco_new(void);


#endif /* CONTING_FILE_PECO_H */
