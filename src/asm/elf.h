#ifndef ELF_H
#define ELF_H

#include <stdbool.h>
#include "elf_typedefs.h"
#include "section.h"
#include "sym_table.h"
#include "rela_table.h"

#define PROGRAM_START_SECTION_SIZE 10
#define PROGRAM_SECTION_INCREMENT 10

typedef struct
{
    s_section** sections, *curr_section;
    int number_of_sections, section_arr_size;
    s_symbol_table* sym_table;

    bool has_ended;
} s_program;

extern s_program p;

int find_section_index(char* name);

void init_program();

void add_section_to_program(s_section* s);

void export_program_to_elf(const char* filename);

void print_program();

typedef struct 
{
    char* bytes;
    int num_written, size;
} s_elf_final;

void write_to_elf_final(s_elf_final final, char* bytes, int size);

#define EI_NIDENT 16
typedef struct {
    unsigned char e_ident[EI_NIDENT];
    t_Elf64_Half e_type;
    t_Elf64_Half e_machine;
    t_Elf64_Word e_version;
    t_Elf64_Addr e_entry;
    t_Elf64_Off e_phoff;
    t_Elf64_Off e_shoff;
    t_Elf64_Word e_flags;
    t_Elf64_Half e_ehsize;
    t_Elf64_Half e_phentsize;
    t_Elf64_Half e_phnum;
    t_Elf64_Half e_shentsize;
    t_Elf64_Half e_shnum;
    t_Elf64_Half e_shstrndx;
} s_Elf64_Ehdr;

void create_elf_header(s_Elf64_Ehdr* header);

typedef struct
{
    s_Elf64_Shdr* section_entries;
    int section_num;
} s_Elf64_section_info;

#endif
