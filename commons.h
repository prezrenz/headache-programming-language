#ifndef COMMONS_H
#define COMMONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* STRUCTS */

#define MAX_ARRAY 256

typedef enum{
    NUMBER,
    COMP_PROC,
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
            int value;
        } number;
        
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

extern object* the_empty_list; // Essentially just a null to end lists
extern object* the_empty_environment;
extern object* the_global_environment;
extern object* symbol_table;
extern object* define_num_symbol;
extern object* define_array_symbol;
extern object* define_func_symbol;
extern object* if_symbol;
extern object* less_symbol;
extern object* great_symbol;
extern object* equal_symbol;

/* HELPERS */


object* gpl(object* obj); // Get Pair Left
object* gpr(object* obj); // Get Pair Right
void spl(object* obj, object* val); // Set Pair Left
void spr(object* obj, object* val); // Set Pair Right
int is_empty_list(object* obj);

/* READING */

char is_ending(char c);
int is_whitespace(char c);
int is_delimiter(char c);

object* make_pair(object* left, object* right);
object* make_symbol(char* value);
object* make_number(int num);

object* read(FILE* input);
object* read_pair(FILE* input);
object* read(FILE* input);

/* ENVIRONMENT */

object* get_enclosing_environment(object* env);
object* get_first_frame(object* env);
object* get_frame_vars(object* frame);
object* get_frame_vals(object* frame);

object* make_frame(object* vars, object* vals);
void add_binding_to_frame(object* var, object* val, object* frame);

object* extend_environment(object* vars, object* vals, object* base_env);

object* lookup_var_val(object* var, object* env);
void set_var_val(object* var, object* val, object* env);
void define_var(object* var, object* val, object* env);

object* setup_environment();

/* EVALUATING */

int is_symbol(object* obj);

int is_define_number(object* obj);

object* get_def_var(object* obj);

object* eval_define_number(object* obj, object* env);

int is_stack_plus(object* obj);
int is_stack_min(object* obj);

object* add_number_objects(object* x, object* y);
object* sub_number_objects(object* x, object* y);

object* get_stacking_symbol(object* obj);

object* eval_stack_plus(object* obj, object* env);
object* eval_stack_min(object* obj, object* env);

int is_if(object* obj);
int is_true(object* obj);

object* get_predicate(object* obj);
object* if_consequent(object* obj);
object* if_alternative(object* obj);

int is_cond_great(object* obj);
int is_cond_less(object* obj);
int is_cond_equal(object* obj);

object* cond_first_sym(object* obj);
object* cond_second_sym(object* obj);

#define COMPARE(sym1, sym2, op) (sym1->data.number.value op sym2->data.number.value)

object* eval(object* obj, object* env);

/* PRINTING */

void print(object* obj);
void print_pair(object* obj);

#endif
