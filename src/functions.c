#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "functions.h"

int push(int val)
{
    char opStr[MAX_BUFFER];
    sprintf(opStr, "X\"%02x%02x%02x00\" -- @0x%02x => AFC 0x%02x %d", AFC, sp, val, i_addr, sp, val);
    sp++;
    printf("AFC: [%s]\n", opStr);
    return pushInstruction(opStr);
}

int copy(int src_addr, int dest_addr) {
    char opStr[MAX_BUFFER];
    sprintf(opStr, "X\"%02x%02x%02x00\" -- @0x%02x => COP 0x%02x 0x%02x", COP, dest_addr, src_addr, i_addr, dest_addr, src_addr);
    printf("COP: [%s]\n", opStr);
    
    return pushInstruction(opStr);
}

int pushCOP(Stack *var_addr)
{
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
    if(i_addr > MAX_INSTRUCTIONS-1) {
        printf("pushInstruction error: NO MORE SPACE LEFT! [%s]\n", instruction);
        return -1;
    }
    InstructionStack *element = malloc(sizeof(&istack));
    if(!element) {
        printf("pushInstruction error: EMPTY STACK! [%s]\n", instruction);
        return -1;
    }
    element->instruction = strndup(instruction, MAX_BUFFER);
    element->next = istack;
    istack = element;

    i_addr++;

    return 0;
}

int pushWhile(int addr)
{
    WhileStack *element = malloc(sizeof(&while_stack));
    if(!element)
        return -1;
    element->addr = addr;
    element->next = while_stack;
    while_stack = element;

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

int pullWhile() {
    while_stack = while_stack->next;
    return 0;
}

int initFile() {
    asm_file = fopen("./build/out.asm", "w");
    if (asm_file == NULL) {
        return -1;
    }

    stack = NULL;
    istack = NULL;
    while_stack = NULL;
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
        sprintf(str, "%sX\"00000000\" -- @0x%02x", (i_empty == MAX_INSTRUCTIONS-1 ? "  " : ", "), i_empty);
        if(writeToFile(str) < 0) {
            return -1;
        }
        i_empty--;
    }

    while (istack != NULL) {
        char str[MAX_BUFFER];
        sprintf(str, "%s%s", (i_addr == MAX_INSTRUCTIONS ? "  " : ", "), istack->instruction);
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
    printf("%d -> 0x%02x\n", a, sp-1);
    return 0;
}

int allocateVar(char *var) {   
    pushVar(var);
    printf("%s -> 0x%02x[%s]\n", var, stack->value.addr, stack->value.name);
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
    printf("[UPDATE]%s -> %02x[%s]\n",  var_addr->value.name, var_addr->value.addr, var_addr->value.name);

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
    sprintf(opStr, "X\"%02x%02x%02x%02x\" -- @0x%02x => %s 0x%02x 0x%02x 0x%02x", op_code, sp-1, sp-1, sp, i_addr, op, sp-1, sp-1 , sp);  // sp-1 is before last element
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
    sprintf(opStr, "JMPF_TMP_%d", depth); // sera remplacer plus tard dans la compilation
    printf("JMPF: [%s]\n", opStr);
    return pushInstruction(opStr);
}

int ifJump(int doElse) { // on execute cette fonction à la fin du body du if, et on remplace l'instruction laissé par la fonction précédente
    InstructionStack *istack_tmp = istack;
    char str[MAX_BUFFER];
    int instruction_cpt = 1;
    sprintf(str, "JMPF_TMP_%d", depth);
    while (istack_tmp != NULL && strcmp(istack_tmp->instruction, str) != 0) {
        istack_tmp = istack_tmp->next;
        instruction_cpt++;
    }

    if(istack_tmp == NULL) {
        printf("ifJump error\n");
        return -1;
    }
    
    if (doElse) {
        ifCond();
    }

    char opStr[MAX_BUFFER];
    sprintf(opStr, "X\"%02x%02x0000\" -- @0x%02x => JMPF 0x%02x", JMPF, i_addr, i_addr - (doElse ? 1 : 0) - instruction_cpt, i_addr);
    printf("JMPF: [%s]\n", opStr);
    istack_tmp->instruction = strndup(opStr, MAX_BUFFER);

    return 0;
}

int whileCond() {  // sert a retenir l'addresse de début de boucle (avant l'evaluation de la contidtion)
    return pushWhile(sp);
}

int whileJump(){
    char opStr[MAX_BUFFER];
    sprintf(opStr, "X\"%02x%02x0000\" -- @0x%02x => JMP 0x%02x", JMP, while_stack->addr, i_addr, while_stack->addr);
    pullWhile();
    printf("JMP: [%s]\n", opStr);
    return pushInstruction(opStr);
}