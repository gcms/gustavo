#ifndef CONTING_FILE_CDF_H
#define CONTING_FILE_CDF_H

#define CONTING_TYPE_FILE_CDF			(conting_file_cdf_get_type())
#define CONTING_FILE_CDF(o)				(G_TYPE_CHECK_INSTANCE_CAST ((o), \
			CONTING_TYPE_FILE_CDF, ContingFileCDF))
#define CONTING_FILE_CDF_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST ((k), \
			CONTING_TYPE_FILE_CDF, ContingFileCDFClass))
#define CONTING_IS_FILE_CDF(o)			(G_TYPE_CHECK_INSTANCE_TYPE ((o), \
			CONTING_TYPE_FILE_CDF))
#define CONTING_IS_FILE_CDF_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), \
			CONTING_TYPE_FILE_CDF))
#define CONTING_FILE_CDF_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), \
			CONTING_TYPE_FILE_CDF, ContingFileCDFClass))

typedef struct ContingFileCDF_ ContingFileCDF;
typedef struct ContingFileCDFClass_ ContingFileCDFClass;

struct ContingFileCDF_
{
	GObject parent;
};

struct ContingFileCDFClass_
{
	GObjectClass parent;
};

GType
conting_file_cdf_get_type(void);

ContingFile *
conting_file_cdf_new(void);


#endif /* CONTING_FILE_CDF_H */
