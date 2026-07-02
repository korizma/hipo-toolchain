/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_MISC_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_MISC_GRAMMAR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 11 "misc/grammar.y"

#include "helper.hpp"

#line 53 "misc/grammar.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    LITERAL = 258,                 /* LITERAL  */
    SYMBOL = 259,                  /* SYMBOL  */
    STRING = 260,                  /* STRING  */
    GPRX = 261,                    /* GPRX  */
    CSRX = 262,                    /* CSRX  */
    GLOBAL = 263,                  /* GLOBAL  */
    EXTERN = 264,                  /* EXTERN  */
    SECTION = 265,                 /* SECTION  */
    WORD = 266,                    /* WORD  */
    SKIP = 267,                    /* SKIP  */
    ASCII = 268,                   /* ASCII  */
    EQU = 269,                     /* EQU  */
    END = 270,                     /* END  */
    HALT = 271,                    /* HALT  */
    INT = 272,                     /* INT  */
    IRET = 273,                    /* IRET  */
    CALL = 274,                    /* CALL  */
    RET = 275,                     /* RET  */
    JMP = 276,                     /* JMP  */
    BEQ = 277,                     /* BEQ  */
    BNE = 278,                     /* BNE  */
    BGT = 279,                     /* BGT  */
    PUSH = 280,                    /* PUSH  */
    POP = 281,                     /* POP  */
    XCHG = 282,                    /* XCHG  */
    ADD = 283,                     /* ADD  */
    SUB = 284,                     /* SUB  */
    MUL = 285,                     /* MUL  */
    DIV = 286,                     /* DIV  */
    NOT = 287,                     /* NOT  */
    AND = 288,                     /* AND  */
    OR = 289,                      /* OR  */
    XOR = 290,                     /* XOR  */
    SHL = 291,                     /* SHL  */
    SHR = 292,                     /* SHR  */
    LD = 293,                      /* LD  */
    ST = 294,                      /* ST  */
    CSRRD = 295,                   /* CSRRD  */
    CSRWR = 296,                   /* CSRWR  */
    WHITESPACE = 297,              /* WHITESPACE  */
    NEWLINE = 298,                 /* NEWLINE  */
    DOT = 299,                     /* DOT  */
    COMMA = 300,                   /* COMMA  */
    PERCENT = 301,                 /* PERCENT  */
    RBRACK = 302,                  /* RBRACK  */
    LBRACK = 303,                  /* LBRACK  */
    DOLLAR = 304,                  /* DOLLAR  */
    COLON = 305,                   /* COLON  */
    PLUS = 306,                    /* PLUS  */
    MINUS = 307,                   /* MINUS  */
    MULT = 308,                    /* MULT  */
    DIVISION = 309,                /* DIVISION  */
    COMMENT = 310,                 /* COMMENT  */
    UMINUS = 311                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 15 "misc/grammar.y"

    long literal;
    char* reg;
    char* symbol;
    char* string;
    char instruction;
    char reg_id;

    s_gpr_pair gprs;
    s_expr* expr;
    s_symbol_list* symbol_list;
    s_sym_lit_list* sym_lit_list;
    s_parser_jmp_operand* jmp_operand;
    s_parser_ls_operand* ls_operand;

#line 142 "misc/grammar.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_MISC_GRAMMAR_TAB_H_INCLUDED  */
