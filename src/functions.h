#ifndef DEF_FUNCTIONS
#define DEF_FUNCTIONS

#define MAX_ELEMENTS 1000

int *getAddress(char *name);
int initFile();
int closeFile();

int *multiply(int *a, int *b);
int *divide(int *a, int *b);
int *substraction(int *a, int *b);
int *addition(int *a, int *b);
int *andOp(int *a, int *b);
int *orOp(int *a, int *b);

typedef struct List {
    int *hd;
    List *tl;
} List;

typedef struct Element {
    char *name;
    int *address;
} Element;

Element tab[MAX_ELEMENTS];
FILE *asm_file;

#endif // DEF_FUNCTIONS