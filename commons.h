#ifndef COMMONS_H
#define COMMONS_H

typedef enum{
    NUMBER,
    LAMBDA,
    ARRAY,
    SYMBOL,
    EMPTY_LIST,
    PAIR
} object_type;

typedef struct object {
    object_type type;
    
    union {
        struct {
            unsigned char value;
        } number;
        
        struct  {
            int curr_index;
            unsigned char arr[256];
        } array;

        struct {
            char* value;
        } symbol;

        struct {
            struct object* left;
            struct object* right;
        } pair;
    
    } data; 

} object;

#endif
