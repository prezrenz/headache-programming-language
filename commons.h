#ifndef COMMONS_H
#define COMMONS_H

typedef enum{
    NUMBER,
    FUNCTION,
    ARRAY
} object_type;

typedef struct {
    object_type type;
    
    union {
        struct {
            unsigned char value;
        } number;
        
        struct {
            int arg_count;
            char** args;
            char** ops;
        } lambda;

        struct  {
            int curr_index;
            unsigned char arr[256];
        } array;
    
    } data; 

} object;

#endif
