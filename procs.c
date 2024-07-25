#include "commons.h"
#include <stdio.h>

object* make_primitive_proc(object* (*proc)(struct object* args)) {
    object* obj;

    obj = malloc(sizeof(object));
    obj->type = PRIMITIVE_PROC;
    obj->data.primitive_proc.proc = proc;

    return obj;
}

object* add_proc(object* args) {
    int result = 0;
    object* current;

    while(!is_empty_list(args)) {
        current = gpl(args);

        if(current->type != NUMBER) {
            fprintf(stderr, "didn't get a number, got type %d instead\n", current->type);
            exit(1);
        }

        result += current->data.number.value;
        args = gpr(args);
    }

    return make_number(result);
}

object* sub_proc(object* args) {
    int result = 0;
    object* current;
   
    current = gpl(args);
 
    if(current->type != NUMBER) {
        fprintf(stderr, "didn't get a number, got type %d instead\n", current->type);
        exit(1);
    }

    result = current->data.number.value;    
    args = gpr(args);

    while(!is_empty_list(args)) {
        current = gpl(args);
        
        if(current->type != NUMBER) {
            fprintf(stderr, "didn't get a number, got type %d instead\n", current->type);
            exit(1);
        }

        result -= current->data.number.value;
        args = gpr(args);
    }

    return make_number(result);
}

object* div_proc(object* args) {
    return make_number(
                        (gpl(args))->data.number.value /
                        (gpl(gpr(args)))->data.number.value);
}

object* mod_proc(object* args) {
    return make_number(
                        (gpl(args))->data.number.value %
                        (gpl(gpr(args)))->data.number.value);
}

object* mult_proc(object* args) {
    int result = 1;
    object* current;

    while(!is_empty_list(args)) {
        current = gpl(args);

        if(current->type != NUMBER) {
            fprintf(stderr, "didn't get a number, got type %d instead\n", current->type);
            exit(1);
        }

        result *= current->data.number.value;
        args = gpr(args);
    }

    return make_number(result);
}
