

%{
#include "../asm/code.h"
#include "helper.c"
int yylex(void);
void yyerror(const char *message);
%}

%union {
    int literal;
    char* reg;
    char* symbol;
    char* string;

    asm_line* line;
    operand_jmp o_jmp;
    operand_ls o_ls;
    sym_or_lit* sym_lit_list;
    gpr_pair gprs;
}

%token <literal> LITERAL
%token <symbol> SYMBOL
%token <string> STRING
%token <reg> GPRX
%token <reg> CSRX

%type <o_jmp> operandJmp
%type <o_ls> operandLS
%type <gprs> grp_12
%type <line> asm_directives 
%type <line> asm_instructions

    /* Deklaracije */

%token GLOBAL EXTERN SECTION WORD SKIP ASCII EQU END
%token HALT INT IRET CALL RET JMP BEQ BNE BGT
%token PUSH POP XCHG
%token ADD SUB MUL DIV
%token NOT AND OR XOR SHL SHR
%token LD ST CSRRD CSRWR
%token WHITESPACE NEWLINE
%token DOT COMMA PERCENT RBRACK LBRACK DOLLAR
%token PLUS MINUS MULT DIVISION
%token GPRX CSRX SYMBOL LITERAL STRING COMMENT

%start input

%%
    /* Pravila gramatike */

input:
    | line input
    ;

line:
        NEWLINE
    |   COMMENT
    |   DOT asm_directives NEWLINE
    |   asm_instructions NEWLINE
    ;

asm_directives: 
        GLOBAL symbol_list
    |   EXTERN symbol_list
    |   SECTION SYMBOL
    |   WORD sym_lit_list
    |   SKIP LITERAL
    |   ASCII STRING
    |   EQU SYMBOL COMMA expr
    |   END
    ;

symbol_list: 
        SYMBOL | SYMBOL COMMA symbol_list
        ;

sym_lit_list:
        LITERAL | SYMBOL | SYMBOL COMMA sym_lit_list | LITERAL COMMA sym_lit_list 
        ;

expr:
        LITERAL
        ;

asm_instructions:
        HALT {
                asm_line* line = new_empty_line(); 
                line.is_instruction = true; 
                line.operation = "halt"; 
                add_line(line);        
            }
    |   INT {
                asm_line* line = new_empty_line(); 
                line.is_instruction = true; 
                line.operation = "int"; 
                add_line(line);        
            }
    |   IRET {
                asm_line* line = new_empty_line(); 
                line.is_instruction = true; 
                line.operation = "iret"; 
                add_line(line);        
            }
    |   RET {
                asm_line* line = new_empty_line(); 
                line.is_instruction = true; 
                line.operation = "ret"; 
                add_line(line);        
            }
    |   CALL operandJmp
    |   JMP operandJmp
    |   branchOp gpr_12 COMMA operandJmp
    |   PUSH gpr_1 | POP gpr_1 
    |   NOT gpr_1
    |   arithmethicOp gpr_12
    |   LD operandLS COMMA gpr_1
    |   ST gpr_1 COMMA operandLS
    |   CSRRD csr_1 COMMA gpr_1
    |   CSRWR gpr_1 COMMA csr_1

gpr_12:
    PERCENT GPRX COMMA PERCENT GPRX {$$.reg1 = $2}
    ;

gpr_1:
    PERCENT GPRX {$$ = $2}
    ;

csr_1:
    PERCENT CSRX {$$ = $2}
    ;

branchOp:
    BNE | BEQ | BGT
    ;

arithmethicOp:
    XCHG | ADD | SUB | MUL | DIV | AND | OR | XOR | SHL | SHR
    ;

operandJmp:
    LITERAL | SYMBOL
    ;

operandLS:
        DOLLAR LITERAL | DOLLAR SYMBOL
    |   LITERAL | SYMBOL
    |   PERCENT GPRX | PERCENT CSRX
    |   RBRACK PERCENT GPRX LBRACK
    |   RBRACK PERCENT CSRX LBRACK
    |   RBRACK PERCENT GPRX PLUS LITERAL LBRACK
    |   RBRACK PERCENT CSRX PLUS LITERAL LBRACK
    |   RBRACK PERCENT GPRX PLUS SYMBOL LBRACK
    |   RBRACK PERCENT CSRX PLUS SYMBOL LBRACK
    ;


%%

    /* C kod */
