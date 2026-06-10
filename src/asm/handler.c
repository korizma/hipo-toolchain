#include "handler.h"
#include <stdlib.h>
#include <string.h>

handlers_arr h_arr;

static section* current_section = NULL;

static section* find_section(char* name)
{
    if (name == NULL) {
        return NULL;
    }

    for (int i = 0; i < p.number_of_sections; i++) {
        if (p.sections[i] != NULL && p.sections[i]->name != NULL &&
            strcmp(p.sections[i]->name, name) == 0) {
            return p.sections[i];
        }
    }

    return NULL;
}

void init_handler_arr()
{
    h_arr.size = HANDLERS_START_SIZE;
    h_arr.entries = (handler_entry*)malloc(sizeof(handler_entry) * h_arr.size);
    h_arr.next_avail = 0;
}

static void register_handler_entry(handler_entry entry)
{
    if (entry.handler == NULL) {
        return;
    }

    if (h_arr.entries == NULL || h_arr.size == 0) {
        init_handler_arr();
    }

    if (h_arr.next_avail == h_arr.size) {
        h_arr.size += HANDLERS_INCREASE_SIZE;
        h_arr.entries = (handler_entry*)realloc(h_arr.entries, sizeof(handler_entry) * h_arr.size);
    }

    h_arr.entries[h_arr.next_avail] = entry;
    h_arr.next_avail++;
}

void register_label_handler(handler_f handler)
{
    handler_entry entry = {HANDLER_LABEL, ASM_INSTR_NONE, ASM_DIR_NONE, handler};
    register_handler_entry(entry);
}

void register_instruction_handler(handler_f handler, asm_instruction instruction)
{
    handler_entry entry = {HANDLER_INSTRUCTION, instruction, ASM_DIR_NONE, handler};
    register_handler_entry(entry);
}

void register_directive_handler(handler_f handler, asm_directive directive)
{
    handler_entry entry = {HANDLER_DIRECTIVE, ASM_INSTR_NONE, directive, handler};
    register_handler_entry(entry);
}

void handle_line(asm_line* line)
{
    if (line == NULL) {
        return;
    }

    if (p.sections == NULL) {
        init_program();
    }

    if (line->is_directive && line->directive == ASM_DIR_SECTION) {
        current_section = find_section(line->section_name);

        if (current_section == NULL) {
            current_section = new_section(line->section_name);
            add_section_to_program(current_section);
        }
    }

    for (int i = 0; i < h_arr.next_avail; i++) {
        handler_entry* entry = &h_arr.entries[i];

        if ((entry->kind == HANDLER_LABEL && line->is_label) ||
            (entry->kind == HANDLER_INSTRUCTION && line->is_instruction &&
             entry->instruction == line->instruction) ||
            (entry->kind == HANDLER_DIRECTIVE && line->is_directive &&
             entry->directive == line->directive)) {
            entry->handler(line, current_section);
            return;
        }
    }
}
