#ifndef RELA_TABLE_H
#define RELA_TABLE_H

#define RELA_TABLE_START_SIZE 10
#define RELA_TABLE_INCREMENT 10

typedef struct s_section s_section;

typedef enum 
{
    R_HIPO_NONE = 0,
    R_HIPO_32 = 1,
    R_HIPO_12 = 2,
    R_HIPO_PC12 = 3
} e_Elf64_reloc_type;

typedef struct 
{
    unsigned long r_offset;    // offset in the section to replace
    int sym_index;    // symbol index in the symbol table
    e_Elf64_reloc_type reloc_type;  // relocation entry type
    long r_addend;  // addend field
} s_Elf64_Rela_entry;

typedef struct s_rela_table
{
    s_section* section;
    s_Elf64_Rela_entry** entries;
    int entry_num, size;
} s_rela_table;

void create_rela_table(s_section* s);

// void create_rela_entry(unsigned long offset, int sym_index, e_Elf64_reloc_type type, long addend);

// notes:
// types of relocation 
// jump label relocation
//      symbol in section
//      symbol not in section
// any reference to a symbol that is not defined at asm time
//      sym global
//      sym local

#endif
