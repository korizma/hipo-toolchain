#include "section.h"
#include <stdlib.h>
#include <string.h>

s_section* new_section(char* name)
{
    s_section* s = (s_section*)malloc(sizeof(s_section));
    s->size = SECTION_START_SIZE;
    s->bytes = (char*)malloc(sizeof(char)*SECTION_START_SIZE);
    s->name = (char*)malloc(strlen(name) + 1);
    strcpy(s->name, name);
    s->next_free = 0;
    return s;
}

void write_bytes_to_section(s_section* s, char* bytes, int num)
{
    if (s->next_free + num > s->size)
    {
        s->size += SECTION_INCREMENT_SIZE;
        s->bytes = realloc(s->bytes, s->size * sizeof(char));
    }
    memcpy(s->bytes + sizeof(char)*s->next_free, bytes, num);
    s->next_free += num;
}

void skip_bytes_in_section(s_section* s, int num)
{
    if (s->next_free + num > s->size)
    {
        s->size += SECTION_INCREMENT_SIZE;
        s->bytes = realloc(s->bytes, s->size * sizeof(char));
    }
    for (int i = 0; i < num; i++)
    {
        s->bytes[i + s->next_free] = 0;
    }
    s->next_free += num;
}
