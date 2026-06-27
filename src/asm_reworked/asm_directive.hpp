#pragma once

#include <string>
#include <vector>

using namespace std;

struct s_expr;
struct s_error;

#define SL_SYM 0
#define SL_LIT 1

enum 
{
    ASM_DIR_NONE = 0,
    ASM_DIR_GLOBAL,
    ASM_DIR_EXTERN,
    ASM_DIR_SECTION,
    ASM_DIR_WORD,
    ASM_DIR_SKIP,
    ASM_DIR_ASCII,
    ASM_DIR_EQU,
    ASM_DIR_END
};

typedef struct s_sym_lit
{
    char type;
    string symbol;
    long literal;
} s_sym_lit;

typedef struct s_asm_directive
{
    char directive;

    // for .extern, .global
    vector<string> symbol_list;

    // for .ascii
    string ascii_string;

    // for .equ
    string equ_symbol;
    s_expr* expr;

    // for .skip
    long skip_literal;

    // for .section
    string section_symbol;

    // for .word
    vector<s_sym_lit> sym_lit_list;

} s_asm_directive;

s_asm_directive* new_asm_directive();

// forwards the directive to a handler, returns error msg or ""
s_error handle_asm_directive(s_asm_directive* directive);

// returns the asm_directive as a string
string asm_directive_to_string(s_asm_directive* directive);

s_error handle_global(s_asm_directive* directive);
s_error handle_extern(s_asm_directive* directive);
s_error handle_section(s_asm_directive* directive);
s_error handle_word(s_asm_directive* directive);
s_error handle_skip(s_asm_directive* directive);
s_error handle_ascii(s_asm_directive* directive);
s_error handle_equ(s_asm_directive* directive);
