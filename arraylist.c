#include "arraylist.h"
#include <stdlib.h>

array_list* array_list_new() {
    array_list* new_array_list = malloc(sizeof(array_list));
    
    if(new_array_list == NULL) {
        return NULL;
    }

    new_array_list->size = 0;

    return new_array_list;
}

void array_list_free(array_list* list) {
    while (list->size > 0) {
        array_list_delete(list, list->head);
    }

    free(list);
}

void array_list_insert(array_list *list, char *data, int pos) {
    if(pos > list->size || pos < 0) {
        return; // should report error
    }

    struct node* new_node = malloc(sizeof(struct node));
    new_node->id = list->size;
    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->value = data;

    struct node* insert_after = array_list_get(list, pos);
    if(insert_after == NULL) {
        return; // should report error
    }

    new_node->next = insert_after->next;
    new_node->prev = insert_after;
    insert_after->next->prev = new_node;
    insert_after->next = new_node;
}

void array_list_append(array_list *list, char *data) {
    struct node* new_node = malloc(sizeof(struct node));
    new_node->id = list->size;
    new_node->next = NULL;
    new_node->prev = list->end;
    new_node->value = data;

    list->size += 1;
    list->end = new_node;
}

struct node* array_list_find(array_list* list, char* data) {
    if(list->size == 0) {
        return NULL; // should report list is empty
    }

    struct node* found_node;
    struct node* current_node;
    current_node = list->head;
    while(current_node->next != NULL) {
        if(current_node->value == data) {
            found_node = current_node;
            break;
        }

        current_node = current_node->next;
    }

    if(found_node == NULL) {
        return NULL; // should report data not found
    }

    return found_node;
}

struct node* array_list_get(array_list* list, int id) {
    if(list->size == 0) {
        return NULL; // should report list is empty
    }

    struct node* found_node;
    struct node* current_node;
    current_node = list->head;
    while(current_node->next != NULL) {
        if(current_node->id == id) {
            found_node = current_node;
            break;
        }

        current_node = current_node->next;
    }

    if(found_node == NULL) {
        return NULL; // should report data not found
    }

    return found_node;
}

void array_list_delete(array_list* list, struct node* found_node) {
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
        list->size -= 1;
        free(found_node);
    }
}

void array_list_delete_data(array_list* list, char* data) {
    if(list->size == 0) {
        return; // should report list empty
    }

    struct node* found_node = array_list_find(list, data); 
    if(found_node == NULL) {
        return; // should report failed
    }

    array_list_delete(list, found_node);
}

void array_list_delete_id(array_list* list, int id) {
    if(list->size == 0) {
        return; // should report list empty
    }
    
    struct node* found_node = array_list_get(list, id); 
    if(found_node == NULL) {
        return; // should report failed
    }

    array_list_delete(list, found_node);
}
