#ifndef CONTING_XML_H
#define CONTING_XML_H

#include <gtk/gtk.h>
#include <libart_lgpl/libart.h>
#include <libxml/tree.h>
#include "contingdrawing.h"

typedef
xmlNodePtr (*ContingSerializeFunc)(gconstpointer object,
        gpointer user_data);
typedef
gpointer (*ContingDeserializeFunc)(xmlNodePtr node,
        gpointer user_data);


xmlNodePtr conting_util_affine_node(const char *name, gdouble affine[6]);
xmlNodePtr conting_util_point_node(const char *name, ArtPoint *p);
xmlNodePtr conting_util_drawing_node(const char *name, ContingDrawing *drawing);
xmlNodePtr conting_util_hash_node(const char *name,
		                          GHashTable *hash_table,
								  ContingSerializeFunc key_func,
								  ContingSerializeFunc val_func,
                                  gpointer user_data);
xmlNodePtr conting_util_list_node(const char *name,
		                          GList *list,
								  ContingSerializeFunc serialize,
                                  gpointer user_data);
void conting_util_load_affine(xmlNodePtr affine_node, gdouble affine[6]);
void conting_util_load_point(xmlNodePtr point_node, ArtPoint *p);
void conting_util_load_hash(xmlNodePtr hash_node, GHashTable *hash,
        ContingDeserializeFunc key_func, ContingDeserializeFunc val_func,
        gpointer user_data);
ContingDrawing *conting_util_load_drawing(xmlNodePtr node,
        GHashTable *id_drawing);
void conting_util_load_list(xmlNodePtr list_node, GList **list,
        ContingDeserializeFunc func, gpointer user_data);
        
#endif /* CONTING_XML_H */
