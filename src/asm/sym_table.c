#include "elf.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>
#include "code.h"
#include "handlers_impl.h"

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

s_error* finalize_symbol_table()
{
    for (int i = 0; i < p.sym_table->symbol_num; i++)
    {
        s_Elf64_Sym* sym = p.sym_table->symbols[i];

        if (sym->state == ST_ENTRY_STATE_EQU)
        {
            simplify_equ_expression(sym->equ_expr);

            if (sym->equ_expr->symbol_num == 0)
            {
                update_const_equ_rela_entries(sym);
            }
            else if (sym->equ_expr->symbol_num > 1)
            {
                return new_symbol_error(0, ERR_EXPR_INVALID, sym->st_name);
            }
            else if (sym->equ_expr->symbol_num == 1)
            {
                int indx = check_symbol_table(sym->equ_expr->symbol_list[0]);
                if (indx == -1)
                {
                    return new_symbol_error(0, ERR_UNRESOLVED_SYMBOL, sym->equ_expr->symbol_list[0]);
                }
                s_Elf64_Sym* b = p.sym_table->symbols[indx];

                if (sym->binding == STB_GLOBAL)
                {
                    // b is extern
                    if (b->binding == STB_GLOBAL && b->section == 0)
                    {
                        return new_symbol_error(0, ERR_EQU_EXTERN, b->st_name);
                    }
                    
                    sym->section = b->section;
                    sym->st_value = b->st_value + sym->equ_expr->value;
                    continue;
                }

                if (b->binding == STB_GLOBAL)
                {
                    sym->section = b->section;
                    sym->st_value = b->st_value + sym->equ_expr->value;

                    update_all_rela_entries(sym);
                    continue;
                }

                if (b->binding == STB_LOCAL)
                {
                    sym->section = b->section;
                    sym->st_value = b->st_value + sym->equ_expr->value;

                    update_all_rela_entries(sym);
                    continue;
                }
            }

            // 1. slucaj, konstanta je => relokacije se brisu i vrednost se upisuje
            // 2. slucaj, globalna je, i B je globalna => nista se ne menja
            // 3. slucaj, globalna je, i B je extern   => greska
            // 4. slucaj, globalna je, b nije globalna => nista se ne menja
            // 5. slucaj, nije globalna, B je globalna => sve relokacije se menjaju da referenciraju B
            // 6. slucaj, nije globalna, B nije global => sve relokacije se menjaju prema sekciji
        }
        else if (sym->state != ST_ENTRY_STATE_COMPLETE)
        {
            return new_symbol_error(0, ERR_UNRESOLVED_SYMBOL, sym->st_name);
        }
    }
    return NULL;
}
