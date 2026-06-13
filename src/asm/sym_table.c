#include "elf.h"
#include <stdlib.h>
#include <string.h>

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

void update_label_size_if_last(s_section* s, int size)
{
    for (int i = p.sym_table->symbol_num-1; i >= 0; i--)
    {
        s_Elf64_Sym* sym = p.sym_table->symbols[i];
        if (sym->st_value != s->next_free || sym->section != s)
            break;
        
        sym->st_size = size;
    }
}

void update_section_size_in_sym_table(s_section* s)
{
    for (int i = p.sym_table->symbol_num-1; i >= 0; i--)
    {
        s_Elf64_Sym* sym = p.sym_table->symbols[i];
        if (sym->type == STT_SECTION && strcmp(sym->st_name, s->name) == 0)
        {
            sym->st_size = s->next_free;
            break;
        }
    }
}


int get_and_set_reference(char* symbol)
{
    int indx = check_symbol_table(symbol);
    if (indx == -1)
    {
        // does not exist
        add_to_symbol_table(symbol, 0, 0, 0, 0, 0, 0, ST_ENTRY_STATE_PARTIAL_REFERENCE);
        indx = p.sym_table->symbol_num - 1;
    }
    
    s_Elf64_Sym* sym = p.sym_table->symbols[indx];
    
    return indx;
}

int finalize_symbol_table()
{
    for (int i = 0; i < p.sym_table->symbol_num; i++)
    {
        s_Elf64_Sym* sym = p.sym_table->symbols[i];

        if (sym->state != ST_ENTRY_STATE_COMPLETE)
        {
            printf("Error: symbol '%s' is referenced but not defined\n", sym->st_name);
            return -1;
        }
    }
    return 0;
}