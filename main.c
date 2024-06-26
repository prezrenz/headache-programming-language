#include <stdio.h>

/* LEXER */

int scan(char* input) {
    char c;
    char tok[256];

    puts(&input[0]);

    if((c = input[0]) == '(') {
        input++;
        c = *input; // skip the (
        while(c != ')' && c != '\0') {
            putc(c, stdout);
            putc('\n', stdout);
            input++;
            c = *input;
        }

        if(c == '\0') {
            fprintf(stderr, "Error: ending ) not found");
            return 2;
        }

        return 0;
    }
    else {
        fprintf(stderr, "Error: expected (");
        return 1;
    }
}

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
            
            if(scan(input) != 0) {
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
        /* Start of program */
        char line[1024];

        while(fgets(line, 1024, program)) {
            if(scan(line) != 0) {
                return 1;
            }
        }

        printf("Successfully opened file");
    }

    return 0;
}
