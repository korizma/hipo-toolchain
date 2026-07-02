#include "expr.hpp"
#include "symbol_table.hpp"
#include <stdlib.h>
#include <unordered_map>
#include <utility>
#include "asm.hpp"

using namespace std;


s_expr* new_expression()
{
    s_expr* expression = new s_expr();
    expression->integer_value = 0;
    expression->in_section = false;
    expression->symbol_table_index = -1;
    expression->undefined_symbol_exists = false;
    return expression;
}

void simplify_expression(s_program* program, s_expr* expression)
{
    unordered_map<long, pair<s_symbol_table_entry*, long>> section_to_symbol_coeff;

    for (int i = 0; i < expression->coeff.size(); i++)
    {
        s_symbol_table_entry* symbol_entry = get_symbol_entry_index(get_symbol_table(program), expression->symbol_index[i]);
        long coeff = expression->coeff[i];

        if (symbol_entry->state == STS_EQU)
        {
            expression->undefined_symbol_exists = true;
            expression->symbol[0] = symbol_entry->name;
            return;
        }

        if (section_to_symbol_coeff.find(symbol_entry->section_symbol_table_index) == section_to_symbol_coeff.end())
        {
            section_to_symbol_coeff[symbol_entry->section_symbol_table_index] = make_pair(symbol_entry, coeff); 
        }
        else
        {
            long offset_diff = section_to_symbol_coeff[symbol_entry->section_symbol_table_index].first->offset_or_value - symbol_entry->offset_or_value;

            section_to_symbol_coeff[symbol_entry->section_symbol_table_index].second += coeff;
            expression->integer_value -= offset_diff * coeff;
        }
    }

    expression->coeff.clear();
    expression->symbol.clear();
    expression->symbol_index.clear();

    for (const auto& [key, value] : section_to_symbol_coeff) 
    {
        if (value.second == 0)
            continue;

        expression->coeff.push_back(value.second);
        expression->symbol_index.push_back(get_symbol_entry_index_by_symbol(get_symbol_table(program), value.first->name));
        expression->symbol.push_back(value.first->name);
    }   
}

bool expression_is_valid(s_program *program, s_expr* expression)
{
    simplify_expression(program, expression);
    return expression->symbol.size() <= 1;
}

void add_literal_to_expression(s_expr* expression, long literal)
{
    expression->integer_value += literal;
}

void add_symbol_to_expression(s_expr* expression, string symbol, int sign)
{
    for  (int i = 0; i < expression->symbol.size(); i++)
    {
        if (expression->symbol[i] == symbol)
        {
            expression->coeff[i] += sign;
            return;
        }
    }

    expression->symbol.push_back(symbol);
    expression->coeff.push_back(sign);
}

void equ_set_symbol_indexes(s_program* program, s_expr* expression)
{
    for (string symbol : expression->symbol)
    {
        long indx = get_symbol_entry_index_by_symbol(get_symbol_table(program), symbol);
        if (indx == -1)
        {
            expression->undefined_symbol_exists = true;
            expression->symbol[0] = symbol;
            return;
        }

        expression->symbol_index.push_back(indx);
    }
}
