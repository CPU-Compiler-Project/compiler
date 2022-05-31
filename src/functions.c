#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "functions.h"

// en fait on a pas implementer de push en VHDL, il faut qu'on gère les adresses memoire
// du fpga. Elles sont numérotées de 0 a 15 dans notre implémentation vhdl et chaque registre fait 1 octet
// Pour savoir l'addresse du premier registre disponible, on a l'int sp qui pointe vers la première adresse dispo
void printStack();
void printStack() {
    
    Stack *stack_tmp = stack;
    printf("On va print la stack paniquez pas\n");

    while (stack_tmp != NULL) {
        printf("Stack value %s \n", stack_tmp->value.name);
        stack_tmp = stack_tmp->next;
    }
}

int push(int val)
{
    char opStr[MAX_BUFFER];
    sprintf(opStr, "X\"%02x%02x%02x00\" -- @%d => AFC 0x%02x %d", AFC, sp, val, i_addr, sp, val);
    i_addr++;
    sp++;
    printf("AFC: [%s]\n", opStr);
    pushInstruction(opStr);

    return 0;
}

int copy(int src_addr, int dest_addr) {
    char opStr[MAX_BUFFER];
    sprintf(opStr, "X\"%02x%02x%02x00\" -- @%d => COP 0x%02x 0x%02x", COP, dest_addr, src_addr, i_addr, dest_addr, src_addr);
    i_addr++;
    printf("COP: [%s]\n", opStr);
    
    return pushInstruction(opStr);
}

int pushCOP(Stack *var_addr)
{
    printStack();
    if(!isVarAddressExist(var_addr)) {
        printf("pushCOP error [%p]\n", var_addr);
        return -1;
    }
    copy(var_addr->value.addr, sp);
    sp++;
    
    return 0;
}

int pushInstruction(char *instruction)
{
    InstructionStack *element = malloc(sizeof(&istack));
    if(!element)
        return -1;
    element->instruction = strndup(instruction, MAX_BUFFER);
    element->next = istack;
    istack = element;

    return 0;
}

int pushVar(char *name)
{
    Stack *element = malloc(sizeof(&stack));
    if(!element)
        return -1;
    element->value = (Data) { .name = name, .addr = sp, .depth = depth };
    element->next = stack;
    stack = element;
    push(0); // On réserve un registre qu'on initialise à 0

    return 0;
}

int pull() {
    stack = stack->next;
    sp--;
    return 0;
}

int pullInstruction() {
    istack = istack->next;
    i_addr--;
    return 0;
}

int initFile() {
    asm_file = fopen("./build/out.asm", "w");
    if (asm_file == NULL) {
        return -1;
    }

    stack = NULL;
    istack = NULL;
    depth = 0;

    return 0;
}

int writeToFile(char *str) {
    if(asm_file == NULL || fprintf(asm_file, "%s\n", str) < 0) {
        return -1;
    }
    return 0;
}

int writeInstructions() {
    int i_empty = MAX_INSTRUCTIONS-1;

    while(i_empty >= MAX_INSTRUCTIONS-1-(MAX_INSTRUCTIONS-1-i_addr)) {
        char str[MAX_BUFFER];
        sprintf(str, "%sX\"00000000\" -- @%d", (i_empty == MAX_INSTRUCTIONS-1 ? "  " : ", "), i_empty);
        if(writeToFile(str) < 0) {
            return -1;
        }
        i_empty--;
    }

    while (istack != NULL) {
        char str[MAX_BUFFER];
        sprintf(str, "%s%s", (i_addr == MAX_INSTRUCTIONS-1 ? "  " : ", "), istack->instruction);
        if(writeToFile(str) < 0) {
            return -1;
        }
        pullInstruction();
    }
    
    return 0;
}

void incr_depth() {
    depth++;
}

void decr_depth() {
    if (depth >=0) 
        depth--;
    else
        depth = 0;
}

int closeFile() {
    if (fclose(asm_file)) {
        return -1;
    }
    return 0;
}

int allocate(int a) {
    push(a);
    printf("%d -> %d[%s]\n", a, stack->value.addr, stack->value.name);
    return 0;
}

int allocateVar(char *var) {   
    pushVar(var);
    printf("%s -> %d[%s]\n", var, stack->value.addr, stack->value.name);
    return 0;
}

int isVarExist(char *name) {
    return (getAddress(name) != NULL);
}

int isVarAddressExist(Stack *var_addr) {
    Stack *stack_tmp = stack;
    while (stack_tmp != NULL) {
        if(stack_tmp == var_addr)
            return 1;
        stack_tmp = stack_tmp->next;
    }
    return 0;
}

int editVar(Stack *var_addr) {
    if(!isVarAddressExist(var_addr)) {
        printf("editVar error [%p]\n", var_addr);
        return -1;
    }

    copy(--sp, var_addr->value.addr); // On libere la derniere case mémoire utilisée

    // instruction de changement de valeur a l'addresse var_addr
    printf("[UPDATE]%s -> %d[%s]\n",  var_addr->value.name, var_addr->value.addr, var_addr->value.name);

    return 0;
}

int createVar(char *name) {
    if(isVarExist(name)) {
        printf("createVar error [%s]\n", name);
        return -1;
    }
    allocateVar(name);
    return 0;
}

Stack *getAddress(char *name) {
    Stack *stack_tmp = stack;

    while (stack_tmp != NULL && strcmp(stack_tmp->value.name, name) != 0) {
        stack_tmp = stack_tmp->next;
    }
    
    if(stack_tmp == NULL) {
        return NULL; //TODO
    }

    return stack_tmp;
}

int addition() {    
    return arithmeticOperation("ADD", ADD);
}

int multiply() {
    return arithmeticOperation("MUL", MUL);
}

int divide() {
    return arithmeticOperation("DIV", DIV);
}

int substraction() {
    return arithmeticOperation("SOU", SOU);
}

int arithmeticOperation(char *op, int op_code) {
    char opStr[MAX_BUFFER];
    sp--;
    sprintf(opStr, "X\"%02x%02x%02x%02x\" -- @%d => %s 0x%02x 0x%02x 0x%02x", op_code, sp-1, sp-1, sp, i_addr, op, sp-1, sp-1 , sp);  // sp-1 is before last element
    i_addr++;
    printf("%s: [%s]\n", op, opStr);
    return pushInstruction(opStr);
}

int andOp() {  // On utilise le flag Z: s'il est levé, alors booléen négatif
    return arithmeticOperation("MUL", MUL) ? 1 : 0; // should be ok but we still want to be sure to output a boolean
}

int orOp() {   // On utilise le flag Z: s'il est levé, alors booléen négatif
    return arithmeticOperation("ADD", ADD) ? 1 : 0; // should be ok but we still want to be sure to output a boolean
}

int ifCond() {
    char opStr[MAX_BUFFER];
    sprintf(opStr, "tmp"); // sera remplacer plus tard dans la compilation
    printf("JMPF: [%s]\n", opStr);
    pushInstruction(opStr);
    return 0;
}

int ifBis() { 
    InstructionStack *element = malloc(sizeof(&istack));
    int instructionCPT = 0;
    while (strcmp(element->instruction,"tmp")!=0) {
        element = istack->next;
        instructionCPT++;
    }

    char opStr[MAX_BUFFER];
    sprintf(opStr, "0x%x ", JMPF);
    printf("JMPF: [%s]\n", opStr);
    pushInstruction(opStr);

    return 0;
}