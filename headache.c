#include "commons.h"
#include <stdio.h>

object* the_empty_list; // Essentially just a null to end lists
object* the_empty_environment;
object* the_global_environment;
object* symbol_table;
object* define_symbol;
object* if_symbol;
object* less_symbol;
object* great_symbol;
object* equal_symbol;
object* lambda_symbol;

object* gpl(object* obj) { // Get Pair Left
    return obj->data.pair.left;
}

object* gpr(object* obj) { // Get Pair Right
    return obj->data.pair.right;
}

void spl(object* obj, object* val) { // Set Pair Left
    obj->data.pair.left = val;
}

void spr(object* obj, object* val) { // Set Pair Right
    obj->data.pair.right = val;
}

int is_empty_list(object* obj) {
    return obj == the_empty_list;
}

int main(int argc, char** argv)
{
    FILE* program;

    /* INITIALIZATION */

    the_empty_list = malloc(sizeof(object));
    the_empty_list->type = EMPTY_LIST;

    // Setup symbol table
    symbol_table = the_empty_list;
    define_symbol = make_symbol("!!");

    if_symbol = make_symbol("??");
    less_symbol = make_symbol("<?");
    great_symbol = make_symbol(">?");
    equal_symbol = make_symbol("=?");

    lambda_symbol = make_symbol("^^");

    // Setup environment
    the_empty_environment = the_empty_list;
    the_global_environment = setup_environment();

#define add_primitive(ache_name, c_name)        \
        define_var(make_symbol(ache_name),      \
                make_primitive_proc(c_name),    \
                the_global_environment);

        add_primitive("@+", add_proc);
        add_primitive("@-", sub_proc);
        add_primitive("//", div_proc);
        add_primitive("%%", mod_proc);
        add_primitive("**", mult_proc);

        add_primitive("[]", make_pair_proc);
        add_primitive("[[]]", make_list_proc);
        add_primitive("[[", get_left_proc);
        add_primitive("]]", get_right_proc);
        add_primitive("@[", set_left_proc);
        add_primitive("@]", set_right_proc);

        add_primitive("@@", print_ascii_proc);
        add_primitive("##", read_line_proc);

    /* START */

    if(argc == 1) {
        puts("Headache REPL\n");
        puts("Press Ctrl+C to Exit\n");

        while (1) {
            printf("headache> ");
            print(eval(read(stdin), the_global_environment));
            printf("\n");
        }
    }
    else if(argc > 2) {
        fprintf(stderr, "Usage: headache (program)\n\tOmit program to enter REPL");
        return 1;
    }
    else if((program = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Error: Failed to open file %s", argv[1]);
        return 2;
    }
    else {
        char* ext = strchr(argv[1], '.');
        if(ext != NULL) {
            if(strcmp(ext, ".ache") != 0) {
                fprintf(stderr, "expected a .ache program file");
                exit(1);
            }
        }

        // Start of program
        while (!feof(program)) {
            print(eval(read(program), the_global_environment));
            // eval(read(program), the_global_environment);
            printf("\n");
        }
        printf("\n\nSuccessfully executed file");
    }

    return 0;
}
