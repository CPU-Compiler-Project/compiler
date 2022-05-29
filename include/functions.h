#ifndef DEF_FUNCTIONS
#define DEF_FUNCTIONS

#include <stdlib.h>
#include <stdio.h>

#define MAX_BUFFER 100

typedef struct Data {
    char *name;
    int addr;
    int value; // il faudra supprimer ce champ a terme, les valeurs doivent uniquement etre stoquées sur la mémoire du fpga
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
enum AsmRegister { R0 = 0x00, R1 = 0x01, R2 = 0x02, R3 = 0x03, R4 = 0x04, R5 = 0x05, R6 = 0x06, R7 = 0x07, R8 = 0x08, R9 = 0x09, R10 = 0x0A, R11 = 0x0B, R12 = 0x0C, R13 = 0x0D, R14 = 0x0E, R15 = 0x0F };

Stack *getAddress(char *name);
int getValue(char *name);
int isVarExist(char *name);
int createVar(char *name);
int editVar(Stack *var_addr);
int initFile();
void incr_depth();
void decr_depth();
int writeToFile(char *str);
int writeInstructions();
int writeValues();
int closeFile();

int pull();
int pullInstruction();
int popASM();
int push(int val);
int pushInstruction(char *instruction);
int pushVar(char *name);
int allocate(int a);
int allocateVar(char *var);

int multiply();
int divide();
int substraction();
int addition();
int andOp();
int orOp();
int ifCond();

int depth;
int sp; // On stoque la valeur courante du pointeur de pile
Stack *stack;
InstructionStack *istack;
FILE *asm_file;

#endif // DEF_FUNCTIONS