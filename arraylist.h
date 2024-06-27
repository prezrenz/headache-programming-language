#ifndef ARRAYLIST_H
#define ARRAYLIST_H

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
void array_list_free(array_list* list);

void array_list_resize(array_list* list);

void array_list_insert(array_list* list, char* data, int pos);
void array_list_append(array_list* list, char* data);
char* array_list_find(array_list* list, char* data);
char* array_list_get(array_list* list, int id);
void array_list_delete(array_list*, char* data);

#endif
