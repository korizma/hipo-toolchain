#ifndef RELA_TABLE_H
#define RELA_TABLE_H

#define RELA_TABLE_START_SIZE 10
#define RELA_TABLE_INCREMENT 10

#include "sym_table.h"

typedef struct s_section s_section;

typedef enum 
{
    R_HIPO_NONE = 0,
    R_HIPO_32 = 1,
    R_HIPO_12 = 2,
} e_Elf64_reloc_type;

typedef struct 
{
    unsigned long r_offset;    // offset in the section to replace
    unsigned long sym_index;    // symbol index in the symbol table
    e_Elf64_reloc_type reloc_type;  // relocation entry type
    unsigned long r_addend;  // addend field
} s_Elf64_Rela_entry;

typedef struct s_rela_table
{
    s_section* section;
    s_Elf64_Rela_entry** entries;
    long entry_num, size;
} s_rela_table;

void create_rela_table(s_section* s);

void create_rela_entry(s_section* s, unsigned long offset, int sym_index, e_Elf64_reloc_type type, long addend);

s_section* create_section_from_rela_table(s_rela_table* rela_table);

void print_rela_table(s_rela_table* rela_table);

void print_all_rela_tables();

// every symbols relocation was used as if the symbol is global
// this function checks for symbols that are LOCAL and adjusts the rela table
void check_rela_table(s_rela_table* rela_table);

void update_const_equ_rela_entries(s_Elf64_Sym* symbol);

void update_all_rela_entries(s_Elf64_Sym* symbol);


void export_rela_table_to_byte_array(s_rela_table* rela_table, s_final_output* output);

char* rela_entry_to_bytes(s_Elf64_Rela_entry* entry);

#endif
