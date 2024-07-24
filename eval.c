#include "commons.h"

int is_symbol(object* obj) {
    return (obj->type == SYMBOL);
}

int is_define_number(object* obj) {
    return (gpl(obj)->type == SYMBOL) && (gpl(obj) == define_num_symbol);
}

object* get_def_var(object* obj) {
    return gpl(gpr(obj));
}

object* get_def_val(object* obj, object* env) {
    if(is_empty_list(gpr(gpr(obj)))) {
        return the_empty_list;
    }

    return eval(gpl(gpr(gpr(obj))), env);
}

object* eval_define_number(object* obj, object* env) {
    object* val = get_def_val(obj, env);

    if(is_empty_list(val)) {
        val = make_number(0);
    }

    define_var(get_def_var(obj), val, env);
    return lookup_var_val(gpl(gpr(obj)), env);
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

#define COMPARE(sym1, sym2, op) (sym1->data.number.value op sym2->data.number.value)

object* eval(object* obj, object* env) {
tailcall:
    if(is_empty_list(obj)) {
        return obj;
    } else if(is_symbol(obj)) {
        return lookup_var_val(obj, env);
    } else if(is_define_number(obj)) {
        return eval_define_number(obj, env);
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
    } else {
        fprintf(stderr, "Eval error: unimplemented or illegal type %d\n", obj->type);
        exit(1);
    }
}
