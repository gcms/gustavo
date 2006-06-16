#include "contingutil.h"
#include "contingdrawing.h"
#include <math.h>

void conting_util_get_bounds(const ArtPoint *p0, const ArtPoint *p1,
		ArtDRect *bounds) {
	bounds->x0 = MIN(p0->x, p1->x);
	bounds->y0 = MIN(p0->y, p1->y);

	bounds->x1 = MAX(p0->x, p1->x);
	bounds->y1 = MAX(p0->y, p1->y);
}

void conting_util_expand_bounds(ArtDRect *bounds, gdouble value) {
	bounds->x0 -= value;
	bounds->y0 -= value;

	bounds->x1 += value;
	bounds->y1 += value;
}
void conting_util_union_bounds(const ArtDRect *bounds0,
		const ArtDRect *bounds1, ArtDRect *bounds) {
	bounds->x0 = MIN(bounds0->x0, bounds1->x0);
	bounds->y0 = MIN(bounds0->y0, bounds1->y0);

	bounds->x1 = MAX(bounds0->x1, bounds1->x1);
	bounds->y1 = MAX(bounds0->y1, bounds1->y1);
}
void conting_util_correct_bounds(const ArtDRect *bounds,
		ArtDRect *dst) {
	ArtDRect new_bounds;
	
	new_bounds.x0 = MIN(bounds->x0, bounds->x1);
	new_bounds.y0 = MIN(bounds->y0, bounds->y1);

	new_bounds.x1 = MAX(bounds->x0, bounds->x1);
	new_bounds.y1 = MAX(bounds->y0, bounds->y1);

	*dst = new_bounds;
}
void conting_util_bounds_to_rect(const ArtDRect *src, GdkRectangle *dst) {
	dst->x = (gint) MIN(src->x0, src->x1);
	dst->y = (gint) MIN(src->y0, src->y1);

	dst->width = (gint) fabs(src->x1 - src->x0);
	dst->height = (gint) fabs(src->y1 - src->y0);
}

/*
 * b1 contains b2?
 */
gboolean conting_util_bounds_contains(const ArtDRect *b1, const ArtDRect *b2) {
	return b1->x0 <= b2->x0 && b1->y0 <= b2->y0
		&& b1->x1 >= b2->x1 && b1->y1 >= b2->y1;
}
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

        *list = g_list_append(*list, func(node, user_data));
    }
}
