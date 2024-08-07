#ifndef COMMONS_H
#define COMMONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* STRUCTS */

#define MAX_ARRAY 256

typedef enum{
    NUMBER,
    SYMBOL,
    EMPTY_LIST,
    PAIR,
    STACKPLUS,
    STACKMIN,
    PRIMITIVE_PROC,
    COMPOUND_PROC
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

        struct {
            struct object* (*proc)(struct object* args);
        } primitive_proc;

        struct {
            struct object* parameters;
            struct object* body;
            struct object* env;
        } compound_proc;
    
    } data; 

} object;

/* GLOBALS */

extern object* the_empty_list; // Essentially just a null to end lists
extern object* the_empty_environment;
extern object* the_global_environment;
extern object* symbol_table;
extern object* define_symbol;
extern object* if_symbol;
extern object* and_symbol;
extern object* or_symbol;
extern object* less_symbol;
extern object* great_symbol;
extern object* equal_symbol;
extern object* lambda_symbol;

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

/* PRIMITIVE PROCEDURES*/

object* make_primitive_proc(object* (*proc)(object* args));

object* add_proc(object* args);
object* sub_proc(object* args);
object* div_proc(object* args);
object* mod_proc(object* args);
object* mult_proc(object* args);

object* make_pair_proc(object* args);
object* get_left_proc(object* args);
object* get_right_proc(object* args);
object* set_left_proc(object* args);
object* set_right_proc(object* args);
object* make_list_proc(object* args);

object* not_proc(object* args);

object* print_ascii_proc(object* args);
object* read_line_proc(object* args);

/* EVALUATING */

object* make_compound_proc(object* parameters, object* body, object* env);
object* make_lambda(object* parameters, object* body);

int is_symbol(object* obj);

int is_define(object* obj);

object* get_def_var(object* obj);
object* get_def_val(object* obj, object* env);

object* eval_define(object* obj, object* env);

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

int is_and(object* obj);
int is_or(object* obj);
object* and_tests(object* obj);
object* or_tests(object* obj);

int is_cond_great(object* obj);
int is_cond_less(object* obj);
int is_cond_equal(object* obj);

object* cond_first_sym(object* obj);
object* cond_second_sym(object* obj);

int is_lambda(object* obj);
object* get_lambda_params(object* obj);
object* get_lambda_body(object* obj);

int is_last_exp(object* obj);
object* first_exp(object* obj);
object* rest_exp(object* obj);

#define COMPARE(sym1, sym2, op) (sym1->data.number.value op sym2->data.number.value)

object* eval(object* obj, object* env);

/* PRINTING */

void print(object* obj);
void print_pair(object* obj);

#endif
