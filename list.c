#include "list.h"

int LLMemSize = 0;
#define LL_MEM() printf("[+] memory allocated: %d bytes\n", LLMemSize)



LLNode *ll_create(void *data) {
    LLNode *node = (LLNode *)malloc(sizeof(LLNode));
    node->data = data;
    node->prev = node->next = NULL;
    LLMemSize += sizeof(LLNode);
    return node;
}


LLNode *ll_head(LLNode *list) {
    LLNode *curr = list;
    while (curr->prev != NULL) curr = (LLNode *)curr->prev;
    return curr;
}


LLNode *ll_tail(LLNode *list) {
    LLNode *curr = list;
    while (curr->next != NULL) curr = (LLNode *)curr->next;
    return curr;
}


void ll_free_node(LLNode *node) {
    if (node == NULL) return;
    free(node);
    LLMemSize -= sizeof(LLNode);
}


void ll_free(LLNode *list) {
    if (list == NULL) return;
    LLNode *curr = ll_head(list);
    do {
        LLNode *temp = (LLNode *)curr->next;
        free(curr);
        LLMemSize -= sizeof(LLNode);
        curr = temp;
    } while (curr != NULL);
}


LLNode *ll_append(LLNode *list, void *data) {
    LLNode *next = ll_create(data);
    LLNode *curr = ll_tail(list);
    curr->next = next;
    next->prev = curr;
    return next;
}


LLNode *ll_insert_before(LLNode *here, void *data) {
    LLNode *node = ll_create(data);
    if (here->prev != NULL) {
        ((LLNode *)here->prev)->next = node;
        node->prev = here->prev;
    }
    here->prev = node;
    node->next = here;
    return node;
}


LLNode *ll_insert_after(LLNode *here, void *data) {
    LLNode *node = ll_create(data);
    if (here->next != NULL) {
        ((LLNode *)here->next)->prev = node;
        node->next = here->next;
    }
    here->next = node;
    node->prev = here;
    return node;
}


bool ll_insert_at(LLNode *list, unsigned int index, void *data) {
    LLNode *node = ll_create(data);
    LLNode *curr = ll_head(list), *last;
    unsigned int count = 0;
    while (count != index && curr != NULL) {
        last = curr;
        curr = (LLNode *)curr->next;
        count++;
    }
    if (curr == NULL) {
        if (count == index) {
            last->next = node;
            node->prev = last;
            return true;
        }
        return false;
    }
    if (count > 0) {
        ((LLNode *)curr->prev)->next = node;
        node->prev = curr->prev;
    }
    node->next = curr;
    curr->prev = node;
    return true;
}


LLNode *ll_get_node_at(LLNode *list, unsigned int index) {
    LLNode *curr = ll_head(list);
    unsigned int count = 0;
    while (count != index && curr != NULL) {
        curr = (LLNode *)curr->next;
        count++;
    }
    return curr;
}


int ll_length(LLNode *list) {
    LLNode *curr = ll_head(list);
    int count = 0;
    while (curr != NULL) {
        curr = (LLNode *)curr->next;
        count++;
    }
    return count;
}


void ll_foreach(LLNode *node, void *fn, void *meta) {
    LLNode *curr = ll_head(node);
    void (*fn_ptr)(void *, void *, int) = fn;
    int index = 0;
    do {
        fn_ptr(curr->data, meta, index);
        curr = (LLNode *)curr->next;
        index++;
    } while (curr != NULL);
}


LLNode *ll_map(LLNode *list, void *fn, void *meta) {
    LLNode *curr = ll_head(list);
    LLNode *mapped = NULL, *mapped_head;
    void *(*fn_ptr)(void *, void *, int) = fn;
    int index = 0;
    do {
        if (mapped == NULL) {
            mapped = ll_create(fn_ptr(curr->data, meta, index));
            mapped_head = mapped;
        } else {
            mapped = ll_append(mapped, fn_ptr(curr->data, meta, index));
        }
        curr = (LLNode *)curr->next;
        index++;
    } while (curr != NULL);
    return mapped_head;
}


LLNode *ll_filter(LLNode *list, void *fn, void *meta) {
    LLNode *curr = ll_head(list);
    LLNode *filtered = NULL, *filtered_head = NULL;
    bool (*fn_ptr)(void *, void *, int) = fn;
    int index = 0;
    do {
        bool result = fn_ptr(curr->data, meta, index);
        if (result) {
            if (filtered == NULL) {
                filtered = ll_create(curr->data);
                filtered_head = filtered;
            } else {
                filtered = ll_append(filtered, curr->data);
            }
        }
        curr = (LLNode *)curr->next;
        index++;
    } while (curr != NULL);
    return filtered_head;
}


void print_data (void *data, void *argument, int index) {
    printf((char *)argument, (uint64_t)data);
}

void *map_oddeven (void *data, void *argument, int index) {
    return (void *)((uint64_t)data % 2);
}

bool filter_divisible (void *data, void *argument, int index) {
    return (uint64_t)data > 0 && (uint64_t)data % (uint64_t)argument == 0;
}

bool filter_lessthan (void *data, void *argument, int index) {
    return (uint64_t)data < (uint64_t)argument;
}



uint64_t time_microsec() {
    struct timeval tv;
    uint64_t usec;
    gettimeofday(&tv, NULL);
    usec = tv.tv_sec;
    usec *= 1000000;
    usec += tv.tv_usec;
    return usec;
}




Dict *dict_create () {
    Dict *dict = (Dict *)malloc(sizeof(Dict));
    dict->length = 0;
    return dict;
}


int dict_indexof (Dict *dict, void *key) {
    if (dict->length == 0) return -1;
    LLNode *curr = dict->keys;
    unsigned int count = 0;
    while (curr != NULL) {
        if (key == curr->data) return count;
        curr = (LLNode *)curr->next;
        count++;
    }
    return -1;    
}

void dict_set (Dict *dict, void *key, void *value) {
    if (dict->length == 0) {
        dict->keys = ll_create(key);
        dict->values = ll_create(value);
        dict->length++;
        return;
    }
    int index = dict_indexof(dict, key);
    if (index == -1) {
        ll_append(dict->keys, key);
        ll_append(dict->values, value);
        dict->length++;
    } else {
        LLNode *node = ll_get_node_at(dict->values, index);
        node->data = value;
    }
}

void dict_unset (Dict *dict, void *key) {
    int index = dict_indexof(dict, key);
    if (index == -1) return;
    LLNode *k = ll_get_node_at(dict->keys, index);
    LLNode *v = ll_get_node_at(dict->values, index);
    if (k->prev != NULL) ((LLNode *)k->prev)->next = k->next;
    if (k->next != NULL) ((LLNode *)k->next)->prev = k->prev;
    if (v->prev != NULL) ((LLNode *)v->prev)->next = v->next;
    if (v->next != NULL) ((LLNode *)v->next)->prev = v->prev;
    ll_free_node(k);
    ll_free_node(v);
}

void *dict_get (Dict *dict, void *key) {
    int index = dict_indexof(dict, key);
    if (index == -1) return NULL;
    return ll_get_node_at(dict->values, index)->data;
}


