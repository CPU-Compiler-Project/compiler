#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "functions.h"

// en fait on a pas implementer de push en VHDL, il faut qu'on gère les adresses memoire
// du fpga. Elles sont numérotées de 0 a 15 dans notre implémentation vhdl et chaque registre fait 1 octet
// Pour savoir l'addresse du premier registre disponible, on a l'int sp qui pointe vers la première adresse dispo

int push(int val)
{
    char opStr[MAX_BUFFER];
    sprintf(opStr, "X\"%02x%02x%02x00\" -- @%d => STORE 0x%02x %d", STORE, sp, val, i_addr, sp, val);
    i_addr++;
    sp++;
    printf("PUSH: [%s]\n", opStr);
    pushInstruction(opStr);
    return 0;
}

// aucune idée de comment marche ce truc avec le fpga
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

// je suis pas sur de ce que fait celle la, pour moi faudrait modifier la stack aussi
int popASM() {
    char opStr[MAX_BUFFER];
    sprintf(opStr, "0x%x", POP);
    printf("POP: [%s]\n", opStr);
    pushInstruction(opStr);
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


// TODO: je pense que le sprintf n'est pas bon
int writeValues() {
    while (stack != NULL) {
        char opStr[MAX_BUFFER];
        //sprintf(opStr, "0x%x %p %d", AFC, stack, stack->value.addr);
        sprintf(opStr, "0x%x %p %d", AFC, stack, stack->value.addr);
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
    printf("%d -> %d[%s] at %p\n", a, stack->value.addr, stack->value.name, (void*)stack);
    return 0;
}

int allocateVar(char *var) {   
    pushVar(var);
    printf("%s -> %d[%s] at %p\n", var, stack->value.addr, stack->value.name, (void*)stack);
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
    // instruction de changement de valeur a l'addresse var_addr
    printf("[UPDATE]%s -> %d[%s] at %p\n",  var_addr->value.name, var_addr->value.addr, var_addr->value.name, (void*)var_addr);
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
        printf("getAddress error not found %s \n", name);
        return NULL; //TODO
    }

    return stack_tmp;
}

int getValue(char *name) {
    Stack *stack_tmp = getAddress(name);
    return stack_tmp != NULL ? stack_tmp->value.addr : 0; // error code could be better
}

// je capte pas pourquoi on fait appel a popASM, pour moi il faut réutiliser la structure des op suivantes
int addition() {
    popASM();
    
    char opStr1[MAX_BUFFER];
    //sprintf(opStr1, "0x%x 0x%x 0x%x", ADD, SP, R0);
    printf("ADD: [%s]\n", opStr1);
    pushInstruction(opStr1);

    return 0;
}

int multiply() {
    Stack *addr1 = stack;
    pull();
    Stack *addr2 = stack;

    char opStr[MAX_BUFFER];
    //sprintf(opStr, "0x%x %p %p", MUL, addr2, addr1);
    sprintf(opStr, "0x%x %d %d", MUL, addr2->value.addr, addr1->value.addr);
    printf("MUL: [%s]\n", opStr);
    pushInstruction(opStr);

    return 0;
}

int divide() {
    Stack *addr1 = stack;
    pull();
    Stack *addr2 = stack;

    char opStr[MAX_BUFFER];
    //sprintf(opStr, "0x%x %p %p", DIV, addr2, addr1);
    sprintf(opStr, "0x%x %d %d", DIV, addr2->value.addr, addr1->value.addr);
    printf("DIV: [%s]\n", opStr);
    pushInstruction(opStr);

    return 0;
}

int substraction() {
    Stack *addr1 = stack;
    pull();
    Stack *addr2 = stack;

    char opStr[MAX_BUFFER];
    //sprintf(opStr, "0x%x %p %p", SOU, addr2, addr1);
    sprintf(opStr, "0x%x %d %d", SOU, addr2->value.addr, addr1->value.addr);
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