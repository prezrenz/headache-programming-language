#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "arraylist.h"
#include "associativearray.h"

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

int read(char* input) {
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
}

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
            fgets(input, 2048, stdin);

            if(read(input) != 0) {
                return 1;
            }
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
        char line[1024];

        while(fgets(line, 1024, program)) {
            if(read(line) != 0) {
                return 1;
            }
        }

        printf("Successfully opened file");
    }

    return 0;
}
