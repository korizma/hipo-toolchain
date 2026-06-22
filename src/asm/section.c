#include "section.h"
#include "elf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern s_program p;

s_section* new_section(char* name)
{
    s_section* s = (s_section*)malloc(sizeof(s_section));

    s->size = SECTION_START_SIZE;
    s->bytes = (char*)malloc(sizeof(char)*SECTION_START_SIZE);
    s->name = (char*)malloc(strlen(name) + 1);
    strcpy(s->name, name);
    s->next_free = 0;
    s->rela_table = 0;
    
    return s;
}

void write_bytes_to_section(s_section* s, char* bytes, int num)
{
    if (s->next_free + num >= s->size)
    {
        s->size += num + SECTION_INCREMENT_SIZE;
        s->bytes = realloc(s->bytes, s->size * sizeof(char));
    }
    memcpy(s->bytes + sizeof(char)*s->next_free, bytes, num);
    s->next_free += num;
}

void skip_bytes_in_section(s_section* s, int num)
{
    if (s->next_free + num >= s->size)
    {
        s->size += num + SECTION_INCREMENT_SIZE;
        s->bytes = realloc(s->bytes, s->size * sizeof(char));
    }
    for (int i = 0; i < num; i++)
    {
        s->bytes[i + s->next_free] = 0;
    }
    s->next_free += num;
}

void print_section(s_section* section)
{
    if (section == 0)
    {
        printf("Section: <none>\n");
        return;
    }

    printf("Section %s: size=%d, capacity=%d\n",
           section->name != 0 ? section->name : "<unnamed>",
           section->next_free,
           section->size);

    if (section->next_free == 0)
    {
        printf("  Bytes: <empty>\n");
    }
    else
    {
        printf("  Bytes:\n");
        for (int i = 0; i < section->next_free; i += 4)
        {
            printf("    0x%08x:", i);
            for (int j = 0; j < 4 && i + j < section->next_free; j++)
            {
                printf(" %02x", (unsigned char)section->bytes[i + j]);
            }
            printf("\n");
        }
    }

}

// the index of the sections name in the string table - 8 bytes
// size of the section - 8 bytes
// flag if the section has a rela table - 1 byte (0x00 means no rela table, 0x01 means it has a rela table)
void export_section_to_byte_array(s_section* section, s_final_output* output)
{
    long index_sec_name = add_and_get_string_string_table(output, section->name);
    
    char* index_sec_name_buffer = long_to_8_bytes(index_sec_name);
    char* section_size_buffer = long_to_8_bytes(section->next_free);
    char  rela_flag = section->rela_table != 0;

    write_n_bytes_final_output(output, index_sec_name_buffer, 8);
    write_n_bytes_final_output(output, section_size_buffer, 8);
    write_n_bytes_final_output(output, &rela_flag, 1);

    free(index_sec_name_buffer);
    free(section_size_buffer);

    write_n_bytes_final_output(output, section->bytes, section->next_free);

    if (section->rela_table != 0)
    {
        export_rela_table_to_byte_array(section->rela_table, output);
    }
}


long section_index_in_program(s_section* section)
{
    for (int i = 0; i < p.number_of_sections; i++)
    {
        if (p.sections[i] == section)
            return i;
    }
    return -1;
}

