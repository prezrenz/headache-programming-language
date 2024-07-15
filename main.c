#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* STRUCTS */

typedef enum{
    NUMBER,
    COMP_PROC,
    ARRAY,
    SYMBOL,
    EMPTY_LIST,
    PAIR,
    STACKPLUS,
    STACKMIN
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

        struct {
            int len;
        } stackop;
    
    } data; 

} object;

/* GLOBALS */

object* the_empty_list; // Essentially just a null to end lists
object* the_empty_environment;
object* the_global_environment;
object* symbol_table;
object* define_num_symbol;
object* define_array_symbol;
object* define_func_symbol;

/* HELPERS */

object* get_pair_left(object* obj) {
    return obj->data.pair.left; // TODO: check if type is pair
}

object* get_pair_right(object* obj) {
    return obj->data.pair.right; // TODO: check if type is pair
}

void set_pair_left(object* obj, object* val) {
    obj->data.pair.left = val;
}

void set_pair_right(object* obj, object* val) {
    obj->data.pair.right = val;
}

int is_empty_list(object* obj) {
    return obj == the_empty_list;
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

object* make_pair(object* left, object* right);

object* make_symbol(char* value) {
    object* obj;
    object* sym;

    sym = symbol_table;
    while (!is_empty_list(sym)) {
        if(strcmp(get_pair_left(sym)->data.symbol.value, value) == 0) {
            return get_pair_left(sym);
        }
        sym = get_pair_right(sym);
    }

    obj = malloc(sizeof(object));
    obj->type = SYMBOL;
    obj->data.symbol.value = strdup(value);
    symbol_table = make_pair(obj, symbol_table); // add to the symbol table

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

object* make_number(char num) {
    object* obj;

    obj = malloc(sizeof(object));
    obj->type = NUMBER;
    obj->data.number.value = num;

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

/* ENVIRONMENT */

object* get_enclosing_environment(object* env) {
    return get_pair_right(env);
}

object* get_first_frame(object* env) {
    return get_pair_left(env);
}

object* get_frame_vars(object* frame) {
    return get_pair_left(frame);
}

object* get_frame_vals(object* frame) {
    return get_pair_right(frame);
}

object* make_frame(object* vars, object* vals) {
    return make_pair(vars, vals);
}

void add_binding_to_frame(object* var, object* val, object* frame) {
    set_pair_left(frame, make_pair(var, get_pair_left(frame)));
    set_pair_right(frame, make_pair(val, get_pair_right(frame)));
}

object* extend_environment(object* vars, object* vals, object* base_env) {
    return make_pair(make_frame(vars, vals), base_env);
}

object* lookup_var_val(object* var, object* env) {
    object* frame;
    object* vars;
    object* vals;

    while (!is_empty_list(env)) {
        frame = get_first_frame(env);
        vars = get_frame_vars(frame);
        vals = get_frame_vals(frame);

        while (!is_empty_list(vars)) {
            if(var == get_pair_left(vars)) {
                return  get_pair_left(vals);
            }

            vars = get_pair_right(vars);
            vals = get_pair_right(vals);
        }

        env = get_enclosing_environment(env);
    }
    fprintf(stderr, "Error: variable not defined\n");
    exit(1);
}

void set_var_val(object* var, object* val, object* env) {
    object* frame;
    object* vars;
    object* vals;

    while (!is_empty_list(env)) {
        frame = get_first_frame(env);
        vars = get_frame_vars(frame);
        vals = get_frame_vals(frame);

        while (!is_empty_list(vars)) {
            if(var == get_pair_left(vars)) {
                set_pair_left(vals, val);
                return;
            }

            vars = get_pair_right(vars);
            vals = get_pair_right(vals);
        }

        env = get_enclosing_environment(env);
    }
    fprintf(stderr, "Error: variable not defined\n");
    exit(1);
}

void define_var(object* var, object* val, object* env) {
    object* frame;
    object* vars;
    object* vals;

    frame = get_first_frame(env);
    vars = get_frame_vars(frame);
    vals = get_frame_vals(frame);

    while (!is_empty_list(vars)) {
        if(var == get_pair_left(vars)) {
            set_pair_left(vals, val);
            return;
        }

        vars = get_pair_right(vars);
        vals = get_pair_right(vals);
    }

    add_binding_to_frame(var, val, frame);
}

object* setup_environment() {
    object* initial_env;

    initial_env = extend_environment(the_empty_list,
                                    the_empty_list,
                                    the_empty_environment);

    return initial_env;
}

/* EVALUATE */

int is_define_number(object* obj) {
    return (get_pair_left(obj)->type == SYMBOL) && (get_pair_left(obj) == define_num_symbol); // NOTE: does this work?
}

object* eval(object* obj, object* env) {
    switch (obj->type) {
        case EMPTY_LIST:
            return obj;
        case SYMBOL:
            return lookup_var_val(obj, env);
        case PAIR:
            if(is_define_number(obj)) {
                define_var(get_pair_left(get_pair_right(obj)), make_number(0), env);
                return lookup_var_val(get_pair_left(get_pair_right(obj)), env);
            } else {
                fprintf(stderr, "Eval error: unimplemented or illegal\n");
                exit(1); // TODO: parse compound procedures
            }
        default:
            fprintf(stderr, "Eval Error: unimplemented or illegal\n");
            exit(1);
    }
}

/* PRINT */

void print(object* obj);

void print_pair(object* obj) {
    object* left;
    object* right;

    left = get_pair_left(obj);
    right = get_pair_right(obj);
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
        case NUMBER:
            printf("%d", obj->data.number.value);
            break;
        case ARRAY:
            printf("%d", obj->data.array.arr[obj->data.array.curr_index]);
        default:
            fprintf(stderr, "Error: unimplemented or unknown object type %d\n", obj->type);
            exit(1);
    }
}

int main(int argc, char** argv)
{
    FILE* program;

    /* INITIALIZATION */

    the_empty_list = malloc(sizeof(object));
    the_empty_list->type = EMPTY_LIST;

    // Setup symbol table
    symbol_table = the_empty_list;
    define_num_symbol = make_symbol("!!");
    define_array_symbol = make_symbol("[]"); 
    define_func_symbol = make_symbol("^^"); 

    // Setup environment
    the_empty_environment = the_empty_list;
    the_global_environment = setup_environment();

    /* START */

    if(argc == 1) {
        puts("Headache REPL\n");
        puts("Press Ctrl+C to Exit\n");

        while (1) {
            printf("headache> ");
            print(read(stdin));
            printf("\n");
        }
    }
    else if(argc > 2)
    {
        fprintf(stderr, "Usage: headache (program)\n\tOmit program to enter REPL");
        return 1;
    }
    else if((program = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Error: Failed to open file %s", argv[1]);
        return 2;
    }
    else {
        // Start of program
        while (!feof(program)) {
            print(eval(read(program), the_global_environment));
            printf("\n");
        }
        printf("\n\nSuccessfully executed file");
    }

    return 0;
}
