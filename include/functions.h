#ifndef DEF_FUNCTIONS
#define DEF_FUNCTIONS

#include <stdlib.h>
#include <stdio.h>

#define MAX_ELEMENTS 1000

int *getAddress(char *name);
int initFile();
int closeFile();

int push(int val, int **addr);
int *allocate(int a);
int *allocateVar(char *var);

int *multiply(int *a, int *b);
int *divide(int *a, int *b);
int *substraction(int *a, int *b);
int *addition(int *a, int *b);
int *andOp(int *a, int *b);
int *orOp(int *a, int *b);

typedef struct Stack {
    int value;
    struct Stack *next;
} Stack;

typedef struct Data {
    char *name;
    int *address;
} Data;

Stack *stack;
Data tab[MAX_ELEMENTS];
FILE *asm_file;

#endif // DEF_FUNCTIONS