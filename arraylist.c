#include "arraylist.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

array_list* array_list_new() {
    array_list* new_array_list = malloc(sizeof(array_list));
    
    if(new_array_list == NULL) {
        return NULL;
    }

    new_array_list->size = 0;

    return new_array_list;
}

int array_list_free(array_list* list) {
    while (list->size > 0) {
        array_list_delete(list, list->head);
    }

    free(list);
    list = NULL;

    return 0;
}

int array_list_insert(array_list *list, char *data, int pos) {
    if(pos > list->size || pos < 0) {
        return 1; // should report error
    }

    if(pos == list->size-1 || pos == 0) { // if inserting at end just append or list empty
        return array_list_append(list, data);
    }

    struct node* new_node = malloc(sizeof(struct node));
    new_node->id = pos+1;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->value = malloc(strlen(data)+1);
    strcpy(new_node->value, data);

    struct node* insert_after = array_list_get(list, pos);
    if(insert_after == NULL) {
        return 2; // should report error, but should not be possible
    }

    new_node->next = insert_after->next;
    new_node->prev = insert_after;
    insert_after->next->prev = new_node;
    insert_after->next = new_node;
     
    struct node* current_node;
    current_node = new_node->next;
    while(current_node != NULL) {
        current_node->id += 1;
        current_node = current_node->next;
    }

    list->size += 1;

    return 0;
}

int array_list_append(array_list *list, char *data) {
    struct node* new_node = malloc(sizeof(struct node));
    
    if(new_node == NULL) {
        return 1; // is this even possible?
    }

    new_node->id = list->size;
    new_node->next = NULL;
    new_node->prev = list->end;
    new_node->value = strdup(data);

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

struct node* array_list_find(array_list* list, char* data) {
    if(list->size == 0) {
        return NULL; // should report list is empty
    }

    struct node* current_node;
    current_node = list->head;
    while(current_node != NULL) {
        if(strcmp(current_node->value, data) == 0) {
            break;
        }

        current_node = current_node->next;
    }

    if(current_node == NULL) {
        return NULL; // should report data not found
    }

    return current_node;
}

struct node* array_list_get(array_list* list, int id) {
    if(list->size == 0) {
        return NULL; // should report list is empty
    }

    struct node* current_node;
    current_node = list->head;
    while(current_node != NULL) {
        if(current_node->id == id) {
            break;
        }

        current_node = current_node->next;
    }

    if(current_node == NULL) {
        return NULL; // should report data not found
    }

    return current_node;
}

int array_list_delete(array_list* list, struct node* found_node) {
    if(list->size == 1) {
        list->head = NULL;
        list->end = NULL;
        list->size = 0;
        free(found_node);
        found_node = NULL;
    } else if(found_node == list->head) {
        list->head = found_node->next;
        
        struct node* current_node;
        current_node = list->head;
        while(current_node != NULL) {
            current_node->id -= 1;
            current_node = current_node->next;
        }

        list->size -= 1;
        free(found_node);
        found_node = NULL;
    } else if(found_node == list->end) {
        list->end = found_node->prev;
        list->end->next = NULL;
        list->size -= 1;
        free(found_node);
        found_node = NULL;
    } else {
        found_node->prev->next = found_node->next;
        found_node->next->prev = found_node->prev;

        struct node* current_node;
        current_node = found_node->next;
        while(current_node != NULL) {
            current_node->id -= 1;
            current_node = current_node->next;
        }

        list->size -= 1;
        free(found_node);
        found_node = NULL;
    }

    return 0;
}

int array_list_delete_data(array_list* list, char* data) {
    if(list->size == 0) {
        return 1; // should report list empty
    }

    struct node* found_node = array_list_find(list, data); 
    if(found_node == NULL) {
        return 2; // should report failed
    }

    return array_list_delete(list, found_node);
}

int array_list_delete_id(array_list* list, int id) {
    if(list->size == 0) {
        return 1; // should report list empty
    }
    
    struct node* found_node = array_list_get(list, id); 
    if(found_node == NULL) {
        return 2; // should report failed
    }

    return array_list_delete(list, found_node);
}

bool array_list_is_empty(array_list* list) {
    return list->size == 0;
}

int array_list_empty(array_list* list) {
    if(array_list_is_empty(list)) {
        return 0; // already empty
    }

    int status;
    while(!array_list_is_empty(list)) {
        status = array_list_delete_id(list, 0);
    }

    return status;
}

void node_print(struct node* n) {
    if(n == NULL) printf("HUH!?");

    printf("\nPOS: %d\n", n->id);
    printf("DATA: %s\n", n->value);
    printf("----------------\n");
}

void array_list_print(array_list* list) {
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

    struct node* current_node = list->head;
    while(current_node != NULL) {
        node_print(current_node);
        current_node = current_node->next;
    }
}

// DEBUG ONLY, exits without cleaning is bad
void array_list_test() {
    printf("* BEGINNING LIST TEST\n\n");

    printf("** CREATING NEW LIST\n");
    array_list* test_list = array_list_new();
    if(test_list == NULL) {
        fprintf(stderr, "Error: could not make new Array List\n");
        exit(1);
    }

    printf("*** Successfully made new list\n");
    array_list_print(test_list);

    int status = 0;

    // Testing Append
    printf("** APPENDING TO LIST\n");
    status = array_list_append(test_list, "DATA1");
    if(status > 0) {
        fprintf(stderr, "Error: append failed, status %d\n", status);
        exit(1);
    }
    array_list_print(test_list);
 
    printf("** APPENDING TO LIST\n");
    status = array_list_append(test_list, "DATA2");
    if(status > 0) {
        fprintf(stderr, "Error: append failed, status %d\n", status);
        exit(1);
    }
    array_list_print(test_list);
    
    printf("** APPENDING TO LIST\n");
    status = array_list_append(test_list, "DATA3");
    if(status > 0) {
        fprintf(stderr, "Error: append failed, status %d\n", status);
        exit(1);
    }
    array_list_print(test_list);

    // Testing Get
    printf("** GETTING DATA1\n");
    struct node* n = array_list_get(test_list, 0);
    if(n == NULL) {
        fprintf(stderr, "Error: get failed\n");
        exit(1);
    }
    node_print(n);

    printf("** GETTING DATA2\n");
    n = array_list_get(test_list, 1);
    if(n == NULL) {
        fprintf(stderr, "Error: get failed\n");
        exit(1);
    }
    node_print(n);

    printf("** GETTING DATA3\n");
    
    n = array_list_get(test_list, 2);
    if(n == NULL) {
        fprintf(stderr, "Error: get failed\n");
        exit(1);
    }
    node_print(n);
    
    // Testing Find
    
    printf("** FINDING DATA1\n");
    n = array_list_find(test_list, "DATA1");
    if(n == NULL) {
        fprintf(stderr, "Error: get failed\n");
        exit(1);
    }
    node_print(n);

    printf("** FINDING DATA2\n");
    n = array_list_find(test_list, "DATA2");
    if(n == NULL) {
        fprintf(stderr, "Error: get failed\n");
        exit(1);
    }
    node_print(n);

    printf("** FINDING DATA3\n");
    n = array_list_find(test_list, "DATA3");
    if(n == NULL) {
        fprintf(stderr, "Error: get failed\n");
        exit(1);
    }
    node_print(n);

    // Testing Insert

    printf("** INSERTING DATA4 AFTER DATA 2\n");
    status = array_list_insert(test_list, "DATA4", 1);
    if(status > 0) {
        fprintf(stderr, "Error: failed to insert, status %d\n", status);
        exit(1);
    }
    array_list_print(test_list);
    
    printf("** INSERTING DATA5 AFTER DATA 3\n");
    status = array_list_insert(test_list, "DATA5", 3);
    if(status > 0) {
        fprintf(stderr, "Error: failed to insert, status %d\n", status);
        exit(1);
    }
    array_list_print(test_list);

    // Testing Delete
    
    printf("** DELETING DATA4 BY ID\n");
    status = array_list_delete_id(test_list, 2);
    if(status > 0) {
        fprintf(stderr, "Error: failed to delete, status %d\n", status);
        exit(1);
    }
    array_list_print(test_list);

    printf("** DELETING DATA3 BY VALUE\n");
    status = array_list_delete_data(test_list, "DATA3");
    if(status > 0) {
        fprintf(stderr, "Error: failed to delete, status %d\n", status);
        exit(1);
    }
    array_list_print(test_list);

    // Testing Empty

    printf("** EMPTYING LIST\n");
    status = array_list_empty(test_list);
    if(status > 0) {        
        fprintf(stderr, "Error: failed to empty, status %d\n", status);
        exit(1);
    }
    array_list_print(test_list);

    // Testing Free

    printf("** FREEING TEST LIST\n");
    status = array_list_free(test_list);
    if(status > 0) {
        fprintf(stderr, "Error: failed to free, status %d\n", status);
        exit(1);
    }

    printf("SUCCESSFULLY FINISHED ARRAY LIST TESTS: NO ERRORS\n");
}
