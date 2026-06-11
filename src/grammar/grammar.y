

%{
#include "../asm/code.h"
#include "helper.h"
int yylex(void);
void yyerror(const char *message);
%}

%code requires {
#include "../asm/code.h"
#include "helper.h"
}


%union {
    int literal;
    char* reg;
    char* symbol;
    char* string;
    asm_instruction opB;
    asm_register gprC;

    s_asm_line* line;
    s_operand_jmp o_jmp;
    s_operand_ls o_ls;
    s_gpr_pair gprs;
    s_expr e;
    s_list_n_sl listn_sl;
    s_list_n_s listn_s;
}

%token <literal> LITERAL
%token <symbol> SYMBOL
%token <string> STRING
%token <reg> GPRX
%token <reg> CSRX

%type <o_jmp> operandJmp
%type <o_ls> operandLS
%type <gprs> gpr_12
%type <line> asm_directives 
%type <line> asm_instructions

%type <listn_sl> sym_lit_list
%type <listn_s> symbol_list
%type <e> expr
%type <opB> branchOp
%type <opB> arithmethicOp
%type <gprC> gpr_1
%type <gprC> csr_1
%type <symbol> symbol_name

    /* Deklaracije */

%token GLOBAL EXTERN SECTION WORD SKIP ASCII EQU END
%token HALT INT IRET CALL RET JMP BEQ BNE BGT
%token PUSH POP XCHG
%token ADD SUB MUL DIV
%token NOT AND OR XOR SHL SHR
%token LD ST CSRRD CSRWR
%token WHITESPACE NEWLINE
%token DOT COMMA PERCENT RBRACK LBRACK DOLLAR COLON
%token PLUS MINUS MULT DIVISION
%token COMMENT

%left PLUS MINUS
%left MULT DIVISION
%precedence UMINUS

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
    |   symbol_name COLON NEWLINE {
                s_asm_line* line = new_empty_line(); 
                line->is_label = true; 
                line->symbol = $1;
                add_line(line);        
            }
    ;

asm_directives: 
        GLOBAL symbol_list {
                s_asm_line* line = new_empty_line(); 
                line->is_directive = true; 
                line->directive = ASM_DIR_GLOBAL;
                reverse_arr_sym(&$2);
                line->symbol_list = $2.arr;
                line->symbol_list_n = $2.n;
                add_line(line);        
            }
    |   EXTERN symbol_list {
                s_asm_line* line = new_empty_line(); 
                line->is_directive = true; 
                line->directive = ASM_DIR_EXTERN;
                reverse_arr_sym(&$2);
                line->symbol_list = $2.arr;
                line->symbol_list_n = $2.n;
                add_line(line);        
            }
    |   SECTION symbol_name {
                s_asm_line* line = new_empty_line(); 
                line->is_directive = true; 
                line->directive = ASM_DIR_SECTION;
                line->section_name = $2;
                add_line(line);        
            }
    |   WORD sym_lit_list {
                s_asm_line* line = new_empty_line(); 
                line->is_directive = true; 
                line->directive = ASM_DIR_WORD;
                reverse_arr_sl(&$2);
                line->sym_or_lit_list = $2.arr;
                line->sym_or_lit_list_n = $2.n;
                add_line(line);        
            }
    |   SKIP LITERAL {
                s_asm_line* line = new_empty_line(); 
                line->is_directive = true; 
                line->directive = ASM_DIR_SKIP;
                line->byte_num = $2;
                add_line(line);        
            }
    |   ASCII STRING {
                s_asm_line* line = new_empty_line(); 
                line->is_directive = true; 
                line->directive = ASM_DIR_ASCII;
                line->ascii_string = $2;
                add_line(line);        
            }
    |   EQU symbol_name COMMA expr {
                s_asm_line* line = new_empty_line(); 
                line->is_directive = true; 
                line->directive = ASM_DIR_EQU;
                line->new_symbol = $2;
                line->expression = $4;
                add_line(line);        
            }
    |   END {
                s_asm_line* line = new_empty_line(); 
                line->is_directive = true; 
                line->directive = ASM_DIR_END;
                add_line(line);        
            }
    ;

symbol_list: 
        symbol_name {
            $$.n = 0;
            $$.curr_size = 10;
            $$.arr = (char**)malloc(sizeof(char*)*$$.curr_size);
            add_to_list_sym_list(&$$, $1);
        }
        | symbol_name COMMA symbol_list {
            add_to_list_sym_list(&$3, $1);
            $$ = $3;
        }
        ;

sym_lit_list:
        LITERAL {
            $$.n = 0;
            $$.curr_size = 10;
            $$.arr = (s_sym_or_lit**)malloc(sizeof(s_sym_or_lit*)*$$.curr_size);
            s_sym_or_lit* novi = new_lit_sl($1);
            add_to_list_sl_list(&$$, novi);
        }
        | symbol_name {
            $$.n = 0;
            $$.curr_size = 10;
            $$.arr = (s_sym_or_lit**)malloc(sizeof(s_sym_or_lit*)*$$.curr_size);
            s_sym_or_lit* novi = new_sym_sl($1);
            add_to_list_sl_list(&$$, novi);
        }
        | symbol_name COMMA sym_lit_list {
            add_to_list_sl_list(&$3, new_sym_sl($1));
            $$ = $3;
        }
        | LITERAL COMMA sym_lit_list {
            add_to_list_sl_list(&$3, new_lit_sl($1));
            $$ = $3;
        }
        ;

expr:
        LITERAL {
            $$.kind = EXPR_LITERAL;
            $$.literal = $1;
            $$.symbol = 0;
            $$.left = 0;
            $$.right = 0;
        }
    |   symbol_name {
            s_expr* e = expr_symbol($1);
            $$ = *e;
        }
    |   expr PLUS expr {
            s_expr* e = expr_binary(EXPR_ADD, expr_literal($1.literal), expr_literal($3.literal));
            *e->left = $1;
            *e->right = $3;
            $$ = *e;
        }
    |   expr MINUS expr {
            s_expr* e = expr_binary(EXPR_SUB, expr_literal($1.literal), expr_literal($3.literal));
            *e->left = $1;
            *e->right = $3;
            $$ = *e;
        }
    |   expr MULT expr {
            s_expr* e = expr_binary(EXPR_MUL, expr_literal($1.literal), expr_literal($3.literal));
            *e->left = $1;
            *e->right = $3;
            $$ = *e;
        }
    |   expr DIVISION expr {
            s_expr* e = expr_binary(EXPR_DIV, expr_literal($1.literal), expr_literal($3.literal));
            *e->left = $1;
            *e->right = $3;
            $$ = *e;
        }
    |   MINUS expr %prec UMINUS {
            s_expr* e = expr_binary(EXPR_NEG, expr_literal(0), 0);
            *e->left = $2;
            $$ = *e;
        }
        ;

symbol_name:
        SYMBOL { $$ = $1; }
    |   CSRX { $$ = $1; }
    |   GPRX { $$ = $1; }
    |   GLOBAL { $$ = "global"; }
    |   EXTERN { $$ = "extern"; }
    |   SECTION { $$ = "section"; }
    |   WORD { $$ = "word"; }
    |   SKIP { $$ = "skip"; }
    |   ASCII { $$ = "ascii"; }
    |   EQU { $$ = "equ"; }
    |   END { $$ = "end"; }
    |   HALT { $$ = "halt"; }
    |   INT { $$ = "int"; }
    |   IRET { $$ = "iret"; }
    |   CALL { $$ = "call"; }
    |   RET { $$ = "ret"; }
    |   JMP { $$ = "jmp"; }
    |   BEQ { $$ = "beq"; }
    |   BNE { $$ = "bne"; }
    |   BGT { $$ = "bgt"; }
    |   PUSH { $$ = "push"; }
    |   POP { $$ = "pop"; }
    |   XCHG { $$ = "xchg"; }
    |   ADD { $$ = "add"; }
    |   SUB { $$ = "sub"; }
    |   MUL { $$ = "mul"; }
    |   DIV { $$ = "div"; }
    |   NOT { $$ = "not"; }
    |   AND { $$ = "and"; }
    |   OR { $$ = "or"; }
    |   XOR { $$ = "xor"; }
    |   SHL { $$ = "shl"; }
    |   SHR { $$ = "shr"; }
    |   LD { $$ = "ld"; }
    |   ST { $$ = "st"; }
    |   CSRRD { $$ = "csrrd"; }
    |   CSRWR { $$ = "csrwr"; }
    ;

asm_instructions:
        HALT {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = ASM_INSTR_HALT;
                add_line(line);        
            }
    |   INT {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = ASM_INSTR_INT;
                add_line(line);        
            }
    |   IRET {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = ASM_INSTR_IRET;
                add_line(line);        
            }
    |   RET {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = ASM_INSTR_RET;
                add_line(line);        
            }
    |   CALL operandJmp {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = ASM_INSTR_CALL;
                line->o_jmp = $2;
                add_line(line);      
            }
    |   JMP operandJmp {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = ASM_INSTR_JMP;
                line->o_jmp = $2;
                add_line(line);      
            }
    |   branchOp gpr_12 COMMA operandJmp {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = $1;
                line->reg1 = $2.reg1;
                line->reg2 = $2.reg2;
                line->o_jmp = $4;
                add_line(line);      
            }
    |   PUSH gpr_1 {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = ASM_INSTR_PUSH;
                line->reg1 = $2;
                add_line(line);      
            }
    |   POP gpr_1 {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = ASM_INSTR_POP;
                line->reg1 = $2;
                add_line(line);      
            }
    |   NOT gpr_1 {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = ASM_INSTR_NOT;
                line->reg1 = $2;
                add_line(line);      
            }
    |   arithmethicOp gpr_12 {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = $1;
                line->reg1 = $2.reg1;
                line->reg2 = $2.reg2;
                add_line(line);      
            }
    |   LD operandLS COMMA gpr_1 {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = ASM_INSTR_LD;
                line->reg1 = $4;
                line->o_ls = $2;
                add_line(line);      
            }
    |   ST gpr_1 COMMA operandLS {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = ASM_INSTR_ST;
                line->reg1 = $2;
                line->o_ls = $4;
                add_line(line);      
            }
    |   CSRRD csr_1 COMMA gpr_1 {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = ASM_INSTR_CSRRD;
                line->reg1 = $2;
                line->reg2 = $4;
                add_line(line);      
            }
    |   CSRWR gpr_1 COMMA csr_1 {
                s_asm_line* line = new_empty_line(); 
                line->is_instruction = true; 
                line->instruction = ASM_INSTR_CSRWR;
                line->reg1 = $2;
                line->reg2 = $4;
                add_line(line);      
            }

gpr_12:
    PERCENT GPRX COMMA PERCENT GPRX {$$.reg1 = reg_from_name($2); $$.reg2 = reg_from_name($5);}
    ;

gpr_1:
    PERCENT GPRX {$$ = reg_from_name($2);}
    ;

csr_1:
    PERCENT CSRX {$$ = reg_from_name($2);}
    ;

branchOp:
        BNE {$$ = ASM_INSTR_BNE;}
    |   BEQ {$$ = ASM_INSTR_BEQ;}
    |   BGT {$$ = ASM_INSTR_BGT;}
    ;

arithmethicOp:
        XCHG {$$ = ASM_INSTR_XCHG;}
    |   ADD {$$ = ASM_INSTR_ADD;}
    |   SUB {$$ = ASM_INSTR_SUB;}
    |   MUL {$$ = ASM_INSTR_MUL;}
    |   DIV {$$ = ASM_INSTR_DIV;}
    |   AND {$$ = ASM_INSTR_AND;}
    |   OR {$$ = ASM_INSTR_OR;}
    |   XOR {$$ = ASM_INSTR_XOR;}
    |   SHL {$$ = ASM_INSTR_SHL;}
    |   SHR {$$ = ASM_INSTR_SHR;}
    ;

operandJmp:
        LITERAL {
            $$.is_literal = true;
            $$.is_symbol = false;
            $$.symbol = 0;
            $$.literal = $1;
        }
    |   symbol_name {
            $$.is_literal = false;
            $$.is_symbol = true;
            $$.symbol = $1;
            $$.literal = 0;
        }
    ;


operandLS:
        DOLLAR LITERAL {
            $$.has_dollar = $$.has_literal = true;
            $$.has_percent = $$.has_brackets = $$.has_symbol = false;
            $$.literal = $2;
        }
    |   DOLLAR symbol_name {
            $$.has_dollar = $$.has_symbol = true;
            $$.has_percent = $$.has_brackets = $$.has_literal = false;
            $$.symbol = $2;
        }
    |   LITERAL {
            $$.has_literal = true;
            $$.has_percent = $$.has_brackets = $$.has_symbol = $$.has_dollar = false;
            $$.literal = $1;
        }
    |   symbol_name {
            $$.has_symbol = true;
            $$.has_percent = $$.has_brackets = $$.has_literal = $$.has_dollar = false;
            $$.symbol = $1;
        }
    |   PERCENT GPRX {
            $$.has_percent = true;
            $$.has_symbol = $$.has_brackets = $$.has_literal = $$.has_dollar = false;
            $$.reg = reg_from_name($2);
        }
    |   PERCENT CSRX {
            $$.has_percent = true;
            $$.has_symbol = $$.has_brackets = $$.has_literal = $$.has_dollar = false;
            $$.reg = reg_from_name($2);
        }
    |   RBRACK PERCENT GPRX LBRACK {
            $$.has_percent = $$.has_brackets = true;
            $$.has_symbol = $$.has_literal = $$.has_dollar = false;
            $$.reg = reg_from_name($3);
        }
    |   RBRACK PERCENT CSRX LBRACK {
            $$.has_percent = $$.has_brackets = true;
            $$.has_symbol = $$.has_literal = $$.has_dollar = false;
            $$.reg = reg_from_name($3);
        }
    |   RBRACK PERCENT GPRX PLUS LITERAL LBRACK {
            $$.has_percent = $$.has_brackets = $$.has_literal = true;
            $$.has_symbol = $$.has_dollar = false;
            $$.reg = reg_from_name($3);
            $$.literal = $5;
        }
    |   RBRACK PERCENT CSRX PLUS LITERAL LBRACK {
            $$.has_percent = $$.has_brackets = $$.has_literal = true;
            $$.has_symbol = $$.has_dollar = false;
            $$.reg = reg_from_name($3);
            $$.literal = $5;
        }
    |   RBRACK PERCENT GPRX PLUS symbol_name LBRACK {
            $$.has_percent = $$.has_brackets = $$.has_symbol = true;
            $$.has_literal = $$.has_dollar = false;
            $$.reg = reg_from_name($3);
            $$.symbol = $5;
        }
    |   RBRACK PERCENT CSRX PLUS symbol_name LBRACK {
            $$.has_percent = $$.has_brackets = $$.has_symbol = true;
            $$.has_literal = $$.has_dollar = false;
            $$.reg = reg_from_name($3);
            $$.symbol = $5;
        }
    ;


%%

    /* C kod */
