/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     DO = 258,
     WHILE = 259,
     IF = 260,
     ELSE = 261,
     INT = 262,
     VOID = 263,
     RETURN = 264,
     COLON = 265,
     COMMA = 266,
     SEMICOLON = 267,
     BRACE_OPEN = 268,
     BRACE_CLOSE = 269,
     BRACKET_OPEN = 270,
     BRACKET_CLOSE = 271,
     PARA_OPEN = 272,
     PARA_CLOSE = 273,
     ID = 274,
     NUM = 275,
     ASSIGN = 276,
     LOGICAL_OR = 277,
     LOGICAL_AND = 278,
     EQ = 279,
     NE = 280,
     LS = 281,
     LSEQ = 282,
     GTEQ = 283,
     GT = 284,
     SHIFT_LEFT = 285,
     SHIFT_RIGHT = 286,
     PLUS = 287,
     MINUS = 288,
     MUL = 289,
     DIV = 290,
     MOD = 291,
     LOGICAL_NOT = 292,
     UNARY_MINUS = 293,
     UNARY_PLUS = 294
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 13 "src/parser.y"

  int n;
  char* id;



/* Line 2068 of yacc.c  */
#line 96 "bin/parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;
