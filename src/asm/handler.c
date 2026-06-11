#include "handler.h"
#include "handlers_impl.h"
#include <stdlib.h>
#include <string.h>

s_handlers_arr h_arr;
extern s_program p;

s_handler_entry init_handlers[] = {
    {HANDLER_LABEL, ASM_INSTR_NONE, ASM_DIR_NONE, handle_label},
    {HANDLER_INSTRUCTION, ASM_INSTR_HALT, ASM_DIR_NONE, handle_halt},
    {HANDLER_INSTRUCTION, ASM_INSTR_INT, ASM_DIR_NONE, handle_int},
    {HANDLER_INSTRUCTION, ASM_INSTR_IRET, ASM_DIR_NONE, handle_iret},
    {HANDLER_INSTRUCTION, ASM_INSTR_CALL, ASM_DIR_NONE, handle_call},
    {HANDLER_INSTRUCTION, ASM_INSTR_RET, ASM_DIR_NONE, handle_ret},
    {HANDLER_INSTRUCTION, ASM_INSTR_JMP, ASM_DIR_NONE, handle_jmp},
    {HANDLER_INSTRUCTION, ASM_INSTR_BEQ, ASM_DIR_NONE, handle_beq},
    {HANDLER_INSTRUCTION, ASM_INSTR_BNE, ASM_DIR_NONE, handle_bne},
    {HANDLER_INSTRUCTION, ASM_INSTR_BGT, ASM_DIR_NONE, handle_bgt},
    {HANDLER_INSTRUCTION, ASM_INSTR_PUSH, ASM_DIR_NONE, handle_push},
    {HANDLER_INSTRUCTION, ASM_INSTR_POP, ASM_DIR_NONE, handle_pop},
    {HANDLER_INSTRUCTION, ASM_INSTR_XCHG, ASM_DIR_NONE, handle_xchg},
    {HANDLER_INSTRUCTION, ASM_INSTR_ADD, ASM_DIR_NONE, handle_arthm},
    {HANDLER_INSTRUCTION, ASM_INSTR_SUB, ASM_DIR_NONE, handle_arthm},
    {HANDLER_INSTRUCTION, ASM_INSTR_MUL, ASM_DIR_NONE, handle_arthm},
    {HANDLER_INSTRUCTION, ASM_INSTR_DIV, ASM_DIR_NONE, handle_arthm},
    {HANDLER_INSTRUCTION, ASM_INSTR_NOT, ASM_DIR_NONE, handle_not},
    {HANDLER_INSTRUCTION, ASM_INSTR_AND, ASM_DIR_NONE, handle_logic},
    {HANDLER_INSTRUCTION, ASM_INSTR_OR, ASM_DIR_NONE, handle_logic},
    {HANDLER_INSTRUCTION, ASM_INSTR_XOR, ASM_DIR_NONE, handle_logic},
    {HANDLER_INSTRUCTION, ASM_INSTR_SHL, ASM_DIR_NONE, handle_sh},
    {HANDLER_INSTRUCTION, ASM_INSTR_SHR, ASM_DIR_NONE, handle_sh},
    {HANDLER_INSTRUCTION, ASM_INSTR_LD, ASM_DIR_NONE, handle_ld},
    {HANDLER_INSTRUCTION, ASM_INSTR_ST, ASM_DIR_NONE, handle_st},
    {HANDLER_INSTRUCTION, ASM_INSTR_CSRRD, ASM_DIR_NONE, handle_csrrd},
    {HANDLER_INSTRUCTION, ASM_INSTR_CSRWR, ASM_DIR_NONE, handle_csrwr},
    {HANDLER_DIRECTIVE, ASM_INSTR_NONE, ASM_DIR_GLOBAL, handle_global},
    {HANDLER_DIRECTIVE, ASM_INSTR_NONE, ASM_DIR_EXTERN, handle_extern},
    {HANDLER_DIRECTIVE, ASM_INSTR_NONE, ASM_DIR_SECTION, handle_s_section},
    {HANDLER_DIRECTIVE, ASM_INSTR_NONE, ASM_DIR_WORD, handle_word},
    {HANDLER_DIRECTIVE, ASM_INSTR_NONE, ASM_DIR_SKIP, handle_skip},
    {HANDLER_DIRECTIVE, ASM_INSTR_NONE, ASM_DIR_ASCII, handle_ascii},
    {HANDLER_DIRECTIVE, ASM_INSTR_NONE, ASM_DIR_EQU, handle_equ},
    {HANDLER_DIRECTIVE, ASM_INSTR_NONE, ASM_DIR_END, handle_end},
};

void init_handler_arr()
{
    h_arr.size = HANDLERS_START_SIZE;
    h_arr.entries = (s_handler_entry*)malloc(sizeof(s_handler_entry) * h_arr.size);
    h_arr.next_avail = 0;
}

void register_handler(e_handler_kind kind, e_asm_instruction inst, e_asm_directive direct, handler_f handler)
{
    if (h_arr.next_avail == h_arr.size) {
        h_arr.size += HANDLERS_INCREASE_SIZE;
        h_arr.entries = (s_handler_entry*)realloc(h_arr.entries, sizeof(s_handler_entry) * h_arr.size);
    }
    s_handler_entry* new_entry = (s_handler_entry*)malloc(sizeof(s_handler_entry));
    new_entry->kind = kind;
    new_entry->instruction = inst;
    new_entry->directive = direct;
    new_entry->handler = handler;
    h_arr.entries[h_arr.next_avail++] = new_entry;
}

void handle_line(s_asm_line* line)
{
    for (int i = 0; i < h_arr.next_avail; i++) 
    {
        s_handler_entry* entry = h_arr.entries[i];
        if (entry->kind == HANDLER_LABEL && line->is_label) 
        {
            entry->handler(line, p.curr_section);
            return;
        }
        else if (entry->kind == HANDLER_INSTRUCTION && line->is_instruction && entry->instruction == line->instruction) 
        {
            entry->handler(line, p.curr_section);
            return;
        }
        else if (entry->kind == HANDLER_DIRECTIVE && line->is_directive && entry->directive == line->directive) 
        {
            entry->handler(line, p.curr_section);
            return;
        }
    }
}

void register_init_handlers()
{
    for (int i = 0; i < sizeof(init_handlers) / sizeof(s_handler_entry); i++) 
    {
        register_handler_struct(&init_handlers[i]);
    }
}


void register_handler_struct(s_handler_entry* entry)
{
    if (h_arr.next_avail == h_arr.size) {
        h_arr.size += HANDLERS_INCREASE_SIZE;
        h_arr.entries = (s_handler_entry*)realloc(h_arr.entries, sizeof(s_handler_entry) * h_arr.size);
    }
    h_arr.entries[h_arr.next_avail++] = entry;
}
