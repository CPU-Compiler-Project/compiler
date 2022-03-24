#include <stdlib.h>
#include <stdio.h>

#include "functions.h"

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
    
}

int *divide(int *a, int *b) {
    
}

int *substraction(int *a, int *b) {
    
}

int *addition(int *a, int *b) {
    
}

int *andOp(int *a, int *b) {
    
}

int *orOp(int *a, int *b) {
    
}
