#ifndef DEF_FUNCTIONS
#define DEF_FUNCTIONS

#include <stdlib.h>
#include <stdio.h>

#define MAX_BUFFER 100

typedef struct Data {
    char *name;
    int value;
    int depth;
} Data;

typedef struct Stack {
    Data value;
    struct Stack *next;
} Stack;

typedef struct InstructionStack {
    char *instruction;
    struct InstructionStack *next;
} InstructionStack;

enum Opcode { ADD = 0x01, MUL = 0x02, SOU = 0x03, DIV = 0x04, COP = 0x05, AFC = 0x06, LOAD = 0x07, STORE = 0x08, JMP = 0x09, JMPF = 0x0A, JMPB = 0x0B, CMP = 0x0C, CMPF = 0x0D, CMPB = 0x0E, PRINT = 0x0F, HALT = 0x10, NOP = 0x11, PUSH = 0x12, POP = 0x13, CALL = 0x14, RET = 0x15 };

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
InstructionStack *istack;
FILE *asm_file;

#endif // DEF_FUNCTIONS