#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_


#define VARIABLE 0
#define FUNCTION 1

typedef struct symbol {
    int type;
} symbol_t;

int insertVariable(symbol_t* variable);
int insertFuncition(symbol_t* function);
symbol_t * getSymbol(char* name);

void beginFunction(void);
void endFunction(void);



#endif
