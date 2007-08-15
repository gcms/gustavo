#include "list.h"

#include <stdlib.h>

struct node {
    void *data;
    struct node *next;
    struct node *prev;
};

static struct node *new_node(void *data) {
    struct node *n = malloc(sizeof(struct node));
    
    if (n == NULL) {
        return NULL;
    }

    n->data = data;
    n->next = n;
    n->prev = n;

    return n;
}

static bool add_prev(struct node *n, void *data) {
    struct node *new = new_node(data);
    if (new == NULL) {
        return false;
    }

    new->next = n;
    new->prev = n->prev;

    n->prev->next = new;
    n->prev = new;

    return true;
}

#define LIST_TYPE_ID 3322014

typedef struct {
    int type_id;
    comparator cmp;
    int size;
    struct node *head;
} list_t;

void *list_new(comparator cmp) {
    list_t *l = malloc(sizeof(list_t));
    
    if (l == NULL) {
        return NULL;
    }

    l->type_id = LIST_TYPE_ID;
    l->cmp = cmp;

    l->size = 0;
    l->head = new_node(NULL);

    if (l->head == NULL) {
        free(l);
        return NULL;
    }

    return l;
}

bool list_append(void *l, void *data) {
    list_t *this = l;

    my_assert(this->type_id == LIST_TYPE_ID, "This is not a List Pointer");

    if (data == NULL) {
        return false;
    }

    if (add_prev(((list_t *) l)->head, data)) {
        ((list_t *) l)->size++;
        return true;
    }

    return false;
}

static struct node *find_node(struct node *head, int index) {
    struct node *p = head;

    do {
        p = p->next;
        if (p == head) {
            return NULL;
        }
    } while (index-- > 0);

    return p;
}

bool list_add(void *l, int index, void *data) {
    list_t *this = l;
    struct node *n;
    
    my_assert(this->type_id == LIST_TYPE_ID, "This is not a List Pointer");

    if (index > ((list_t *) l)->size) {
        return false;
    } else if (index == ((list_t *) l)->size) {
        return list_append(l, data);
    }

    if (data == NULL) {
        return false;
    }

    n = find_node(((list_t *) l)->head, index);
    if (n == NULL) {
        return false;
    }

    if (add_prev(n, data)) {
        ((list_t *) l)->size++;
        return true;
    }

    return false;
}

void list_clear(void *l, bool free_data) {
    list_t *this = l;
    struct node *p;

    my_assert(this->type_id == LIST_TYPE_ID, "This is not a List Pointer");

    p = this->head->next;

    while (p != this->head) {
        struct node *tmp = p;

        p = p->next;
        if (free_data) {
            free(tmp->data);
        }
        free(tmp);
    }

    this->size = 0;
    this->head->next = this->head->prev = this->head;
}


static struct node *find_node_by_data(struct node *head, void *data,
        comparator c) {
    struct node *p = head->next;

    while (p != head) {
        if (p->data == data || (c != NULL && c(p->data, data) == 0)) {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

bool list_contains(void *l, void *data) {
    list_t *this = l;
    my_assert(this->type_id == LIST_TYPE_ID, "This is not a List Pointer");

    return find_node_by_data(this->head, data, this->cmp) != NULL;
}

void *list_get(void *l, int index) {
    list_t *this = l;
    struct node *n;

    my_assert(this->type_id == LIST_TYPE_ID, "This is not a List Pointer");

    n = find_node(this->head, index);

    return n == NULL ? n : n->data;
}

void list_set_comparator(void *l, comparator cmp) {
    list_t *this = l;
    my_assert(this->type_id == LIST_TYPE_ID, "This is not a List Pointer");
    this->cmp = cmp;
}

int list_size(void *l) {
    list_t *this = l;
    my_assert(this->type_id == LIST_TYPE_ID, "This is not a List Pointer");

    return this->size;
}

bool list_remove(void *l, void *data, bool free_data) {
    list_t *this = l;
    struct node *n;

    my_assert(this->type_id == LIST_TYPE_ID, "This is not a List Pointer");

    n = find_node_by_data(this->head, data, this->cmp);
    if (n == NULL) {
        return false;
    }

    if (free_data) {
        free(n->data);
    }

    n->prev->next = n->next;
    n->next->prev = n->prev;
    
    free(n);

    return true;
}

void *list_remove_by_index(void *l, int index) {
    list_t *this = l;
    void *data;
    struct node *n;

    my_assert(this->type_id == LIST_TYPE_ID, "This is not a List Pointer");

    n = find_node(l, index);

    if (n == NULL) {
        return NULL;
    }

    data = n->data;

    n->next->prev = n->prev;
    n->prev->next = n->next;

    free(n);

    return data;
}

void list_delete(void *l, bool free_data) {
    list_t *this = l;
    my_assert(this->type_id == LIST_TYPE_ID, "This is not a List Pointer");
    list_clear(l, free_data);
    free(this->head);
    free(this);
}

void *list_set(void *l, int index, void *data) {
    list_t *this = l;
    struct node *n;

    my_assert(this->type_id == LIST_TYPE_ID, "This is not a List Pointer");

    n = find_node(this->head, index);
    if (n == NULL) {
        return NULL;
    } else {
        void *result = n->data;
        n->data = data;
        return result;
    }
}

void list_iterate(void *l, list_handler hdl, void *data) {
    list_t *this = l;
    struct node *p;
    int i;

    my_assert(this->type_id == LIST_TYPE_ID, "This is not a List Pointer");

    for (p = this->head, i = 0; p != NULL; p = p->next, i++) {
        hdl(p->data, i, data);
    }
}

#include "iterator.h"

#define LIST_ITERATOR_TYPE_ID 321431
typedef struct {
    iterator_t it;

    int type_id;

    struct node *head;
    struct node *e;
} list_iterator_t;

static void list_iterator_next(void *it) {
    list_iterator_t *this = it;

    my_assert(this->type_id == LIST_ITERATOR_TYPE_ID,
            "This is not a List Iterator");

    if (this->e != this->head) {
        this->e = this->e->next;
    }
}

static void *list_iterator_get(void *it) {
    list_iterator_t *this = it;

    my_assert(this->type_id == LIST_ITERATOR_TYPE_ID,
            "This is not a List Iterator");

    return this->e != this->head ? this->e->data : NULL;
}

static bool list_iterator_has_more(void *it) {
    list_iterator_t *this = it;

    my_assert(this->type_id == LIST_ITERATOR_TYPE_ID,
            "This is not a List Iterator");

    return this->e != this->head;
}

void *list_iterator(void *l) {
    list_t *this = l;
    list_iterator_t *it;

    my_assert(this->type_id == LIST_TYPE_ID, "This is not a List Pointer");

    
    it = malloc(sizeof(list_iterator_t));

    it->it.next = list_iterator_next;
    it->it.get = list_iterator_get;
    it->it.has_more = list_iterator_has_more;

    it->type_id = LIST_ITERATOR_TYPE_ID;

    it->e = this->head->next;
    it->head = this->head;

    return it;
}
