#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>


int LLMemSize;
#define LL_MEM() printf("[+] memory allocated: %d bytes\n", LLMemSize)


typedef struct {
    void *data, *prev, *next;
} LLNode;

typedef struct {
    LLNode *keys, *values;
    int length;
} Dict;


LLNode *ll_create(void *data);
LLNode *ll_head(LLNode *list);
LLNode *ll_tail(LLNode *list);
void ll_free_node(LLNode *node);
void ll_free(LLNode *list);
LLNode *ll_append(LLNode *list, void *data);
LLNode *ll_insert_before(LLNode *here, void *data);
LLNode *ll_insert_after(LLNode *here, void *data);
bool ll_insert_at(LLNode *list, unsigned int index, void *data);
LLNode *ll_get_node_at(LLNode *list, unsigned int index);
int ll_length(LLNode *list);
void ll_foreach(LLNode *node, void *fn, void *meta);
LLNode *ll_map(LLNode *list, void *fn, void *meta);
LLNode *ll_filter(LLNode *list, void *fn, void *meta);

void print_data (void *data, void *argument, int index);
void *map_oddeven (void *data, void *argument, int index);
bool filter_divisible (void *data, void *argument, int index);
bool filter_lessthan (void *data, void *argument, int index);

uint64_t time_microsec();

Dict *dict_create ();
int dict_indexof (Dict *dict, void *key);
void dict_set (Dict *dict, void *key, void *value);
void dict_unset (Dict *dict, void *key);
void *dict_get (Dict *dict, void *key);

#endif