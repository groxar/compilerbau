/* 
 * scanner.l - Scanner utility for the DHBW compiler
 */

%{
	#include "parser.h"
    #include "string.h"
    #include "stdlib.h"

    //Track line and column numbers
    #define YY_USER_INIT { myColCnt = 0; }
    #define YY_USER_ACTION {                    \
    yylloc.first_line = yylineno;                \
    yylloc.last_line = yylineno;                 \
    yylloc.first_column = myColCnt;              \
    yylloc.last_column = myColCnt + yyleng - 1;  \
    myColCnt += yyleng; }
    int myColCnt;
%}

/* 
 * Default setting for flex which indicates that yytext, which is one of the main connections
 * between the scanner and parser, is handled as pointer of char instead of an array of char. 
*/
%pointer	

/* save the line number of terminals */
%option yylineno

/* finish after scanning the provided file */
%option noyywrap

/* Scanner: Error on unmatched input */
/* %option warn nodefault */

/* Flex: report on the scanner generator */
%option verbose

%%
"\n" { myColCnt = 0 ;}
do {return DO;}
while {return WHILE;}
if {return IF;}
else {return ELSE;}
int { yylval.n = INT; return INT;}
void {yylval.n = VOID; return VOID;}
return {return RETURN;}
\: {return COLON;}
\, {return COMMA;}
\; {return SEMICOLON;}
\[ {return BRACKET_OPEN;}
\] {return BRACKET_CLOSE;}
\{ {return BRACE_OPEN;}
\} {return BRACE_CLOSE;}
\( {return PARA_OPEN;}
\) {return PARA_CLOSE;}
[a-z][a-zA-z0-9_]* { yylval.id = strdup(yytext); return ID;}
0|[1-9][0-9]* { yylval.n = atoi(yytext); return NUM;}
\=\= {return EQ;}
\= {return ASSIGN;}
\|\| {return LOGICAL_OR;}
\&\& {return LOGICAL_AND;}
\!\= {return NE;}
\< {return LS;}
\<\= {return LSEQ;}
\>\= {return GTEQ;}
\> {return GT;}
\<< {return SHIFT_LEFT;}
\>> {return SHIFT_RIGHT;}
\+ {return PLUS;}
\- {return MINUS;}
\* {return MUL;}
\/ {return DIV;}
\% {return MOD;}
\! {return LOGICAL_NOT;}
//.*$ {}
/\*.*\*/ {}
