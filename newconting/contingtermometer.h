#ifndef __CONTING_TERMOMETER_H__
#define __CONTING_TERMOMETER_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CONTING_TYPE_TERMOMETER (conting_termometer_get_type())
#define CONTING_TERMOMETER(o)          (G_TYPE_CHECK_INSTANCE_CAST ((o), \
            CONTING_TYPE_TERMOMETER, ContingTermometer))
#define CONTING_TERMOMETER_CLASS(k)    (G_TYPE_CHECK_CLASS_CAST ((k), \
            CONTING_TYPE_TERMOMETER, ContingTermometerClass))
#define CONTING_IS_TERMOMETER(o)       (G_TYPE_CHECK_INSTANCE_TYPE ((o), \
            CONTING_TYPE_TERMOMETER))
#define CONTING_IS_TERMOMETER_CLASS(k) (G_TYPE_CHECK_CLASS_TYPE ((k), \
            CONTING_TYPE_TERMOMETER))
#define CONTING_TERMOMETER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), \
            CONTING_TYPE_TERMOMETER, ContingTermometerClass))


typedef struct _ContingTermometer        ContingTermometer;
typedef struct _ContingTermometerClass   ContingTermometerClass;

struct _ContingTermometer
{
  GtkWidget widget;
};

struct _ContingTermometerClass
{
  GtkWidgetClass parent_class;
};

GType conting_termometer_get_type(void);
GtkWidget *conting_termometer_new(gdouble value);
void conting_termometer_set_value(ContingTermometer *self, gdouble value);
gdouble conting_termometer_get_value(ContingTermometer *self);



G_END_DECLS

#endif /* __CONTING_TERMOMETER_H__ */


