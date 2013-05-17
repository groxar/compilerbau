/* 
 * parser.y - Parser utility for the DHBW compiler
 */
 
%{	
	// Project-specific includes
	#include "diag.h"
    #include "string.h"
    #include "stdio.h"
    #include "symboltable.h"
    #include "ir_generator.h"
    #include "typcheck.h" 
    #define voidCheck(a) (a!=0&&a->var_type!=VOID)?(a->size==1?a:yyerror("Reference access not allowed")):yyerror("Operations arent allowed on void")
%}

%union {
  int n;
  char* id;
  int regCnr;
  int lblCnr;
  struct scope_list *s;
}

%{
	static int n_para;
    scope_list_t* crntFunc = (scope_list_t*)0;
    scope_list_t* callFunc = (scope_list_t*)0;
    scope_list_t* callFuncPara = (scope_list_t*)0;
    char buffer[100];
%}
 
// Verbose error messages
%error-verbose

%locations
%start program

// Compiler in debugging informatirr
%debug

// Enable debug output
%initial-action
{
	yydebug = 0;
    initSymboltable();
    initIR();
};


/*
 * One shift/reduce conflict is expected for the "dangling-else" problem. This
 * conflict however is solved by the default behavior of bison for shift/reduce 
 * conflicts (shift action). The default behavior of bison corresponds to what
 * we want bison to do: SHIFT if the lookahead is 'ELSE' in order to bind the 'ELSE' to
 * the last open if-clause. 
 */
%expect 1

%token DO WHILE
%token IF ELSE
%token INT VOID
%token RETURN

%token COLON COMMA SEMICOLON

%token BRACE_OPEN BRACE_CLOSE
%token BRACKET_OPEN BRACKET_CLOSE
%token PARA_OPEN PARA_CLOSE

%token ID
%token NUM

%token ASSIGN 
%token LOGICAL_OR
%token LOGICAL_AND
%token EQ NE     
%token LS LSEQ GTEQ GT 
%token SHIFT_LEFT SHIFT_RIGHT
%token PLUS MINUS     
%token MUL DIV MOD
%token LOGICAL_NOT UNARY_MINUS UNARY_PLUS

%right ASSIGN
%left LOGICAL_OR
%left LOGICAL_AND
%left EQ NE
%left LS LSEQ GTEQ GT
%left SHIFT_LEFT SHIFT_RIGHT
%left PLUS MINUS
%left MUL DIV MOD
%right UNARY_MINUS UNARY_PLUS LOGICAL_NOT
%left BRACKET_OPEN BRACKET_CLOSE

%type<n> type
%type<id> ID
%type<n> NUM
%type<n> variable_declaration
%type<n> identifier_declaration
/*ir code*/
%type<s> expression
%type<s> primary
%type<s> function_call
%type<s> function_call_parameters
%type<id> function_begin



%%

program
     : program_element_list
     ;

program_element_list
     : program_element_list program_element 
     | program_element 
     ;

program_element
     : variable_declaration SEMICOLON
     | function_declaration SEMICOLON
     | function_definition
     | SEMICOLON
     ;
									
type
     : INT { $$ = INT; }
     | VOID { $$ = VOID; }
     ;

variable_declaration
     : variable_declaration COMMA ID BRACKET_OPEN NUM BRACKET_CLOSE { $$=variable_declaration_tc($1,$3,$5); } 
     | variable_declaration COMMA ID { $$=variable_declaration_tc($1,$3,1); }
     | identifier_declaration {$$ = $1;}
     ;

identifier_declaration
     : type ID BRACKET_OPEN NUM BRACKET_CLOSE { variable_declaration_tc($1,$2,$4); }
     | type ID { $$=variable_declaration_tc($1,$2,1); }
     ;

function_definition
     : function_begin PARA_CLOSE BRACE_OPEN {function_definition_tc1($1, n_para, crntFunc);} 
       stmt_list BRACE_CLOSE {function_definition_tc($1, crntFunc);}
     | function_begin function_parameter_list PARA_CLOSE BRACE_OPEN {function_definition_tc1($1, n_para, crntFunc);} 
       stmt_list BRACE_CLOSE  {function_definition_tc($1, crntFunc);}
     ;

function_declaration
     : function_begin PARA_CLOSE { function_declaration_tc($1, n_para); }
     | function_begin function_parameter_list PARA_CLOSE { function_declaration_tc($1, n_para); }
     ;

function_begin
     : type ID PARA_OPEN { n_para = 0; $$=function_begin_tc($1,$2); } 
    
     ;

function_parameter_list
     : function_parameter { n_para++; }
     | function_parameter_list COMMA function_parameter {n_para++;}
     ;
	
function_parameter
     : identifier_declaration 
     ;
									
stmt_list
     : /* empty: epsilon */
     | stmt_list stmt
     ;

stmt
     : stmt_block
     | variable_declaration SEMICOLON
     | expression SEMICOLON
     | stmt_conditional
     | stmt_loop
     | RETURN expression { return_tc($2->var_type, crntFunc); }SEMICOLON
     | RETURN { return_tc2(crntFunc); }SEMICOLON
     | SEMICOLON /* empty statement */
     ;

stmt_block
     : BRACE_OPEN stmt_list BRACE_CLOSE
     ;
	
stmt_conditional
     : stmt_begin 
     | stmt_begin ELSE { backPatch(1); gotoIR(OP_GO,NULL,NULL); frontPatch(1);} stmt {trackUnsetGoto()->firstPara = genLabel();}
     ;

stmt_begin
     : IF PARA_OPEN expression PARA_CLOSE { gotoIR(OP_GOF,NULL,voidCheck($3)); } stmt { trackUnsetGoto()->firstPara = genLabel();}
     ;
									
stmt_loop
     : WHILE PARA_OPEN expression PARA_CLOSE {gotoIR(OP_GOF,NULL,voidCheck($3));gotoIR(OP_GOT,genLabel(),voidCheck($3)); backPatch(1);} stmt {trackUnsetGoto()->firstPara = genLabel();frontPatch(1);} 
     | DO {gotoIR(OP_GOT,genLabel(),NULL); backPatch(1);} stmt WHILE PARA_OPEN expression PARA_CLOSE SEMICOLON {frontPatch(1);trackUnsetGoto()->secondPara = voidCheck($6); }
     ;
									
expression
     : expression ASSIGN expression             {$$ = assignIR(voidCheck($1),voidCheck($3));}
     | expression LOGICAL_OR expression         {$$ = calcIR(OP_LOR,voidCheck($1),voidCheck($3));}
     | expression LOGICAL_AND expression        {$$ = calcIR(OP_LAND,voidCheck($1),voidCheck($3));}
     | LOGICAL_NOT expression                   {$$ = calcIR(OP_EQ,genTemp(INT,0),voidCheck($2));}
     | expression EQ expression                 {$$ = calcIR(OP_EQ,voidCheck($1),voidCheck($3));}
     | expression NE expression                 {$$ = calcIR(OP_NE,voidCheck($1),voidCheck($3));}
     | expression LS expression                 {$$ = calcIR(OP_LT,voidCheck($1),voidCheck($3));}
     | expression LSEQ expression               {$$ = calcIR(OP_LE,voidCheck($1),voidCheck($3));} 
     | expression GTEQ expression               {$$ = calcIR(OP_GE,voidCheck($1),voidCheck($3));}
     | expression GT expression                 {$$ = calcIR(OP_GT,voidCheck($1),voidCheck($3));} 
     | expression PLUS expression               {$$ = calcIR(OP_ADD,voidCheck($1),voidCheck($3));}
     | expression MINUS expression              {$$ = calcIR(OP_SUB,voidCheck($1),voidCheck($3));}
     | expression SHIFT_LEFT expression         {$$ = calcIR(OP_SL,voidCheck($1),voidCheck($3));}
     | expression SHIFT_RIGHT expression        {$$ = calcIR(OP_SR,voidCheck($1),voidCheck($3));}
     | expression MUL expression                {$$ = calcIR(OP_MUL,voidCheck($1),voidCheck($3));}
     | expression MOD expression                {$$ = calcIR(OP_MOD,voidCheck($1),voidCheck($3));}
     | expression DIV expression                {$$ = calcIR(OP_DIV,voidCheck($1),voidCheck($3));}
     | MINUS expression %prec UNARY_MINUS       {$$ = calcIR(OP_SUB, genTemp(INT,0),voidCheck($2));}
     | PLUS expression %prec UNARY_PLUS         {$$ = voidCheck($2);}
     | ID BRACKET_OPEN primary BRACKET_CLOSE    {$$ = arrayLoadIR(getSymbol($1),$3);}
     | PARA_OPEN expression PARA_CLOSE          {$$ = $2;}
     | function_call                            {$$ = $1;}
     | primary                                  {$$ = $1;}
     ;



primary
     : NUM  { $$ = genTemp(INT,$1); }

     | ID   { $$ = getSymbol($1); } 
     ;

function_call
      : ID PARA_OPEN PARA_CLOSE { $$ = function_call_tc($1, callFunc); }
      | ID PARA_OPEN {n_para = 0; callFunc= getSymbol($1);} function_call_parameters PARA_CLOSE { $$ = function_call_tc2($1, callFunc, n_para, buffer); }
      ;

function_call_parameters
     : function_call_parameters COMMA expression {n_para++;}
     | expression { function_call_parameters_tc($1, n_para, buffer, callFunc, callFuncPara); n_para++; }
     ;

%%
void yyerror (const char *msg)
{
	FATAL_COMPILER_ERROR(INVALID_SYNTAX, 1, "(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}
