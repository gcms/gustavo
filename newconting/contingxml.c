#include "contingxml.h"

xmlNodePtr conting_util_affine_node(const char *name, gdouble affine[6]) {
    xmlNodePtr node;
    char buff[256];

    node = xmlNewNode(NULL, BAD_CAST "attribute");
    xmlNewProp(node, BAD_CAST "type", BAD_CAST "affine");
    xmlNewProp(node, BAD_CAST "name", BAD_CAST name);

    sprintf(buff, "%lf %lf %lf %lf %lf %lf", affine[0], affine[1], affine[2],
            affine[3], affine[4], affine[5]);
    xmlAddChild(node, xmlNewText(BAD_CAST buff));

    return node;
}
xmlNodePtr conting_util_point_node(const char *name, ArtPoint *p)
{
    xmlNodePtr node;
    char buff[256];

    node = xmlNewNode(NULL, BAD_CAST "attribute");
    xmlNewProp(node, BAD_CAST "type", BAD_CAST "point");
    xmlNewProp(node, BAD_CAST "name", BAD_CAST name);

    sprintf(buff, "%lf %lf", p->x, p->y);
    xmlAddChild(node, xmlNewText(BAD_CAST buff));

    return node;
}
xmlNodePtr conting_util_drawing_node(const char *name, ContingDrawing *drawing)
{
    xmlNodePtr node;
    guint id;
    char buff[256];

    node = xmlNewNode(NULL, BAD_CAST "attribute");
    xmlNewProp(node, BAD_CAST "type", BAD_CAST "drawing");
    xmlNewProp(node, BAD_CAST "name", name);
    
    g_object_get(G_OBJECT(drawing),
            "id", &id,
            NULL);
    sprintf(buff, "%u", id);
    xmlNewProp(node, BAD_CAST "id", BAD_CAST buff);

    return node;
}

static void hash_foreach(gpointer key, gpointer value, gpointer user_data)
{
	xmlNodePtr parent;
	xmlNodePtr node;
	ContingSerializeFunc key_func;
	ContingSerializeFunc val_func;
    gpointer extra_data;

	gpointer *data = user_data;

	parent = data[0];
	key_func = data[1];
	val_func = data[2];
    extra_data = data[3];

	node = xmlNewNode(NULL, "node");
	xmlAddChild(node, key_func(key, extra_data));
	xmlAddChild(node, val_func(value, extra_data));

	xmlAddChild(parent, node);
}

xmlNodePtr conting_util_hash_node(const char *name,
		                          GHashTable *hash_table,
								  ContingSerializeFunc key_func,
								  ContingSerializeFunc val_func,
                                  gpointer extra_data)
{
	xmlNodePtr node;
	gpointer user_data[4];

	node = xmlNewNode(NULL, BAD_CAST "attribute");
	xmlNewProp(node, BAD_CAST "type", BAD_CAST "map");
	xmlNewProp(node, BAD_CAST "name", BAD_CAST name);

	user_data[0] = node;
	user_data[1] = key_func;
	user_data[2] = val_func;
    user_data[3] = extra_data;
	g_hash_table_foreach(hash_table, hash_foreach, user_data);

	return node;
}
xmlNodePtr conting_util_list_node(const char *name,
		                          GList *list,
								  ContingSerializeFunc serialize,
                                  gpointer user_data)
{
	xmlNodePtr node;
	GList *n;

	node = xmlNewNode(NULL, BAD_CAST "attribute");
	xmlNewProp(node, BAD_CAST "type", BAD_CAST "list");
	xmlNewProp(node, BAD_CAST "name", name);

	g_print("g_list_next()\n");
	for (n = list; n != NULL; n = g_list_next(n)) {
		xmlAddChild(node, serialize(n->data, user_data));
	}

	return node;
}
void conting_util_load_point(xmlNodePtr point_node, ArtPoint *p) {
    xmlChar *point_text;

    point_text = xmlNodeListGetString(point_node->doc, point_node->children,
            TRUE);

    sscanf(point_text, "%lf %lf", &p->x, &p->y);

    xmlFree(point_text);
}

void conting_util_load_affine(xmlNodePtr affine_node, gdouble affine[6])
{
    xmlChar *affine_text;

    affine_text = xmlNodeListGetString(affine_node->doc, affine_node->children,
            TRUE);


    sscanf(affine_text, "%lf %lf %lf %lf %lf %lf",
            affine, affine + 1, affine + 2,
            affine + 3, affine + 4, affine + 5);

    xmlFree(affine_text);
}
void conting_util_load_hash(xmlNodePtr hash_node, GHashTable *hash,
        ContingDeserializeFunc key_func, ContingDeserializeFunc val_func,
        gpointer user_data)
{
    xmlNodePtr node;

    for (node = hash_node->children; node; node = node->next) {
        xmlNodePtr child;
        gpointer key, value;

        printf("node->name = %s\n", node->name);
        if (node->children == NULL)
            continue;

        for (child = node->children;
                child && xmlStrEqual(child->name, BAD_CAST "text");
                child = child->next);

        printf("child->name = %s\n", child->name);
        key = key_func(child, user_data);
        for (child = child->next;
                child && xmlStrEqual(child->name, BAD_CAST "text");
                child = child->next);
        printf("child->name = %s\n", child->name);
        value = val_func(child, user_data);

        printf("putting: %p (%lf %lf)\n", key,
                ((ArtPoint *) value)->x, ((ArtPoint *) value)->y);
        g_hash_table_insert(hash, key, value);
    }
}
ContingDrawing *conting_util_load_drawing(xmlNodePtr node,
        GHashTable *id_drawing) {
    xmlChar *id;
    ContingDrawing *result;

    id = xmlGetProp(node, BAD_CAST "id");

    result = g_hash_table_lookup(id_drawing,
            GUINT_TO_POINTER(strtoul(id, NULL, 10)));

    xmlFree(id);

    return result;
}

void conting_util_load_list(xmlNodePtr list_node, GList **list,
        ContingDeserializeFunc func, gpointer user_data) {
    xmlNodePtr node;

    for (node = list_node->children; node; node = node->next) {
        if (xmlStrEqual(node->name, BAD_CAST "text"))
            continue;

		g_print("g_list_append()\n");
        *list = g_list_append(*list, func(node, user_data));
    }
}
