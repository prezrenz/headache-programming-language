#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "arraylist.h"
#include "associativearray.h"
#include "commons.h"

/* ENVIRONMENT */

assoc_array* environment;

/* READ */

char is_not_ending(char c) {
    return c != '\0' && c != ')';
}

char* parse_name(char** input) {
    char c = **input;
    char* name = malloc(256);
    *name = '\0';

    while(c != ' ' && is_not_ending(c)) {
        if(isalnum(c)) {
            strncat(name, &c, 1);
        }
        else {
            return NULL; // found non-alphanumeric in name
        }

        (*input)++;
        c = **input;
    }

    if(c == '\0') {
        return NULL;
    }

    if(c == ')') {
        (*input)--;
    }

    printf("%s", name);
    return name;
}

int is_whitespace(char c) {
    return c == ' ' || c == '\n';
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
        return NULL;
    }

    left = read(input);
    right = read_pair(input);

    return make_pair(left, right);
}

object* read(FILE* input) {
    char c;
    c = getc(input);

    if(c == '(') {
        return read_pair(input);
    } else if(c == '@') {
        c = getc(input);
        if(c == '@') {
            object* obj = make_symbol("@@");
            return obj;
        } else {
            fprintf(stderr, "Error: expected @ after @\n");
            exit(1);
        }
    } else if(c == '!') {
        c = getc(input);
        if(c == '!') {
            object* obj = make_symbol("!!");
            return obj;
        } else {
            fprintf(stderr, "Error: expected ! after !\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "Error: expected (");
        return NULL;
    }

}

/* int read(char* input) {
    char c;
    array_list* list;
    list = array_list_new();

    puts(&input[0]);

    if((c = input[0]) == '(') {
        input++;
        c = *input; // skip the (
        while(is_not_ending(c)) {
            switch (c) {
                case '!':
                    input++;
                    c = *input;
                    if(c == '!') {
                        array_list_append(list, "!!");
                    }
                    else {
                        fprintf(stderr, "Error: expected ! after !");
                        return 2;
                    }
                    break;
                case '@':
                    input++;
                    c = *input;
                    if(c == '@') {
                        array_list_append(list, "@@");
                    }
                    else {
                        fprintf(stderr, "Error: expected @ after @");
                        return 2;
                    }
                    break;
                case ' ':
                case '\n':
                    break;
                default:
                   if(isalnum(c)) {
                       char* new_name = strdup(parse_name(&input));

                       if(new_name == NULL) {
                           return 3; // failed to parse name
                       }

                       array_list_append(list, new_name);
                   }
                   else {
                       printf("UNIMPLEMENTED\n");
                   }
            }

            putc(c, stdout);
            putc('\n', stdout);

            input++;
            c = *input;
        }

        if(c == '\0') {
            fprintf(stderr, "Error: ending ) not found");
            return 2;
        }

        array_list_print(list);

        return 0;
    }
    else {
        fprintf(stderr, "Error: expected (");
        return 1;
    }
} */

/* EVALUATE */

/* PRINT */

int main(int argc, char** argv)
{
    FILE* program;

    if(argc == 1) {
        char input[2048];

        puts("Headache REPL");
        puts("Press Ctrl+C to Exit\n");

        while (1) {
            fputs("headache> ", stdout);

            read(stdin);
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
