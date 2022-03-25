#include <stdlib.h>
#include <stdio.h>

#include "functions.h"

int push(int val, int **addr)
{
    Stack *element = malloc(sizeof(&stack));
    if(!element)
        return -1;
    element->value = val;
    element->next = stack;
    stack = element;

    *addr = &stack->value;

    return 0;
}

int initFile() {
    asm_file = fopen("./build/out.asm", "w");
    if (asm_file == NULL) {
        return -1;
    }
    return 0;
}

int closeFile() {
    if (fclose(asm_file)) {
        return -1;
    }
    return 0;
}


int *allocate(int a) {
    int *addr = NULL;
    push(a, &addr);
    printf("%d -> %d at %p\n", a, stack->value, (void*)stack->next);
    return addr;
}

int *allocateVar(char *var) {
    int *addr = malloc(sizeof(int));
    

    return addr;
}

int *getAddress(char *name) {
    int *add = NULL;
    for (int i = 0 ; i < MAX_ELEMENTS ; i++) {
        if(tab[i].name == name) {
            add = tab[i].address;
            break;
        }
    }
    if(add == NULL) {
        return NULL; //TODO
    }

    return add;
}


int *multiply(int *a, int *b) {
    return NULL;
}

int *divide(int *a, int *b) {
    return NULL;
}

int *substraction(int *a, int *b) {
    return NULL;
}

int *addition(int *a, int *b) {
    return NULL;
}

int *andOp(int *a, int *b) {
    return NULL;
}

int *orOp(int *a, int *b) {
    return NULL;
}
