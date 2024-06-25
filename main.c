#include <stdio.h>

int main(int argc, char** argv)
{
    FILE* program;

    if(argc != 2) {
        fprintf(stderr, "Usage: headache <program>");
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
