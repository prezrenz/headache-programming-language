#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "associativearray.h"
#include "commons.h"

/* GLOBALS */

assoc_array* environment;

object* the_empty_list;

/* HELPERS */

object* pair_left(object* obj) {
    return obj->data.pair.left; // TODO: check if type is pair
}

object* pair_right(object* obj) {
    return obj->data.pair.right; // TODO: check if type is pair
}

/* READ */

char is_ending(char c) {
    return c == '\0' && c == ')';
}

int is_whitespace(char c) {
    return c == ' ' || c == '\n';
}

int is_delimiter(char c) {
    return is_whitespace(c) || c == EOF || c == ')';
}

object* make_symbol(char* value) {
    object* obj;

    obj = malloc(sizeof(object));
    obj->type = SYMBOL;
    obj->data.symbol.value = strdup(value);

    return obj;
}

object* make_pair(object* left, object* right) {
    object* obj;

    obj = malloc(sizeof(object));
    obj->type = PAIR;
    obj->data.pair.left = left;
    obj->data.pair.right = right;

    return obj;
}

object* read(FILE* input);

object* read_pair(FILE* input) {
    char c;
    object* left;
    object* right;

    c = getc(input);
    if(c == ')') {
        return the_empty_list;
    }

    if(is_delimiter(c)) {
        fprintf(stderr, "Error: expected )\n");
        exit(1);
    }
    
    ungetc(c, input);
    left = read(input);
    
    c = getc(input);
    while(is_whitespace(c) && c != EOF) {
        c = getc(input);
    }
    ungetc(c, input);
    
    right = read_pair(input);

    return make_pair(left, right);
}

object* read(FILE* input) {
    char c;
    c = getc(input);

    while(is_whitespace(c) && c != EOF) {
        c = getc(input);
    }

#define BUFFER_MAX 1024
    int i = 0;
    char buffer[BUFFER_MAX];

    if(c == '(') {
        return read_pair(input);
    } else  {
        // Read a symbol
        while(!is_delimiter(c)) {

            if(i < BUFFER_MAX - 1) {
                buffer[i] = c;
                i++;
            } else {
                fprintf(stderr, "Error: symbol name too long, max is %d\n", BUFFER_MAX);
                exit(1);
            }

            c = getc(input);
        }
        ungetc(c, input);
        buffer[i] = '\0';
        return make_symbol(buffer);
    }

}

/* EVALUATE */

/* PRINT */

void print(object* obj);

void print_pair(object* obj) {
    object* left;
    object* right;

    left = pair_left(obj);
    right = pair_right(obj);
    print(left);

    if(right->type == PAIR) {
        printf(" ");
        print_pair(right);
    } else if(right->type == EMPTY_LIST) {
        return;
    }
}

void print(object* obj) {
    switch (obj->type) {
        case EMPTY_LIST:
            printf("()");
            break;
        case SYMBOL:
            printf("%s", obj->data.symbol.value);
            break;
        case PAIR:
            printf("(");
            print_pair(obj);
            printf(")");
            break;
        default:
            fprintf(stderr, "Error: unimplemented or unknown object type %d\n", obj->type);
            exit(1);
    }
}

int main(int argc, char** argv)
{
    FILE* program;

    the_empty_list = malloc(sizeof(object));
    the_empty_list->type = EMPTY_LIST;

    if(argc == 1) {
        char input[2048];

        puts("Headache REPL\n");
        puts("Press Ctrl+C to Exit\n");

        while (1) {
            printf("headache> ");
            print(read(stdin));
            printf("\n");
        }
    }
    else if(argc > 1)
    {
        fprintf(stderr, "Usage: headache (program)\n\tOmit program to enter REPL");
        return 1;
    }
    /* else if((program = fopen(argv[1], "r")) == NULL) { */
    /*     fprintf(stderr, "Error: Failed to open file %s", argv[1]); */
    /*     return 2; */
    /* } */
    /* else { */
    /*     // Start of program  */
    /**/
    /*     while(fgets(line, 1024, program)) { */
    /*         char* line_ptr = line; */
    /*         if(read(&line_ptr) != 0) { */
    /*             return 1; */
    /*         } */
    /*     } */
    /**/
    /*     printf("Successfully opened file"); */
    /* } */

    return 0;
}
