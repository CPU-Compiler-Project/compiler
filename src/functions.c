#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "functions.h"


int push(int val)
{
    Stack *element = malloc(sizeof(&stack));
    if(!element)
        return -1;
    element->value = (Data) { .name = "tmp_anonymous_var", .value = val, .depth = depth };
    element->next = stack;
    stack = element;

    return 0;
}

int pushInstruction(char *instruction)
{
    InstructionStack *element = malloc(sizeof(&istack));
    if(!element)
        return -1;
    element->instruction = instruction;
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
    depth = 0;

    return 0;
}

int writeToFile(char *str) {
    if(asm_file == NULL || fprintf(asm_file, "%s\n", str) < 0) {
        return -1;
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

int addition() {
    Stack *addr1 = stack;
    pull();
    Stack *addr2 = stack;
    
    char addOp[MAX_BUFFER];
    sprintf(addOp, "0x%x %p %p", ADD, addr2, addr1);
    printf("add: [%s]\n", addOp);
    pushInstruction(addOp);
    writeToFile(addOp); // do this at the end?

    return 0;
}

int multiply() {
    int *addr1 = &stack->value.value;
    pull();
    int *addr2 = &stack->value.value;
    //TODO: write asm

    return 0;
}

int divide() {
    int *addr1 = &stack->value.value;
    pull();
    int *addr2 = &stack->value.value;
    //TODO: write asm (DIV addr2 addr1)

    return 0;
}

int substraction() {
    int *addr1 = &stack->value.value;
    pull();
    int *addr2 = &stack->value.value;
    //TODO: write asm

    return 0;
}

int andOp() {
    int *addr1 = &stack->value.value;
    pull();
    int *addr2 = &stack->value.value;
    //TODO: write asm

    return 0;
}

int orOp() {
    int *addr1 = &stack->value.value;
    pull();
    int *addr2 = &stack->value.value;
    //TODO: write asm

    return 0;
}