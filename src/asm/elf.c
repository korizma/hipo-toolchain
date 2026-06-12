#include "elf.h"
#include "stdlib.h"
#include "string.h"

s_program p;

s_section* new_section(char* name)
{
    s_section* s = (s_section*)malloc(sizeof(s_section));
    s->size = SECTION_START_SIZE;
    s->bytes = (char*)malloc(sizeof(char)*SECTION_START_SIZE);
    s->name = (char*)malloc(strlen(name) + 1);
    strcpy(s->name, name);
    s->next_free = 0;
    return s;
}

void write_bytes_to_section(s_section* s, char* bytes, int num)
{
    if (s->next_free + num > s->size)
    {
        s->size += SECTION_INCREMENT_SIZE;
        s->bytes = realloc(s->bytes, s->size * sizeof(char));
    }
    memcpy(s->bytes + sizeof(char)*s->next_free, bytes, num);
    s->next_free += num;
}

void init_program()
{
    p.sections = (s_section**)malloc(sizeof(s_section*)*PROGRAM_START_SECTION_SIZE);
    p.section_arr_size = PROGRAM_START_SECTION_SIZE;
    p.number_of_sections = 0;

    p.sym_table = (s_symbol_table*)malloc(sizeof(s_symbol_table));
    p.sym_table->symbol_num = 0;
    p.sym_table->size = SYM_TABLE_START_SIZE;
    p.sym_table->symbols = (s_Elf64_Sym**)malloc(sizeof(s_Elf64_Sym*) * p.sym_table->size);
}

void add_section_to_program(s_section* s)
{
    if (p.number_of_sections == p.section_arr_size)
    {
        p.section_arr_size += PROGRAM_SECTION_INCREMENT;
        p.sections = realloc(p.sections, sizeof(s_section*) * p.section_arr_size);
    }
    p.sections[p.number_of_sections++] = s;
}

int find_section_index(char* name)
{
    for (int i = 0; i < p.number_of_sections; i++)
    {
        if (strcmp(p.sections[i]->name, name) == 0)
            return i;
    }
    return -1;
}


void export_program_to_elf()
{
    
}


void skip_bytes_in_section(s_section* s, int num)
{
    if (s->next_free + num > s->size)
    {
        s->size += SECTION_INCREMENT_SIZE;
        s->bytes = realloc(s->bytes, s->size * sizeof(char));
    }
    for (int i = 0; i < num; i++)
    {
        s->bytes[i + s->next_free] = 0;
    }
    s->next_free += num;
}


void add_to_symbol_table(   char* symbol, 
                            e_Elf64_SymbolType type, 
                            e_Elf64_SymbolBinding binding, 
                            e_Elf64_SymbolVisibility visibility, 
                            s_section* section, 
                            long sym_offset,
                            long sym_size,
                            e_Elf64_symbol_entry_state state
                            )
{
    if (p.sym_table->symbol_num == p.sym_table->size)
    {
        p.sym_table->size += SYM_TABLE_INCREMENT;
        p.sym_table->symbols = realloc(p.sym_table->symbols, sizeof(s_Elf64_Sym*) * p.sym_table->size);
    }
    s_Elf64_Sym* new_entry = (s_Elf64_Sym*)malloc(sizeof(s_Elf64_Sym));
    new_entry->section = section;
    new_entry->binding = binding;
    new_entry->type = type;
    new_entry->visibility = visibility;
    new_entry->st_size = sym_size;
    new_entry->st_value = sym_offset;
    new_entry->st_name = symbol;
    new_entry->state = state;
    p.sym_table->symbols[p.sym_table->symbol_num++] = new_entry;
}

int check_symbol_table(char* symbol)
{
    for (int i = 0; i < p.sym_table->symbol_num; i++)
    {
        if (strcmp(symbol, p.sym_table->symbols[i]->st_name) == 0)
            return i;
    }
    return -1;
}

bool check_if_symbol_can_be_jumped_to(char* symbol)
{
    int indx = check_symbol_table(symbol);
    if (indx == -1)
        return false;
    return p.sym_table->symbols[indx]->type == STT_NOTYPE;
}

void create_rela_table(s_section* s)
{
    s->rela_table = (s_rela_table*)malloc(sizeof(s_rela_table));
    s->rela_table->size = RELA_TABLE_START_SIZE;
    s->rela_table->entry_num = 0;
    s->rela_table->entries = (s_Elf64_Rela_entry**)malloc(sizeof(s_Elf64_Rela_entry*) * s->rela_table->size);
    s->rela_table->section = s;
}


void update_label_size_if_last(s_section* s, int size)
{

}

void update_section_size_in_sym_table(s_section* s)
{
    
}
