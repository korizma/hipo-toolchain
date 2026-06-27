#include "symbol_table.hpp"
#include "asm.hpp"

s_symbol_table_entry* get_symbol_entry_index(int index)
{
    s_symbol_table* table = get_symbol_table(); 
    if (table->entries.size() <= index)
        return NULL;

    return &table->entries[index];
}

s_symbol_table_entry* get_symbol_entry_symbol(string symbol)
{
    s_symbol_table* table = get_symbol_table(); 

    for (s_symbol_table_entry entry : table->entries)
    {
        if (entry.name == symbol)
            return &entry;
    }
    return NULL;
}

s_symbol_table_entry* create_new_symbol_entry(string symbol)
{
    s_symbol_table* table = get_symbol_table(); 

    for (s_symbol_table_entry entry : table->entries)
    {
        if (entry.name == symbol)
            return NULL;
    }

    s_symbol_table_entry new_entry;
    new_entry.name = symbol;

    table->entries.push_back(new_entry);
    
    return &table->entries.back();
}

s_symbol_table_entry* get_and_create_new_symbol_entry(string symbol)
{
    s_symbol_table_entry* entry = create_new_symbol_entry(symbol);

    if (entry != NULL) return entry;
    
    s_symbol_table* table = get_symbol_table(); 

    for (s_symbol_table_entry entry : table->entries)
    {
        if (entry.name == symbol)
            return &entry;
    }
}

long get_symbol_entry_index_by_symbol(string symbol)
{    
    s_symbol_table* table = get_symbol_table(); 

    for (int i = 0; i < table->entries.size(); i++)
    {
        s_symbol_table_entry entry = table->entries[i];
        if (entry.name == symbol)
            return i;
    }
    return -1;
}

vector<char> symbol_table_entry_to_bytes(s_symbol_table_entry entry);

string symbol_table_to_string();

vector<s_error> finalize_symbol_table();