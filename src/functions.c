#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "functions.h"


int push(int val)
{
    char opStr[MAX_BUFFER];
    sprintf(opStr, "0x%x %d", PUSH, val);
    printf("PUSH: [%s]\n", opStr);
    pushInstruction(opStr);

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
    element->value = (Data) { .name = name, .value = 0, .depth = depth };
    element->next = stack;
    stack = element;

    return 0;
}

int pull() {
    stack = stack->next;
    return 0;
}
int popASM() {
    char opStr[MAX_BUFFER];
    sprintf(opStr, "0x%x %p", POP, R0);
    printf("POP: [%s]\n", opStr);
    pushInstruction(opStr);
    return 0;
}

int pullInstruction() {
    istack = istack->next;
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
    while (istack != NULL) {
        if(writeToFile(istack->instruction) < 0) {
            return -1;
        }
        pullInstruction();
    }
    
    return 0;
}

int writeValues() {
    while (stack != NULL) {
        char opStr[MAX_BUFFER];
        sprintf(opStr, "0x%x %p %d", AFC, stack, stack->value.value);
        printf("AFC: [%s]\n", opStr);
        pushInstruction(opStr);
        pull();
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
    printf("%d -> %d[%s] at %p\n", a, stack->value.value, stack->value.name, (void*)stack);
    return 0;
}

int allocateVar(char *var) {   
    pushVar(var);
    printf("%s -> %d[%s] at %p\n", var, stack->value.value, stack->value.name, (void*)stack);
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
    var_addr->value.value = stack->value.value;
    printf("[UPDATE]%s -> %d[%s] at %p\n",  var_addr->value.name, var_addr->value.value, var_addr->value.name, (void*)var_addr);
    pull();

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

int getValue(char *name) {
    Stack *stack_tmp = getAddress(name);
    return stack_tmp != NULL ? stack_tmp->value.value : 0; // error code could be better
}

int addition() {
    popASM();
    
    char opStr1[MAX_BUFFER];
    sprintf(opStr1, "0x%x 0x%x 0x%x", ADD, SP, R0);
    printf("ADD: [%s]\n", opStr1);
    pushInstruction(opStr1);

    return 0;
}

int multiply() {
    Stack *addr1 = stack;
    pull();
    Stack *addr2 = stack;

    char opStr[MAX_BUFFER];
    sprintf(opStr, "0x%x %p %p", MUL, addr2, addr1);
    printf("MUL: [%s]\n", opStr);
    pushInstruction(opStr);

    return 0;
}

int divide() {
    Stack *addr1 = stack;
    pull();
    Stack *addr2 = stack;

    char opStr[MAX_BUFFER];
    sprintf(opStr, "0x%x %p %p", DIV, addr2, addr1);
    printf("DIV: [%s]\n", opStr);
    pushInstruction(opStr);

    return 0;
}

int substraction() {
    Stack *addr1 = stack;
    pull();
    Stack *addr2 = stack;

    char opStr[MAX_BUFFER];
    sprintf(opStr, "0x%x %p %p", SOU, addr2, addr1);
    printf("SOU: [%s]\n", opStr);
    pushInstruction(opStr);

    return 0;
}

int andOp() {
    Stack *addr1 = stack;
    pull();
    Stack *addr2 = stack;
    //TODO: write asm

    return 0;
}

int orOp() {
    Stack *addr1 = stack;
    pull();
    Stack *addr2 = stack;
    //TODO: write asm

    return 0;
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