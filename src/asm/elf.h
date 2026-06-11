#ifndef s_section_H
#define s_section_H

#include <stdbool.h>

#define SECTION_START_SIZE 16
#define SECTION_INCREMENT_SIZE 16
#define PROGRAM_START_SECTION_SIZE 10
#define PROGRAM_SECTION_INCREMENT 10
#define SYM_TABLE_START_SIZE 10
#define SYM_TABLE_INCREMENT 10


typedef struct
{
    char* name;
    int next_free, size;
    char* bytes;
} s_section;


typedef enum {
    STB_LOCAL = 0,
    STB_GLOBAL = 1,
    STB_WEAK = 2,
    STB_LOOS = 10,
    STB_HIOS = 12,
    STB_LOPROC = 13,
    STB_HIPROC = 15
} s_Elf64_SymbolBinding;

typedef enum {
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
} s_Elf64_SymbolType;

typedef enum {
    STV_DEFAULT = 0,
    STV_INTERNAL = 1,
    STV_HIDDEN = 2,
    STV_PROTECTED = 3
} s_Elf64_SymbolVisibility;

typedef struct {
    char*    st_name;        // symbol name
    unsigned char st_info;   // symbol type and binding
    unsigned char st_other;  // visibility
    s_section* section;            // s_section index
    long    st_value;        // symbol value/address/offset
    long   st_size;          // symbol size in bytes
} s_Elf64_Sym;

typedef struct 
{
    s_Elf64_Sym** symbols;
    int symbol_num, size;
} s_symbol_table;

typedef struct
{
    s_section** sections;
    int number_of_sections, section_arr_size;
    s_symbol_table* sym_table;
} s_program;

s_section* new_s_section(char* name);

void write_bytes_to_section(s_section* s, char* bytes, int num);

void init_program();

void add_section_to_program(s_section* s);

void export_program_to_elf();

void add_to_symbol_table(   char* symbol, 
                            s_Elf64_SymbolType type, 
                            s_Elf64_SymbolBinding binding, 
                            s_Elf64_SymbolVisibility visibility, 
                            s_section* section, 
                            long sym_offset,
                            long sym_size );

bool check_symbol_table(char* symbol);

#endif
