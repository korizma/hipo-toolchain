#include "expr.hpp"
#include "symbol_table.hpp"
#include <stdlib.h>

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

void simplify_expression(s_expr* expression)
{
    vector<string> symbols;
    vector<long> coeffs;

    for (int i = 0; i < expression->symbol.size(); i++)
    {
        if (expression->coeff[i] == 0)
            continue;

        symbols.push_back(expression->symbol[i]);
        coeffs.push_back(expression->coeff[i]);
    }

    expression->symbol = symbols;
    expression->coeff = coeffs;
}

bool expression_is_valid(s_expr* expression)
{
    simplify_expression(expression);
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

void equ_set_symbol_indexes(s_expr* expression)
{
    for (string symbol : expression->symbol)
    {
        long indx = get_symbol_entry_index_by_symbol(symbol);
        if (indx == -1)
        {
            expression->undefined_symbol_exists = true;
            return;
        }

        expression->symbol_index.push_back(indx);
    }
}
