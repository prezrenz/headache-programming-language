#include "commons.h"

char is_ending(char c) {
    return c == '\0' && c == ')';
}

int is_whitespace(char c) {
    return c == ' ' || c == '\n';
}

int is_delimiter(char c) {
    return is_whitespace(c) || c == EOF || c == ')';
}

object* make_pair(object* left, object* right) {
    object* obj;

    obj = malloc(sizeof(object));
    obj->type = PAIR;
    obj->data.pair.left = left;
    obj->data.pair.right = right;

    return obj;
}

object* make_symbol(char* value) {
    object* obj;
    object* sym;

    sym = symbol_table;
    while (!is_empty_list(sym)) {
        if(strcmp(gpl(sym)->data.symbol.value, value) == 0) {
            return gpl(sym);
        }
        sym = gpr(sym);
    }

    obj = malloc(sizeof(object));
    obj->type = SYMBOL;
    obj->data.symbol.value = strdup(value);
    symbol_table = make_pair(obj, symbol_table); // add to the symbol table

    return obj;
}

object* make_number(int num) {
    object* obj;

    obj = malloc(sizeof(object));
    obj->type = NUMBER;
    obj->data.number.value = num;

    return obj;
}

object* make_array() {
    object* obj;

    obj = malloc(sizeof(object));
    obj->type = NUMBER;
    obj->data.array.curr_index = 0;

    for(int i = 0; i < MAX_ARRAY; i++) {
        obj->data.array.arr[i] = 0;
    }

    return obj;
}

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
#define STACKOP_MAX 255
    
    int i = 0;
    char buffer[BUFFER_MAX];

    if(c == '(') {
        // Read a pair
        return read_pair(input);
    } else if(c == '+') {
        // Read a stacking plus
        object* obj;
        obj = malloc(sizeof(object));
        obj->type = STACKPLUS;
        obj->data.stackop.len = 0;

        while (!is_delimiter(c)) {
            if(c != '+') {
                fprintf(stderr, "Error: expected '+' but got '%c' instead\n", c);
                exit(1);
            }

            if(i < STACKOP_MAX) {
                obj->data.stackop.len += 1;
                i++;
            } else {
                fprintf(stderr, "Error: too many +, max is %d\n", STACKOP_MAX);
                exit(1);
            }
            c = getc(input);
        }
        ungetc(c, input);
        return obj;
    } else if(c == '-') {
        // Read a stacking minus
        object* obj;
        obj = malloc(sizeof(object));
        obj->type = STACKMIN;
        obj->data.stackop.len = 0;

        while (!is_delimiter(c)) {
            if(c != '-') {
                fprintf(stderr, "Error: expected '-' but got '%c' instead\n", c);
                exit(1);
            }

            if(i < STACKOP_MAX) {
                obj->data.stackop.len += 1;
                i++;
            } else {
                fprintf(stderr, "Error: too many -, max is %d\n", STACKOP_MAX);
                exit(1);
            }
            c = getc(input);
        }
        ungetc(c, input);
        return obj;
    } else {
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
