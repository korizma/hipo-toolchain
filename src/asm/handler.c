#include "handler.h"
#include <stdlib.h>
#include <string.h>

handlers_arr h_arr;

static section* current_section = NULL;

static char* copy_string(char* string)
{
    if (string == NULL) {
        return NULL;
    }

    size_t len = strlen(string) + 1;
    char* copy = (char*)malloc(len);

    if (copy != NULL) {
        memcpy(copy, string, len);
    }

    return copy;
}

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
    h_arr.names = (char**)malloc(sizeof(char*)*h_arr.size);
    h_arr.handlers = (handler_f*)malloc(sizeof(handler_f) * h_arr.size);
    h_arr.next_avail = 0;
}

void register_handler(handler_f handler, char* name)
{
    if (handler == NULL || name == NULL) {
        return;
    }

    if (h_arr.handlers == NULL || h_arr.names == NULL || h_arr.size == 0) {
        init_handler_arr();
    }

    if (h_arr.next_avail == h_arr.size) {
        h_arr.size += HANDLERS_INCREASE_SIZE;
        h_arr.names = (char**)realloc(h_arr.names, sizeof(char*) * h_arr.size);
        h_arr.handlers = (handler_f*)realloc(h_arr.handlers, sizeof(handler_f) * h_arr.size);
    }

    h_arr.names[h_arr.next_avail] = copy_string(name);
    h_arr.handlers[h_arr.next_avail] = handler;
    h_arr.next_avail++;
}

void handle_line(asm_line* line)
{
    if (line == NULL) {
        return;
    }

    if (p.sections == NULL) {
        init_program();
    }

    if (line->is_directive && line->operation != NULL &&
        strcmp(line->operation, "section") == 0) {
        current_section = find_section(line->section_name);

        if (current_section == NULL) {
            current_section = new_section(line->section_name);
            add_section_to_program(current_section);
        }
    }

    char* name = line->is_label ? "label" : line->operation;

    if (name == NULL) {
        return;
    }

    for (int i = 0; i < h_arr.next_avail; i++) {
        if (h_arr.names[i] != NULL && strcmp(h_arr.names[i], name) == 0) {
            h_arr.handlers[i](line, current_section);
            return;
        }
    }
}
