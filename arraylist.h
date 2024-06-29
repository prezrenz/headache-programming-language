#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdlib.h>

struct node {
    struct node* next;
    struct node* prev;
    char* value;
    int id;
};

typedef struct {
    int size;
    struct node* head;
    struct node* end;
} array_list;

array_list* array_list_new();
int array_list_free(array_list* list);

// void array_list_resize(array_list* list);

int array_list_insert(array_list* list, char* data, int pos);
int array_list_append(array_list* list, char* data);

struct node* array_list_find(array_list* list, char* data);
struct node* array_list_get(array_list* list, int id);

int array_list_delete(array_list* list, struct node* found_node);
int array_list_delete_data(array_list* list, char* data);
int array_list_delete_id(array_list* list, int id);

void node_print(struct node* n);
void array_list_print(array_list* list);
void array_list_test();

#endif
