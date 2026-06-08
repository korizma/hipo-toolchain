/* Lekser za Asembler */

%{
#include "grammar.tab.h"
%}

/* Definicije */

NEWLINE     [\n|(\r\n)]
DOT         \.
COMMA       \,
DOLLAR      $
PERCENT     %
RBRACK      [
LBRACK      ]
PLUS        +
MINUS       -
MULT        *
DIVISION    /


GLOBAL  global
EXTERN  extern
SECTION section
WORD    word
SKIP    skip
ASCII   ascii
EQU     equ
END     end

HALT    halt
INT     int
IRET    iret
CALL    call
RET     ret
JMP     jmp
BEQ     beq
BNE     bne
BGT     bgt
PUSH    push
POP     pop
XCHG    xchg
ADD     add
SUB     sub
MUL     mul
DIV     div
NOT     not
AND     and
OR      or
XOR     xor
SHL     shl
SHR     shr
LD      ld
ST      st
CSRRD   csrrd
CSRWR   csrwr

GPRX    r([0-9]|1[0-5])
CSRX    (status)|(handler)|(cause)

STRING  \'[a-z]+\'
SYMBOL  [a-zA-Z][a-zA-Z_0-9]*
LITERAL  [0-9]+

COMMENT #.*\n


%%
    /* Pravila */

[\t\r ]+ return ;
\n       return NEWLINE;
"."      return DOT;
","      return COMMA;
"%"      return PERCENT;
"["      return RBRACK;
"]"      return LBRACK;
"+"      return PLUS;
"-"      return MINUS;
"*"      return MULT;
"/"      return DIVISION;
"$"      return DOLLAR;
{GLOBAL}  return GLOBAL;
{EXTERN}  return EXTERN;
{SECTION} return SECTION;
{WORD}    return WORD;
{SKIP}    return SKIP;
{ASCII}   return ASCII;
{EQU}     return EQU;
{END}     return END;
{HALT}    return HALT;
{INT}     return INT;
{IRET}    return IRET;
{CALL}    return CALL;
{RET}     return RET;
{JMP}     return JMP;
{BEQ}     return BEQ;
{BNE}     return BNE;
{BGT}     return BGT;
{PUSH}    return PUSH;
{POP}     return POP;
{XCHG}    return XCHG;
{ADD}     return ADD;
{SUB}     return SUB;
{MUL}     return MUL;
{DIV}     return DIV;
{NOT}     return NOT;
{AND}     return AND;
{OR}      return OR;
{XOR}     return XOR;
{SHL}     return SHL;
{SHR}     return SHR;
{LD}      return LD;
{ST}      return ST;
{CSRRD}   return CSRRD;
{CSRWR}   return CSRWR;
{GPRX}    return GPRX;
{CSRX}    return CSRX;
{COMMENT} return COMMENT;
{STRING}  return STRING;
{LITERAL}  return LITERAL;
{SYMBOL}  return SYMBOL;

%%
/* C kod*/

int yywrap() {
    return 1;
}
