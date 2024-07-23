#include "commons.h"

void print_pair(object* obj) {
    object* left;
    object* right;

    left = gpl(obj);
    right = gpr(obj);
    print(left);

    if(right->type == PAIR) {
        printf(" ");
        print_pair(right);
    } else if(right->type == EMPTY_LIST) {
        return;
    }
}

void print(object* obj) {
    switch (obj->type) {
        case EMPTY_LIST:
            printf("()");
            break;
        case SYMBOL:
            printf("%s", obj->data.symbol.value);
            break;
        case PAIR:
            printf("(");
            print_pair(obj);
            printf(")");
            break;
        case NUMBER:
            printf("%d", obj->data.number.value);
            break;
        default:
            fprintf(stderr, "Error: unimplemented or unknown object type %d\n", obj->type);
            exit(1);
    }
}
