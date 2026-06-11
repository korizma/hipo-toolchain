#ifndef s_section_H
#define s_section_H

#include <stdbool.h>

#define SECTION_START_SIZE 16
#define SECTION_INCREMENT_SIZE 16
#define PROGRAM_START_SECTION_SIZE 10
#define PROGRAM_SECTION_INCREMENT 10
#define SYM_TABLE_START_SIZE 10
#define SYM_TABLE_INCREMENT 10
#define RELA_TABLE_START_SIZE 10
#define RELA_TABLE_INCREMENT 10


typedef enum 
{
    STB_LOCAL = 0,
    STB_GLOBAL = 1,
    STB_WEAK = 2,
    STB_LOOS = 10,
    STB_HIOS = 12,
    STB_LOPROC = 13,
    STB_HIPROC = 15
} e_Elf64_SymbolBinding;

typedef enum 
{
    STT_NOTYPE = 0,
    STT_OBJECT = 1,
    STT_FUNC = 2,
    STT_SECTION = 3,
    STT_FILE = 4,
    STT_COMMON = 5,
    STT_TLS = 6,
    STT_LOOS = 10,
    STT_HIOS = 12,
    STT_LOPROC = 13,
    STT_HIPROC = 15
} e_Elf64_SymbolType;

typedef enum 
{
    STV_DEFAULT = 0,
    STV_INTERNAL = 1,
    STV_HIDDEN = 2,
    STV_PROTECTED = 3
} e_Elf64_SymbolVisibility;

typedef enum 
{
    R_HIPO_NONE = 0,
    R_HIPO_32 = 1,
    R_HIPO_12 = 2,
    R_HIPO_PC12 = 3
} e_Elf64_reloc_type;

typedef struct s_section s_section;

typedef struct {
    unsigned long r_offset;    // offset in the section to replace
    int sym_index;    // symbol index in the symbol table
    e_Elf64_reloc_type reloc_type;  // relocation entry type
    long r_addend;  // addend field
} s_Elf64_Rela_entry;

typedef struct
{
    s_section* section;
    s_Elf64_Rela_entry** entries;
    int entry_num, size;
} s_rela_table;

typedef struct {
    char*    st_name;        // symbol name
    e_Elf64_SymbolType type;
    e_Elf64_SymbolBinding binding;
    e_Elf64_SymbolVisibility visibility;
    s_section* section;      // s_section index
    long    st_value;        // symbol value/address/offset
    long   st_size;          // symbol size in bytes
} s_Elf64_Sym;

typedef struct 
{
    s_Elf64_Sym** symbols;
    int symbol_num, size;
} s_symbol_table;


struct s_section
{
    char* name;
    int next_free, size;
    char* bytes;
    s_rela_table* rela_table;
};



typedef struct
{
    s_section** sections, *curr_section;
    int number_of_sections, section_arr_size;
    s_symbol_table* sym_table;
} s_program;

s_section* new_section(char* name);

void write_bytes_to_section(s_section* s, char* bytes, int num);

void skip_bytes_in_section(s_section* s, int num);

int find_section_index(char* name);

int find_label_in_section_if_last(s_section* s);

void init_program();

void add_section_to_program(s_section* s);

void export_program_to_elf();

void add_to_symbol_table(   char* symbol, 
                            e_Elf64_SymbolType type, 
                            e_Elf64_SymbolBinding binding, 
                            e_Elf64_SymbolVisibility visibility, 
                            s_section* section, 
                            long sym_offset,
                            long sym_size );

int check_symbol_table(char* symbol);

// only returns true if symbol has type NOTYPE
bool check_if_symbol_can_be_jumped_to(char* symbol);

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
