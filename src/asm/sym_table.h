#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#define SYM_TABLE_START_SIZE 10
#define SYM_TABLE_INCREMENT 10

typedef struct s_section s_section;

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
    STT_SECTION = 3,
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
    ST_ENTRY_STATE_EMPTY,
    ST_ENTRY_STATE_PARTIAL_GLOBAL,
    ST_ENTRY_STATE_PARTIAL_REFERENCE,
    ST_ENTRY_STATE_COMPLETE,
    ST_ENTRY_STATE_EQU
} e_Elf64_symbol_entry_state;

typedef struct {
    char*    st_name;        // symbol name
    e_Elf64_SymbolType type;
    e_Elf64_SymbolBinding binding;
    e_Elf64_SymbolVisibility visibility;
    s_section* section;      // s_section index
    long    st_value;        // symbol value/address/offset
    long   st_size;          // symbol size in bytes

    // for assembly
    e_Elf64_symbol_entry_state state;
} s_Elf64_Sym;

typedef struct 
{
    s_Elf64_Sym** symbols;
    int symbol_num, size;
} s_symbol_table;

void add_to_symbol_table(   char* symbol, 
                            e_Elf64_SymbolType type, 
                            e_Elf64_SymbolBinding binding, 
                            e_Elf64_SymbolVisibility visibility, 
                            s_section* section, 
                            long sym_offset,
                            long sym_size,
                            e_Elf64_symbol_entry_state state
                        );

int check_symbol_table(char* symbol);

// this is called when an instruction needs info for relocation
// if symbol does not exist it will create a temporary entry in the symbol table that needs to be updated later
// returns index if success, -1 if the symbol is already in the sym table and has type STT_SECTION
int get_and_set_reference(char* symbol);

void update_label_size_if_last(s_section* s, int size);

void update_section_size_in_sym_table(s_section* s);

// returns an error if there are symbols that are referenced but not inited
struct s_error* finalize_symbol_table();

#endif
