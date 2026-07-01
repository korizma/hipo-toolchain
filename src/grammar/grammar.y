%define parse.error detailed
%define parse.lac full

%{
#include "helper.hpp"

int yylex(void);
void yyerror(const char *message);
%}

%code requires {
#include "helper.hpp"
}

%union {
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
}

%token <literal> LITERAL
%token <symbol> SYMBOL
%token <string> STRING
%token <reg> GPRX
%token <reg> CSRX

%type <jmp_operand> operandJmp
%type <ls_operand> operandLS
%type <gprs> gpr_12
%type <symbol_list> symbol_list
%type <sym_lit_list> sym_lit_list
%type <expr> expr
%type <instruction> branchOp
%type <instruction> arithmeticOp
%type <reg_id> gpr_1
%type <reg_id> csr_1
%type <symbol> symbol_name

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

input:
        %empty
    |   input line_with_comment
    ;

line_with_comment:
        line NEWLINE
    ;

line:
        %empty
    |   DOT asm_directives
    |   asm_instructions
    |   label
    |   label DOT asm_directives
    |   label asm_instructions
    ;

label:
        symbol_name COLON {
            parser_add_label($1);
        }
    ;

asm_directives:
        GLOBAL symbol_list {
            parser_add_symbol_list_directive(ASM_DIR_GLOBAL, $2);
        }
    |   EXTERN symbol_list {
            parser_add_symbol_list_directive(ASM_DIR_EXTERN, $2);
        }
    |   SECTION symbol_name {
            parser_add_section_directive($2);
        }
    |   WORD sym_lit_list {
            parser_add_word_directive($2);
        }
    |   SKIP LITERAL {
            parser_add_skip_directive($2);
        }
    |   ASCII STRING {
            parser_add_ascii_directive($2);
        }
    |   EQU symbol_name COMMA expr {
            parser_add_equ_directive($2, $4);
        }
    |   END {
            parser_add_end_directive();
        }
    ;

symbol_list:
        symbol_name {
            $$ = parser_new_symbol_list($1);
        }
    |   symbol_list COMMA symbol_name {
            parser_add_symbol_to_list($1, $3);
            $$ = $1;
        }
    ;

sym_lit_list:
        LITERAL {
            $$ = parser_new_sym_lit_list(new_lit_sl($1));
        }
    |   symbol_name {
            $$ = parser_new_sym_lit_list(new_sym_sl($1));
        }
    |   sym_lit_list COMMA symbol_name {
            parser_add_sym_lit_to_list($1, new_sym_sl($3));
            $$ = $1;
        }
    |   sym_lit_list COMMA LITERAL {
            parser_add_sym_lit_to_list($1, new_lit_sl($3));
            $$ = $1;
        }
    ;

expr:
        LITERAL {
            $$ = new_expression();
            add_literal_to_expression($$, $1);
        }
    |   symbol_name {
            $$ = new_expression();
            add_symbol_to_expression($$, $1, 1);
        }
    |   expr PLUS LITERAL {
            $$ = $1;
            add_literal_to_expression($$, $3);
        }
    |   expr PLUS symbol_name {
            $$ = $1;
            add_symbol_to_expression($$, $3, 1);
        }
    |   expr MINUS LITERAL {
            $$ = $1;
            add_literal_to_expression($$, -$3);
        }
    |   expr MINUS symbol_name {
            $$ = $1;
            add_symbol_to_expression($$, $3, -1);
        }
    |   MINUS LITERAL %prec UMINUS {
            $$ = new_expression();
            add_literal_to_expression($$, -$2);
        }
    |   MINUS symbol_name %prec UMINUS {
            $$ = new_expression();
            add_symbol_to_expression($$, $2, -1);
        }
    ;

symbol_name:
        SYMBOL { $$ = $1; }
    |   CSRX { $$ = $1; }
    |   GPRX { $$ = $1; }
    |   GLOBAL { $$ = parser_copy_symbol("global"); }
    |   EXTERN { $$ = parser_copy_symbol("extern"); }
    |   SECTION { $$ = parser_copy_symbol("section"); }
    |   WORD { $$ = parser_copy_symbol("word"); }
    |   SKIP { $$ = parser_copy_symbol("skip"); }
    |   ASCII { $$ = parser_copy_symbol("ascii"); }
    |   EQU { $$ = parser_copy_symbol("equ"); }
    |   END { $$ = parser_copy_symbol("end"); }
    |   HALT { $$ = parser_copy_symbol("halt"); }
    |   INT { $$ = parser_copy_symbol("int"); }
    |   IRET { $$ = parser_copy_symbol("iret"); }
    |   CALL { $$ = parser_copy_symbol("call"); }
    |   RET { $$ = parser_copy_symbol("ret"); }
    |   JMP { $$ = parser_copy_symbol("jmp"); }
    |   BEQ { $$ = parser_copy_symbol("beq"); }
    |   BNE { $$ = parser_copy_symbol("bne"); }
    |   BGT { $$ = parser_copy_symbol("bgt"); }
    |   PUSH { $$ = parser_copy_symbol("push"); }
    |   POP { $$ = parser_copy_symbol("pop"); }
    |   XCHG { $$ = parser_copy_symbol("xchg"); }
    |   ADD { $$ = parser_copy_symbol("add"); }
    |   SUB { $$ = parser_copy_symbol("sub"); }
    |   MUL { $$ = parser_copy_symbol("mul"); }
    |   DIV { $$ = parser_copy_symbol("div"); }
    |   NOT { $$ = parser_copy_symbol("not"); }
    |   AND { $$ = parser_copy_symbol("and"); }
    |   OR { $$ = parser_copy_symbol("or"); }
    |   XOR { $$ = parser_copy_symbol("xor"); }
    |   SHL { $$ = parser_copy_symbol("shl"); }
    |   SHR { $$ = parser_copy_symbol("shr"); }
    |   LD { $$ = parser_copy_symbol("ld"); }
    |   ST { $$ = parser_copy_symbol("st"); }
    |   CSRRD { $$ = parser_copy_symbol("csrrd"); }
    |   CSRWR { $$ = parser_copy_symbol("csrwr"); }
    ;

asm_instructions:
        HALT {
            parser_add_no_operand_instruction(ASM_INSTR_HALT);
        }
    |   INT {
            parser_add_no_operand_instruction(ASM_INSTR_INT);
        }
    |   IRET {
            parser_add_no_operand_instruction(ASM_INSTR_IRET);
        }
    |   RET {
            parser_add_no_operand_instruction(ASM_INSTR_RET);
        }
    |   CALL operandJmp {
            parser_add_jump_instruction(ASM_INSTR_CALL, $2);
        }
    |   JMP operandJmp {
            parser_add_jump_instruction(ASM_INSTR_JMP, $2);
        }
    |   branchOp gpr_12 COMMA operandJmp {
            parser_add_branch_instruction($1, $2, $4);
        }
    |   PUSH gpr_1 {
            parser_add_single_gpr_instruction(ASM_INSTR_PUSH, $2);
        }
    |   POP gpr_1 {
            parser_add_single_gpr_instruction(ASM_INSTR_POP, $2);
        }
    |   NOT gpr_1 {
            parser_add_single_gpr_instruction(ASM_INSTR_NOT, $2);
        }
    |   arithmeticOp gpr_12 {
            parser_add_two_gpr_instruction($1, $2);
        }
    |   LD operandLS COMMA gpr_1 {
            parser_add_ld_instruction($2, $4);
        }
    |   ST gpr_1 COMMA operandLS {
            parser_add_st_instruction($2, $4);
        }
    |   CSRRD csr_1 COMMA gpr_1 {
            parser_add_csrrd_instruction($2, $4);
        }
    |   CSRWR gpr_1 COMMA csr_1 {
            parser_add_csrwr_instruction($2, $4);
        }
    ;

gpr_12:
        PERCENT GPRX COMMA PERCENT GPRX {
            $$.reg1 = reg_from_name($2);
            $$.reg2 = reg_from_name($5);
        }
    ;

gpr_1:
        PERCENT GPRX {
            $$ = reg_from_name($2);
        }
    ;

csr_1:
        PERCENT CSRX {
            $$ = reg_from_name($2);
        }
    ;

branchOp:
        BNE { $$ = ASM_INSTR_BNE; }
    |   BEQ { $$ = ASM_INSTR_BEQ; }
    |   BGT { $$ = ASM_INSTR_BGT; }
    ;

arithmeticOp:
        XCHG { $$ = ASM_INSTR_XCHG; }
    |   ADD { $$ = ASM_INSTR_ADD; }
    |   SUB { $$ = ASM_INSTR_SUB; }
    |   MUL { $$ = ASM_INSTR_MUL; }
    |   DIV { $$ = ASM_INSTR_DIV; }
    |   AND { $$ = ASM_INSTR_AND; }
    |   OR { $$ = ASM_INSTR_OR; }
    |   XOR { $$ = ASM_INSTR_XOR; }
    |   SHL { $$ = ASM_INSTR_SHL; }
    |   SHR { $$ = ASM_INSTR_SHR; }
    ;

operandJmp:
        LITERAL {
            $$ = parser_new_jmp_literal($1);
        }
    |   symbol_name {
            $$ = parser_new_jmp_symbol($1);
        }
    ;

operandLS:
        DOLLAR LITERAL {
            $$ = parser_new_ls_operand(ASM_OP_LS_IMM_LIT, $2, NULL, ASM_REG_R0);
        }
    |   DOLLAR symbol_name {
            $$ = parser_new_ls_operand(ASM_OP_LS_IMM_SYM, 0, $2, ASM_REG_R0);
        }
    |   LITERAL {
            $$ = parser_new_ls_operand(ASM_OP_LS_MEM_LIT, $1, NULL, ASM_REG_R0);
        }
    |   symbol_name {
            $$ = parser_new_ls_operand(ASM_OP_LS_MEM_SYM, 0, $1, ASM_REG_R0);
        }
    |   PERCENT GPRX {
            $$ = parser_new_ls_operand(ASM_OP_LS_REG, 0, NULL, reg_from_name($2));
        }
    |   RBRACK PERCENT GPRX LBRACK {
            $$ = parser_new_ls_operand(ASM_OP_LS_REG_MEM, 0, NULL, reg_from_name($3));
        }
    |   RBRACK PERCENT GPRX PLUS LITERAL LBRACK {
            $$ = parser_new_ls_operand(ASM_OP_LS_REG_MEM_LIT, $5, NULL, reg_from_name($3));
        }
    |   RBRACK PERCENT GPRX PLUS symbol_name LBRACK {
            $$ = parser_new_ls_operand(ASM_OP_LS_REG_MEM_SYM, 0, $5, reg_from_name($3));
        }
    ;

%%
