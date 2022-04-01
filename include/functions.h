#ifndef DEF_FUNCTIONS
#define DEF_FUNCTIONS

#include <stdlib.h>
#include <stdio.h>

typedef struct Data {
    char *name;
    int value;
    int depth;
} Data;

typedef struct Stack {
    Data value;
    struct Stack *next;
} Stack;

enum Opcode { COP = 0x05, ADD = 0x01 };

Stack *getAddress(char *name);
int isVarExist(char *name);
int createVar(char *name);
int editVar(Stack *var_addr);
int initFile();
void incr_depth();
void decr_depth();
int closeFile();

int pull();
int push(int val);
int pushVar(char *name);
int allocate(int a);
int allocateVar(char *var);

int multiply();
int divide();
int substraction();
int addition();
int andOp();
int orOp();

int depth;
Stack *stack;
FILE *asm_file;

#endif // DEF_FUNCTIONS