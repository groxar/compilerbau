/* 
 * parser.y - Parser utility for the DHBW compiler
 */
 
%{	
	// Project-specific includes
	#include "diag.h"
    #include "string.h"
    #include "stdio.h"
    #include "symboltable.h"
%}

%union {
  int n;
  char* id;
  int regCnr;
  int lblCnr;
}

%{
    static int n_para;
%}
 
// Verbose error messages
%error-verbose

%locations
%start program

// Compiler in debugging information
%debug

// Enable debug output
%initial-action
{
	yydebug = 1;
    initSymboltable();
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
<<<<<<< HEAD
%type<n> variable_declaration
%type<n> identifier_declaration
=======
%type<regCnr> expression
>>>>>>> 35bd73c1e1a074ab6496e136e7eb1ec8fb8780ba

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
     : variable_declaration COMMA ID BRACKET_OPEN NUM BRACKET_CLOSE { if(!insertSymbol(VAR,$1,$3,0,$5))
        {
            ;
        }
     $$ = $1; } 
     | variable_declaration COMMA ID { if(!insertSymbol(VAR,$1,$3,0,1))
        {

        }
     $$ = $1; }
     | identifier_declaration {$$ = $1}
     ;

identifier_declaration
     : type ID BRACKET_OPEN NUM BRACKET_CLOSE {if(!insertSymbol(VAR,$1,$2,0, $4))
        {
            ;
        }
     }
     | type ID { if(!insertSymbol(VAR,$1,$2,0,1))
        {
            ;
        }
     $$ = $1; }
     ;

function_definition
     : function_begin PARA_CLOSE BRACE_OPEN stmt_list BRACE_CLOSE { if(!endFunction())
        {
            ;
        }
     }
     | function_begin function_parameter_list PARA_CLOSE BRACE_OPEN stmt_list BRACE_CLOSE { if(!endFunction())
        {
            ;
        }
     }
     ;

function_declaration
     : function_begin PARA_CLOSE { if(!endFunction())
        {
            ;
        }
     }
     | function_begin function_parameter_list PARA_CLOSE { if(!endFunction())
        {
            ;
        }
     }
     ;

function_begin
     : type ID PARA_OPEN {n_para = 0; if(!beginFunction($1,$2))
        {
            ;
        }
     }
     ;

function_parameter_list
     : function_parameter { n_para++; }
     | function_parameter_list COMMA function_parameter
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
     | RETURN expression SEMICOLON
     | RETURN SEMICOLON
     | SEMICOLON /* empty statement */
     ;

stmt_block
     : BRACE_OPEN stmt_list BRACE_CLOSE
     ;
	
stmt_conditional
     : IF PARA_OPEN expression PARA_CLOSE stmt
     | IF PARA_OPEN expression PARA_CLOSE stmt ELSE stmt
     ;
									
stmt_loop
     : WHILE PARA_OPEN expression PARA_CLOSE stmt
     | DO stmt WHILE PARA_OPEN expression PARA_CLOSE SEMICOLON
     ;
									
expression
     : expression ASSIGN expression {$$=$3; doAssign($1, $3);}
     | expression LOGICAL_OR expression {$$=doEval(LOGICAL_OR, $1, $3);}
     | expression LOGICAL_AND expression {$$=doEval(LOGICAL_AND, $1, $3);}
     | LOGICAL_NOT expression {$$=doEval(LOGICAL_NOT, $2, 0);}
     | expression EQ expression {$$=doEval(EQ, $1, $3);}
     | expression NE expression {$$=doEval(NE, $1, $3);}
     | expression LS expression  {$$=doEval(LS, $1, $3);}
     | expression LSEQ expression  {$$=doEval(LSEQ, $1, $3);}
     | expression GTEQ expression  {$$=doEval(GTEQ, $1, $3);}
     | expression GT expression {$$=doEval(GT, $1, $3);}
     | expression PLUS expression  {$$=doEval(PLUS, $1, $3);}
     | expression MINUS expression  {$$=doEval(MINUS, $1, $3);}
     | expression SHIFT_LEFT expression
     | expression SHIFT_RIGHT expression
     | expression MUL expression  {$$=doEval(MUL, $1, $3);}
     | expression MOD expression  {$$=doEval(MOD, $1, $3);}
     | expression DIV expression   {$$=doEval(DIV, $1, $3);}
     | MINUS expression %prec UNARY_MINUS
     | PLUS expression %prec UNARY_PLUS
     | ID BRACKET_OPEN primary BRACKET_CLOSE
     | PARA_OPEN expression PARA_CLOSE
     | function_call /*{ $$ = sa_FunctionCall($1 , @1); } //$$ == yylval->symbol*/
     | primary
     ;

primary
     : NUM
     | ID
     ;

function_call
      : ID PARA_OPEN PARA_CLOSE
      | ID PARA_OPEN function_call_parameters PARA_CLOSE
      ;

function_call_parameters
     : function_call_parameters COMMA expression
     | expression
     ;
%%

void yyerror (const char *msg)
{
	FATAL_COMPILER_ERROR(INVALID_SYNTAX, 1, "(%d.%d-%d.%d): %s\n", yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column, msg);
}
