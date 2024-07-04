#ifndef ASSOCIATIVE_ARRAY_H
#define ASSOCIATIVE_ARRAY_H

#include <stdlib.h>
#include <stdbool.h>

#include "commons.h"

struct kv_node {
    struct kv_node* next;
    struct kv_node* prev;
    object* value; // TODO: Replace value with symbol struct
    char* key;
};

typedef struct {
    int size;
    struct kv_node* head;
    struct kv_node* end;
} assoc_array;

assoc_array* assoc_array_new();
int assoc_array_free(assoc_array* list);

int assoc_array_append(assoc_array* list, char* key, object* value);

object* assoc_array_get_value(assoc_array* list, char* key);
struct kv_node* assoc_array_get(assoc_array* list, char* key);

int assoc_array_delete(assoc_array* list, struct kv_node* found_node);
int assoc_array_delete_key(assoc_array* list, char* key);

bool assoc_array_is_empty(assoc_array* list);
int assoc_array_empty(assoc_array* list);

/* void kv_node_print(struct kv_node* n); */
/* void assoc_array_print(assoc_array* list); */
/* void assoc_array_test(); */

#endif
