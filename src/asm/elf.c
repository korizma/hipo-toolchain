#include "elf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char* program_to_string(s_program *program);

s_program p;

void init_program()
{
    p.sections = (s_section**)malloc(sizeof(s_section*)*PROGRAM_START_SECTION_SIZE);
    p.section_arr_size = PROGRAM_START_SECTION_SIZE;
    p.number_of_sections = 0;
    p.has_ended = false;

    p.sym_table = (s_symbol_table*)malloc(sizeof(s_symbol_table));
    p.sym_table->symbol_num = 0;
    p.sym_table->size = SYM_TABLE_START_SIZE;
    p.sym_table->symbols = (s_Elf64_Sym**)malloc(sizeof(s_Elf64_Sym*) * p.sym_table->size);
}

void add_section_to_program(s_section* s)
{
    if (p.number_of_sections == p.section_arr_size)
    {
        p.section_arr_size += PROGRAM_SECTION_INCREMENT;
        p.sections = realloc(p.sections, sizeof(s_section*) * p.section_arr_size);
    }
    p.sections[p.number_of_sections++] = s;
}

void export_program_to_binary(const char* filename)
{
    s_final_output output;
    output.pos = 0;
    output.size = 10;
    output.bytes = (char*)malloc(sizeof(char) * output.size);
    output.str_table.string_num = 0;
    output.str_table.size = 10;
    output.str_table.strings = (char**)malloc(sizeof(char*) * output.str_table.size);

    export_sym_table_to_byte_array(&output);

    for (int i = 0; i < p.number_of_sections; i++)
    {
        export_section_to_byte_array(p.sections[i], &output);
    }

    // now just open the file and write the string table first and then write the bytes of output
    FILE* file = fopen(filename, "wb");

    if (file == NULL)
    {
        printf("Error opening file %s for writing\n", filename);
        return;
    }

    char* string_table_size_bytes = long_to_8_bytes(output.str_table.string_num);
    fwrite(string_table_size_bytes, 1, 8, file);

    for (int i = 0; i < output.str_table.string_num; i++)
    {
        char* string_bytes = output.str_table.strings[i];
        long string_size = strlen(string_bytes) + 1;

        fwrite(string_bytes, 1, string_size, file);
    }

    fwrite(output.bytes, 1, output.pos, file);
    fclose(file);
}


void add_string_to_string_table(s_final_output* output, char* string)
{
    if (output->str_table.string_num == output->str_table.size)
    {
        output->str_table.size += 10;
        output->str_table.strings = realloc(output->str_table.strings, sizeof(char*) * output->str_table.size);
    }
    output->str_table.strings[output->str_table.string_num] = (char*)malloc(strlen(string) + 1);
    strcpy(output->str_table.strings[output->str_table.string_num], string);
    output->str_table.string_num++;
}

long get_index_of_string_in_string_table(s_final_output* output, char* string)
{
    for (long i = 0; i < output->str_table.string_num; i++)
    {
        if (strcmp(output->str_table.strings[i], string) == 0)
        {
            return i;
        }
    }

    return -1;
}

long add_and_get_string_string_table(s_final_output* output, char* string)
{
    long index = get_index_of_string_in_string_table(output, string);
    if (index == -1)
    {
        add_string_to_string_table(output, string);
        index = output->str_table.string_num - 1;
    }

    return index;
}

void write_n_bytes_final_output(s_final_output* output, char* bytes, int n)
{
    if (output->pos + n >= output->size)
    {
        output->size += n + 10;
        output->bytes = realloc(output->bytes, sizeof(char*) * output->size);
    }

    for (int i = 0; i < n; i++)
    {
        output->bytes[output->pos + i] = bytes[i];
    }
    output->pos += n;
}

// needs to free after
char* long_to_8_bytes(long x)
{
    char* bytes = (char*)malloc(8);
    for (int i = 0; i < 8; i++)
    {
        bytes[i] = (x >> (i * 8)) & 0xFF;
    }
    return bytes;
}
char* int_to_4_bytes(int x)
{
    char* bytes = (char*)malloc(4);
    for (int i = 0; i < 4; i++)
    {
        bytes[i] = (x >> (i * 8)) & 0xFF;
    }
    return bytes;
}

void export_program_to_text(const char* filename)
{
    char* program_string = program_to_string(&p);

    FILE* file = fopen(filename, "wb");

    if (file == NULL)
    {
        printf("Error opening file %s for writing\n", filename);
        return;
    }

    fwrite(program_string, 1, strlen(program_string), file);
    fclose(file);
}