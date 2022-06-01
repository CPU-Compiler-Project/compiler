#ifndef DEF_FUNCTIONS
#define DEF_FUNCTIONS

#include <stdlib.h>
#include <stdio.h>

#define MAX_BUFFER 100
#define MAX_INSTRUCTIONS 64

typedef struct Data {
    char *name;
    int addr;
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
int isVarAddressExist(Stack *var_addr);
int createVar(char *name);
int editVar(Stack *var_addr);
int initFile();
void incr_depth();
void decr_depth();
int writeToFile(char *str);
int writeInstructions();
int closeFile();

int pull();
int pullInstruction();
int push(int val);
int pushCOP(Stack *var_addr);
int copy(int src_addr, int dest_addr);
int pushInstruction(char *instruction);
int pushVar(char *name);
int allocate(int a);
int allocateVar(char *var);

int arithmeticOperation(char *op, int op_code);
int multiply();
int divide();
int substraction();
int addition();
int andOp();
int orOp();
int ifCond();
int ifJump();
int ifJumpElse();

// Les adresses memoire du FPGA sont numérotées de 0 a 15 dans notre implémentation vhdl et chaque registre fait 1 octet
// Pour savoir l'addresse du premier registre disponible, on a l'int sp qui pointe vers la première adresse dispo

int depth;
int i_addr;
int sp; // On stocke la valeur courante du pointeur de pile
Stack *stack;
InstructionStack *istack;
FILE *asm_file;

#endif // DEF_FUNCTIONS