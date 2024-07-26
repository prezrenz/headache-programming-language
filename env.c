#include "commons.h"

object* get_enclosing_environment(object* env) {
    return gpr(env);
}

object* get_first_frame(object* env) {
    return gpl(env);
}

object* get_frame_vars(object* frame) {
    return gpl(frame);
}

object* get_frame_vals(object* frame) {
    return gpr(frame);
}

object* make_frame(object* vars, object* vals) {
    return make_pair(vars, vals);
}

void add_binding_to_frame(object* var, object* val, object* frame) {
    spl(frame, make_pair(var, gpl(frame)));
    spr(frame, make_pair(val, gpr(frame)));
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
            if(var == gpl(vars)) {
                return  gpl(vals);
            }

            vars = gpr(vars);
            vals = gpr(vals);
        }

        env = get_enclosing_environment(env);
    }
    fprintf(stderr, "Error: variable %s not defined\n", var->data.symbol.value);
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
            if(var == gpl(vars)) {
                spl(vals, val);
                return;
            }

            vars = gpr(vars);
            vals = gpr(vals);
        }

        env = get_enclosing_environment(env);
    }
    fprintf(stderr, "Error: variable %s not defined\n", var->data.symbol.value);
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
        if(var == gpl(vars)) {
            spl(vals, val);
            return;
        }

        vars = gpr(vars);
        vals = gpr(vals);
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
