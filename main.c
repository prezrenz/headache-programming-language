#include <stdio.h>

/* LEXER */



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
            printf("Input was: %s", input);
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

        printf("Successfully opened file");
    }

    return 0;
}
