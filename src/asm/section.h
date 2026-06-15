#ifndef SECTION_H
#define SECTION_H

#include "elf_typedefs.h"

#define SECTION_START_SIZE 16
#define SECTION_INCREMENT_SIZE 16

#define SHT_NULL 0 /* Section header table entry unused */
#define SHT_PROGBITS 1 /* Program data */
#define SHT_SYMTAB 2 /* Symbol table */
#define SHT_STRTAB 3 /* String table */
#define SHT_RELA 4 /* Relocation entries with addends */
#define SHT_DYNAMIC 6 /* Dynamic linking information */
#define SHT_NOTE 7 /* Notes */
#define SHT_NOBITS 8 /* Program space with no data (bss) */
#define SHT_REL 9 /* Relocation entries, no addends */

#define SHF_WRITE (1 << 0) /* Writable */
#define SHF_ALLOC (1 << 1) /* Occupies memory during execution */
#define SHF_EXECINSTR (1 << 2) /* Executable */
#define SHF_MERGE (1 << 4) /* Might be merged */
#define SHF_STRINGS (1 << 5) /* Contains nul-terminated strings */
#define SHF_INFO_LINK (1 << 6) /* 'sh_info' contains SHT index */
#define SHF_GROUP (1 << 9) /* Section is member of a group. */
#define SHF_TLS (1 << 10) /* Section hold thread-local data. */
#define SHF_MASKOS 0x0ff00000 /* OS-specific. */
#define SHF_MASKPROC 0xf0000000 /* Processor-specific */

typedef struct s_rela_table s_rela_table;

typedef struct {
    t_Elf64_Word sh_name;       // index in the section name string table
    t_Elf64_Word sh_type;       // defines the sections content and meaning
    t_Elf64_Xword sh_flags;     // flags
    t_Elf64_Addr sh_addr;       // virtual address of the first byte
    t_Elf64_Off sh_offset;      // location of the first byte of the section in the ELF file
    t_Elf64_Xword sh_size;      // size of the section in the ELF file
    t_Elf64_Word sh_link;       // link to a different section, this is the index of that section int the section header table
    t_Elf64_Word sh_info;       // additional info, up to interpretation
    t_Elf64_Xword sh_addralign; // alignment of the section
    t_Elf64_Xword sh_entsize;   // if the section is a table, this field represents the size of that entry
} s_Elf64_Shdr;

typedef struct s_section
{
    // ELF format section header entry in the Section Table
    s_Elf64_Shdr st_entry;

    char* name;
    int next_free, size;
    char* bytes;
    s_rela_table* rela_table;
} s_section;

s_section* new_section(char* name);

void write_bytes_to_section(s_section* s, char* bytes, int num);

void skip_bytes_in_section(s_section* s, int num);

void print_section(s_section* section);

#endif
