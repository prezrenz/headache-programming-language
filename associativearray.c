#include "associativearray.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

assoc_array* assoc_array_new() {
    assoc_array* new_assoc_array = malloc(sizeof(assoc_array));
    
    if(new_assoc_array == NULL) {
        return NULL;
    }

    new_assoc_array->size = 0;

    return new_assoc_array;
}

int assoc_array_free(assoc_array* list) {
    while (list->size > 0) {
        assoc_array_delete(list, list->head);
    }

    free(list);

    return 0;
}

int assoc_array_append(assoc_array* list, char* key, char* value) {
    struct kv_node* found_node = assoc_array_get(list, key);
    if(found_node != NULL) {
        return 1; // key already exists
    }

    struct kv_node* new_node = malloc(sizeof(struct kv_node));
    
    if(new_node == NULL) {
        return 2; // is this even possible?
    }

    new_node->key = strdup(key);
    new_node->next = NULL;
    new_node->prev = list->end;
    new_node->value = strdup(value);

    if(list->size == 0) {
        list->head = new_node;
        list->end = new_node;
        list->size += 1;
        return 0;
    }

    list->size += 1;
    list->end->next = new_node;
    list->end = new_node;

    return 0;
}

struct kv_node* assoc_array_get(assoc_array* list, char* key) {
    if(list->size == 0) {
        return NULL; // should report list is empty
    }

    struct kv_node* current_node;
    current_node = list->head;
    while(current_node != NULL) {
        if(strcmp(current_node->key, key) == 0) {
            break;
        }

        current_node = current_node->next;
    }

    if(current_node == NULL) {
        return NULL; // should report data not found
    }

    return current_node;
}

int assoc_array_delete(assoc_array* list, struct kv_node* found_node) {
    if(list->size == 1) {
        list->head = NULL;
        list->end = NULL;
        list->size = 0;
        free(found_node);
    } else if(found_node == list->head) {
        list->head = found_node->next; 
        list->size -= 1;
        free(found_node);
    } else if(found_node == list->end) {
        list->end = found_node->prev;
        list->size -= 1;
        free(found_node);
    } else {
        found_node->prev->next = found_node->next;
        found_node->next->prev = found_node->prev;
        list->size -= 1;
        free(found_node);
    }

    return 0;
}

int assoc_array_delete_key(assoc_array* list, char* key) {
    if(list->size == 0) {
        return 1; // should report list empty
    }
    
    struct kv_node* found_node = assoc_array_get(list, key); 
    if(found_node == NULL) {
        return 2; // should report failed
    }

    return assoc_array_delete(list, found_node);
}

bool assoc_array_is_empty(assoc_array* list) {
    return list->size == 0;
}

int assoc_array_empty(assoc_array* list) {
    if(assoc_array_is_empty(list)) {
        return 0; // already empty
    }

    int status;
    while(!assoc_array_is_empty(list)) {
        status = assoc_array_delete(list, list->head);
    }

    return status;
}

void node_print(struct kv_node* n) {
    if(n == NULL) printf("HUH!?");

    printf("\nKEY: %s\n", n->key);
    printf("VALUE: %s\n", n->value);
    printf("----------------\n");
}

void assoc_array_print(assoc_array* list) {
    printf("\nList Size: %d\n", list->size);
    printf("================");

    if(list == NULL) {
        printf("\nCANNOT PRINT, LIST IS NULL!");
        return;
    }

    if(list->size == 0) {
        printf("\nList is Empty!\n\n");
        return;
    }

    struct kv_node* current_node = list->head;
    while(current_node != NULL) {
        node_print(current_node);
        current_node = current_node->next;
    }
}

// DEBUG ONLY, exits without cleaning is bad
void assoc_array_test() {
    printf("* BEGINNING LIST TEST\n\n");

    printf("** CREATING NEW LIST\n");
    assoc_array* test_list = assoc_array_new();
    if(test_list == NULL) {
        fprintf(stderr, "Error: could not make new Array List\n");
        exit(1);
    }

    printf("*** Successfully made new list\n");
    assoc_array_print(test_list);

    int status = 0;

    // Testing Append
    printf("** APPENDING TO LIST\n");
    status = assoc_array_append(test_list, "DATA1", "DATA1");
    if(status > 0) {
        fprintf(stderr, "Error: append failed, status %d\n", status);
        exit(1);
    }
    assoc_array_print(test_list);
 
    printf("** APPENDING TO LIST\n");
    status = assoc_array_append(test_list, "DATA2", "DATA2");
    if(status > 0) {
        fprintf(stderr, "Error: append failed, status %d\n", status);
        exit(1);
    }
    assoc_array_print(test_list);
    
    printf("** APPENDING TO LIST\n");
    status = assoc_array_append(test_list, "DATA3", "DATA3");
    if(status > 0) {
        fprintf(stderr, "Error: append failed, status %d\n", status);
        exit(1);
    }
    assoc_array_print(test_list);

    // Testing Get
    printf("** GETTING DATA1\n");
    struct kv_node* n = assoc_array_get(test_list, 0);
    if(n == NULL) {
        fprintf(stderr, "Error: get failed\n");
        exit(1);
    }
    node_print(n);

    printf("** GETTING DATA2\n");
    n = assoc_array_get(test_list, "DATA2");
    if(n == NULL) {
        fprintf(stderr, "Error: get failed\n");
        exit(1);
    }
    node_print(n);

    printf("** GETTING DATA3\n");
    
    n = assoc_array_get(test_list, "DATA3");
    if(n == NULL) {
        fprintf(stderr, "Error: get failed\n");
        exit(1);
    }
    node_print(n);
    
    // Testing Delete
    
    printf("** DELETING DATA2 BY ID\n");
    status = assoc_array_delete_key(test_list, "DATA2");
    if(status > 0) {
        fprintf(stderr, "Error: failed to delete, status %d\n", status);
        exit(1);
    }
    assoc_array_print(test_list);

    printf("** DELETING DATA3 BY VALUE\n");
    status = assoc_array_delete_key(test_list, "DATA3");
    if(status > 0) {
        fprintf(stderr, "Error: failed to delete, status %d\n", status);
        exit(1);
    }
    assoc_array_print(test_list);

    // Testing Empty

    printf("** EMPTYING LIST\n");
    status = assoc_array_empty(test_list);
    if(status > 0) {        
        fprintf(stderr, "Error: failed to empty, status %d\n", status);
        exit(1);
    }
    assoc_array_print(test_list);

    // Testing Free

    printf("** FREEING TEST LIST\n");
    status = assoc_array_free(test_list);
    if(status > 0) {
        fprintf(stderr, "Error: failed to free, status %d\n", status);
        exit(1);
    }

    printf("SUCCESSFULLY FINISHED ARRAY LIST TESTS: NO ERRORS\n");
}
