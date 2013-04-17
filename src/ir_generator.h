enum opcodes {
	OP_ASSIGN,
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_MINUS,
	OP_IFEQ,
	OP_IFNE,
	OP_IFGT,
	OP_IFGE,
	OP_IFLS,
	OP_IFLE,
	OP_GOTO,
	OP_RETURN,
	OP_RETURN_OPNULL,
	OP_CALL_ONE,
	OP_CALL_TWO,
	OP_ARRAY_LD,
	OP_ARRAY_ST,
	OP_LOGICAL_OR,
	OP_LOGICAL_AND,
	OP_LOGICAL_NOT
};

typedef struct symbol {
    const char *name;			//symbol name (unique in each table)
    int isFunc;					//not 0 if symbol is a function
    union{						//each symbol is either a function or a variable
    	struct variable var;
    	struct function func;
    }is;
    struct symbol *next;		//points to the next list element
} symbol;
