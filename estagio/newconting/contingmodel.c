#include "contingitemdata.h"
#include "contingmodel.h"

#include <assert.h>


/* PARENT CLASS POINTER */
static gpointer parent_class = NULL;

/* CLASS PRIVATE MODEL ACCESSOR MACRO */
#define CONTING_MODEL_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE((o), \
        CONTING_TYPE_MODEL, ContingModelPrivate))

/* CLASS PRIVATE MODEL TYPE AND STRUCTURE */
typedef struct ContingModelPrivate_ ContingModelPrivate;
struct ContingModelPrivate_ {
	GList *items;
	gint n_items;

	GHashTable *type_setup; /* hash<gint, ContingItemTypeSetup> */

	gboolean loaded;
};

typedef gpointer (*ContingItemSetupGetKey)(ContingItemData *);
typedef GDestroyNotify ContingItemSetupDestroyKey;

typedef struct {
	ContingItemType type;
	gint n_elements;

	ContingItemSetupGetKey get_key;
	ContingItemSetupDestroyKey destroy_key;

	GHashTable *key_items;
	GList *items;
} ContingItemTypeSetup;

static void
conting_item_type_setup_clear(ContingItemTypeSetup *setup)
{
	g_hash_table_remove_all(setup->key_items);
	g_list_free(setup->items);
	setup->items = NULL;
	setup->n_elements = 0;
}

static void
conting_item_type_setup_destroy(gpointer data)
{
	ContingItemTypeSetup *setup = data;

	g_hash_table_destroy(setup->key_items);
	g_list_free(setup->items);

	g_free(setup);
}

ContingItemTypeSetup *
conting_model_add_setup(ContingModel *self, ContingItemType type,
		ContingItemSetupGetKey get_key, ContingItemSetupDestroyKey destroy_key,
	   	GHashFunc hash, GEqualFunc equals)
{
	ContingModelPrivate *priv;
	ContingItemTypeSetup *setup;

	g_return_val_if_fail(self != NULL && CONTING_IS_MODEL(self), NULL);

	priv = CONTING_MODEL_GET_PRIVATE(self);

	setup = g_new(ContingItemTypeSetup, 1);
	setup->type = type;

	setup->get_key = get_key;

	setup->key_items = g_hash_table_new_full(hash, equals, destroy_key, NULL);
	setup->items = NULL;
	setup->n_elements = 0;

	g_hash_table_insert(priv->type_setup, GINT_TO_POINTER(type), setup);

	return setup;
}

static ContingItemTypeSetup *
conting_model_get_setup(ContingModel *self, ContingItemType type)
{
	ContingModelPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_MODEL(self), NULL);

	priv = CONTING_MODEL_GET_PRIVATE(self);

	return g_hash_table_lookup(priv->type_setup, GINT_TO_POINTER(type));
}


static void
conting_model_clear_items(ContingModel *self)
{
	ContingModelPrivate *priv;
	GList *n;

	g_return_if_fail(self != NULL && CONTING_IS_MODEL(self));

	priv = CONTING_MODEL_GET_PRIVATE(self);

	for (n = priv->items; n != NULL; n = g_list_next(n)) {
		g_object_unref(n->data);
	}
	g_list_free(priv->items);
	priv->items = NULL;
	priv->n_items = 0;
}

static void
conting_model_clear_setup_cb(gpointer k, gpointer v, gpointer u)
{
	conting_item_type_setup_clear(v);
}

void
conting_model_clear(ContingModel *self)
{
	ContingModelPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_MODEL(self));

	priv = CONTING_MODEL_GET_PRIVATE(self);

	g_hash_table_foreach(priv->type_setup, conting_model_clear_setup_cb, NULL);

	conting_model_clear_items(self);

	priv->loaded = FALSE;
}

void
conting_model_add_item(ContingModel *self, ContingItemData *item)
{
	ContingModelPrivate *priv;
	ContingItemTypeSetup *setup;
	ContingItemType type = -1;

	g_return_if_fail(self != NULL && CONTING_IS_MODEL(self));
	g_return_if_fail(self != NULL && CONTING_IS_ITEM_DATA(item));

	priv = CONTING_MODEL_GET_PRIVATE(self);

	priv->items = g_list_append(priv->items, item);

	g_object_get(item, "type", &type, NULL);

	setup = conting_model_get_setup(self, type);

	if (setup == NULL)
		return;

	g_hash_table_insert(setup->key_items,
			setup->get_key ? setup->get_key(item) : item,
			item);
	setup->items = g_list_append(setup->items, item);
	setup->n_elements++;
}

void
conting_model_load_list(ContingModel *self, const GList *list)
{
	ContingModelPrivate *priv;
	const GList *n;

	g_return_if_fail(self != NULL && CONTING_IS_MODEL(self));

	priv = CONTING_MODEL_GET_PRIVATE(self);

	if (priv->loaded)
		conting_model_clear(self);

	assert(priv->loaded == FALSE);

	for (n = list; n != NULL; n = g_list_next(n)) {
		conting_model_add_item(self, n->data);
	}

	priv->loaded = TRUE;
}
void
conting_model_load_file(ContingModel *self, ContingFile *file,
		const gchar *filename)
{
	ContingModelPrivate *priv;
	GList *list;

	g_return_if_fail(self != NULL && CONTING_IS_MODEL(self));

	priv = CONTING_MODEL_GET_PRIVATE(self);

	list = conting_file_get_item_data(file, filename);

	conting_model_load_list(self, list);

	g_list_free(list);
}

const GList *
conting_model_get_items_by_type(ContingModel *self, ContingItemType type)
{
	ContingItemTypeSetup *setup;
	
	setup = conting_model_get_setup(self, type);

	return setup ? setup->items : NULL;
}

const GList *
conting_model_get_buses(ContingModel *self)
{
	ContingModelPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_MODEL(self), NULL);

	priv = CONTING_MODEL_GET_PRIVATE(self);

	return conting_model_get_items_by_type(self, CONTING_ITEM_TYPE_BUS);
}

const GList *
conting_model_get_branches(ContingModel *self)
{
	ContingModelPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_MODEL(self), NULL);

	priv = CONTING_MODEL_GET_PRIVATE(self);

	return conting_model_get_items_by_type(self, CONTING_ITEM_TYPE_BRANCH);
}

const GList *
conting_model_get_items(ContingModel *self)
{
	ContingModelPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_MODEL(self), NULL);

	priv = CONTING_MODEL_GET_PRIVATE(self);

	return priv->items;
}

ContingItemData *
conting_model_get_item(ContingModel *self, ContingItemType type, gpointer key)
{
	ContingItemTypeSetup *setup;

	setup = conting_model_get_setup(self, type);

	return setup ? g_hash_table_lookup(setup->key_items, key) : NULL;
}

gboolean
conting_model_loaded(ContingModel *self)
{
	ContingModelPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_MODEL(self), FALSE);

	priv = CONTING_MODEL_GET_PRIVATE(self);

	return priv->loaded;
}

static void
conting_model_finalize(GObject *self)
{
	ContingModelPrivate *priv;

	g_return_if_fail(self != NULL && CONTING_IS_MODEL(self));

	priv = CONTING_MODEL_GET_PRIVATE(self);

	g_hash_table_destroy(priv->type_setup);

	conting_model_clear_items(CONTING_MODEL(self));

	G_OBJECT_CLASS(parent_class)->finalize(self);
}

static void
conting_model_instance_init(GTypeInstance *self, gpointer g_class)
{
	ContingModelPrivate *priv;

    g_return_if_fail(self != NULL && CONTING_IS_MODEL(self));

	priv = CONTING_MODEL_GET_PRIVATE(self);

	priv->items = NULL;
	priv->n_items = 0;
	priv->loaded = FALSE;

	priv->type_setup = g_hash_table_new_full(NULL, NULL,
			NULL, conting_item_type_setup_destroy);
}

static void
conting_model_class_init(gpointer g_class, gpointer class_data)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS(g_class);
	object_class->finalize = conting_model_finalize;

	g_type_class_add_private(g_class, sizeof(ContingModelPrivate));

    parent_class = g_type_class_peek_parent(g_class);

	g_type_class_ref(CONTING_TYPE_ITEM_DATA);
}

GType conting_model_get_type(void) {
    static GType type = 0;

    if (type == 0) {
        static GTypeInfo type_info = {
            sizeof(ContingModelClass),
            NULL,
            NULL,
            conting_model_class_init,
            NULL,
            NULL,
            sizeof(ContingModel),
            0,
            conting_model_instance_init,
            NULL
        };

        type = g_type_register_static(G_TYPE_OBJECT,
                "ContingModel",
                &type_info, 0);
    }

    return type;
}

struct ContingItemBranchKey_ {
	gint x, y;
};

ContingItemBranchKey *
conting_item_branch_key_new(gint x, gint y)
{
	ContingItemBranchKey *key = g_new(ContingItemBranchKey, 1);

	key->x = MIN(x, y);
	key->y = MAX(x, y);

	return key;
}

static gint
conting_item_branch_key_cmp(const ContingItemBranchKey *k1,
	   const ContingItemBranchKey *k2)
{
	return (k1->x - k2->x) || (k1->y - k2->y);
}

static guint
conting_item_branch_key_hash(gconstpointer data)
{
	const ContingItemBranchKey *k = data;

	return 31 * k->x + (k->y << 3);
}

static gboolean
conting_item_branch_key_equals(gconstpointer a, gconstpointer b)
{
	const ContingItemBranchKey *k1 = a;
	const ContingItemBranchKey *k2 = b;

	return conting_item_branch_key_cmp(k1, k2) == 0;
}
ContingItemData *
conting_model_get_bus(ContingModel *self, gint num)
{
	ContingModelPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_MODEL(self), NULL);

	priv = CONTING_MODEL_GET_PRIVATE(self);


	return conting_model_get_item(self, CONTING_ITEM_TYPE_BUS,
			GINT_TO_POINTER(num));
}

ContingItemData *
conting_model_get_branch(ContingModel *self, gint x, gint y)
{
	ContingModelPrivate *priv;

	g_return_val_if_fail(self != NULL && CONTING_IS_MODEL(self), NULL);

	priv = CONTING_MODEL_GET_PRIVATE(self);

	return conting_model_get_item(self, CONTING_ITEM_TYPE_BRANCH,
			conting_item_branch_key_new(x, y));
}

static gpointer
conting_item_get_bus_key(ContingItemData *item)
{
	return GINT_TO_POINTER(conting_item_data_bus_number(item));
}

static gpointer
conting_item_get_branch_key(ContingItemData *item)
{
	gint x, y;

	conting_item_data_get_attr(item,
			"tap bus number", &x,
			"z bus number", &y,
			NULL);

	return conting_item_branch_key_new(x, y);
}

ContingModel *
conting_model_new_default(void) 
{
	ContingModel *self = g_object_new(CONTING_TYPE_MODEL, NULL);

	conting_model_add_setup(self, CONTING_ITEM_TYPE_BUS,
			conting_item_get_bus_key, NULL,
			NULL, NULL);

	conting_model_add_setup(self, CONTING_ITEM_TYPE_BRANCH,
			conting_item_get_branch_key, g_free,
			conting_item_branch_key_hash, conting_item_branch_key_equals);


	return self;
}
static gpointer
conting_item_get_flow_branch_key(ContingItemData *item)
{
	gint x, y;

	conting_item_data_get_attr(item,
			"start-1", &x,
			"end-1", &y,
			NULL);

	return conting_item_branch_key_new(x, y);
}
static gpointer
conting_item_get_flow_trans_key(ContingItemData *item)
{
	gint x, y;

	conting_item_data_get_attr(item,
			"start", &x,
			"end", &y,
			NULL);

	return conting_item_branch_key_new(x, y);
}

ContingModel *
conting_model_new_load_flow(void)
{
	ContingModel *self = g_object_new(CONTING_TYPE_MODEL, NULL);
	conting_model_add_setup(self, CONTING_ITEM_TYPE_FLOW_BUS,
			conting_item_get_bus_key, NULL,
			NULL, NULL);

	conting_model_add_setup(self, CONTING_ITEM_TYPE_FLOW_BRANCH,
			conting_item_get_flow_branch_key, g_free,
			conting_item_branch_key_hash, conting_item_branch_key_equals);


	conting_model_add_setup(self, CONTING_ITEM_TYPE_FLOW_TRANS,
			conting_item_get_flow_trans_key, g_free,
			conting_item_branch_key_hash, conting_item_branch_key_equals);

	return self;
}
