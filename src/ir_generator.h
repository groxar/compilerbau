enum opcodes {
	ASSIGN,
	PLUS,
	MINUS,
	MUL,
	DIV,
	MOD,
	UNARY_MINUS,
	UNARY_PLUS,
	EQ,
	NE,
	GT,
	GTEQ,
	LS,
	LSEQ,
	OP_GOTO,
	OP_RETURN,
	OP_RETURN_OPNULL,
	OP_CALL_ONE,
	OP_CALL_TWO,
	OP_ARRAY_LD,
	OP_ARRAY_ST,
	LOGICAL_OR,
	LOGICAL_AND,
	LOGICAL_NOT
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
