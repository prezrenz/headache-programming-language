#include "commons.h"
#include <stdio.h>

object* make_compound_proc(object* parameters, object* body, object* env) {
    object* obj;

    obj = malloc(sizeof(object));
    obj->type = COMPOUND_PROC;
    obj->data.compound_proc.parameters = parameters;
    obj->data.compound_proc.body = body;
    obj->data.compound_proc.env = env;

    return obj;
}

object* make_lambda(object* parameters, object* body) {
    return make_pair(lambda_symbol, make_pair(parameters, body));
}

int is_symbol(object* obj) {
    return (obj->type == SYMBOL);
}

int is_define(object* obj) {
    return (gpl(obj)->type == SYMBOL) && (gpl(obj) == define_symbol);
}

object* get_def_var(object* obj) {
    if(is_symbol(gpl(gpr(obj)))) {
        return gpl(gpr(obj));
    } else {
        return gpl(gpl(gpr(obj)));
    }
}

object* get_def_val(object* obj, object* env) {
    if(is_empty_list(gpr(gpr(obj)))) {
        return make_number(0);
    }

    if(is_symbol(gpl(gpr(obj)))) {
        return eval(gpl(gpr(gpr(obj))), env);
    } else {
        return eval(make_lambda(gpr(gpl(gpr(obj))), gpr(gpr(obj))), env);
    }

}

object* eval_define(object* obj, object* env) {
    define_var(get_def_var(obj), get_def_val(obj, env), env);
    return get_def_var(obj);
}

int is_stack_plus(object* obj) {
    return (gpl(obj)->type == STACKPLUS);
}

int is_stack_min(object* obj) {
    return (gpl(obj)->type == STACKMIN);
}

object* add_number_objects(object* x, object* y) {
    unsigned char xval = x->data.number.value;
    unsigned char yval = y->data.number.value;
    return make_number(xval + yval);
}

object* sub_number_objects(object* x, object* y) {
    unsigned char xval = x->data.number.value;
    unsigned char yval = y->data.number.value;
    return make_number(xval - yval);
}

object* get_stacking_symbol(object* obj) {
    return gpl(gpr(obj));
}

object* eval_stack_plus(object* obj, object* env) {
    object* new_val = add_number_objects(
                        lookup_var_val(
                            get_stacking_symbol(obj), env),
                        gpl(obj));
    set_var_val(get_stacking_symbol(obj), new_val, env);     
    return lookup_var_val(get_stacking_symbol(obj), env);
}

object* eval_stack_min(object* obj, object* env) { 
    object* new_val = sub_number_objects(
                        lookup_var_val(
                            get_stacking_symbol(obj), env),
                        gpl(obj));
    set_var_val(get_stacking_symbol(obj), new_val, env);     
    return lookup_var_val(get_stacking_symbol(obj), env);
}

int is_if(object* obj) {
    return (gpl(obj)->type == SYMBOL) && (gpl(obj) == if_symbol);
}

int is_true(object* obj) {
    return obj->data.number.value > 0;
}

object* get_predicate(object* obj) {
    return gpl(gpr(obj));
}

object* if_consequent(object* obj) {
    return gpl(gpr(gpr(obj)));
}

object* if_alternative(object* obj) {
    if(is_empty_list(gpr(gpr(gpr(obj))))) {
        return make_number(-1);
    } else {
        return gpl(gpr(gpr(gpr(obj))));
    }
}

int is_cond_great(object* obj) {
    return (gpl(obj)->type == SYMBOL) && (gpl(obj) == great_symbol);
}

int is_cond_less(object* obj) {
    return (gpl(obj)->type == SYMBOL) && (gpl(obj) == less_symbol);
}

int is_cond_equal(object* obj) {
    return (gpl(obj)->type == SYMBOL) && (gpl(obj) == equal_symbol);
}

object* cond_first_sym(object* obj) {
    return gpl(gpr(obj));
}

object* cond_second_sym(object* obj) {
    return gpl(gpr(gpr(obj)));
}

int is_lambda(object* obj) {
    return (gpl(obj)->type == SYMBOL) && (gpl(obj) == lambda_symbol);
}

object* get_lambda_params(object* obj) {
    return gpl(gpr(obj));
}

object* get_lambda_body(object* obj) {
    return gpr(gpr(obj));
}

int is_last_exp(object* obj) {
    return is_empty_list(gpr(obj));
}

object* first_exp(object* obj) {
    return gpl(obj);
}

object* rest_exp(object* obj) {
    return gpr(obj);
}

#define COMPARE(sym1, sym2, op) (sym1->data.number.value op sym2->data.number.value)

int is_application(object* obj) {
    return obj->type == PAIR;
}


object* list_of_vals(object* obj, object* env) {
    if(is_empty_list(obj)) {
        return the_empty_list;
    } else {
        return make_pair(eval(gpl(obj), env),
                            list_of_vals(gpr(obj), env));
    }
}

object* eval(object* obj, object* env) {
    object* proc;
    object* args;

tailcall:
    if(is_empty_list(obj)) {
        return obj;
    } else if(is_symbol(obj)) {
        return lookup_var_val(obj, env);
    } else if(is_define(obj)) {
        return eval_define(obj, env);
    } else if(is_stack_plus(obj)) {
        return eval_stack_plus(obj, env);
    } else if(is_stack_min(obj)) {
        return eval_stack_min(obj, env);
    } else if(is_if(obj)) {
        obj = is_true(eval(get_predicate(obj), env)) ?
                if_consequent(obj) :
                if_alternative(obj);
        goto tailcall;
    } else if(is_cond_great(obj)) {
        object* sym1 = lookup_var_val(cond_first_sym(obj), env);
        object* sym2 = lookup_var_val(cond_second_sym(obj), env);
        return make_number(COMPARE(sym1, sym2, >));
    } else if(is_cond_less(obj)) {
        object* sym1 = lookup_var_val(cond_first_sym(obj), env);
        object* sym2 = lookup_var_val(cond_second_sym(obj), env);
        return make_number(COMPARE(sym1, sym2, <));
    } else if(is_cond_equal(obj)) {
        object* sym1 = lookup_var_val(cond_first_sym(obj), env);
        object* sym2 = lookup_var_val(cond_second_sym(obj), env);
        return make_number(COMPARE(sym1, sym2, ==));
    } else if(is_lambda(obj)) {
        return make_compound_proc(get_lambda_params(obj),
                                    get_lambda_body(obj),
                                    env);
    } else if(is_application(obj)) { // NOTE: needs to be checked AFTER others
        proc = eval(gpl(obj), env); // Gets the operator
        args = list_of_vals(gpr(obj), env); // Gets operands
        
        if(proc->type == PRIMITIVE_PROC) { 
            return (proc->data.primitive_proc.proc)(args);
         } else if(proc->type == COMPOUND_PROC) {
            env = extend_environment(proc->data.compound_proc.parameters,
                                        args,
                                        proc->data.compound_proc.env);
            obj = proc->data.compound_proc.body;
            
            // Start evaluating body, except last one
            while(!is_last_exp(obj)) {
                eval(first_exp(obj), env);
                obj = rest_exp(obj);
            }
            obj = first_exp(obj); // the last command in body
            goto tailcall;        // starts the tailcall
                                  // with the last command in body as exp
         } else {
             fprintf(stderr, "Error: unknown procedure type %d\n", proc->type);
             exit(1);
         }

    } else {
        fprintf(stderr, "Eval error: unimplemented or illegal type %d\n", obj->type);
        exit(1);
    }
}
