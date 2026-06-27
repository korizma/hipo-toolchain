#pragma once

#include <string>
#include <vector>

using namespace std;

typedef struct s_expr
{
    long integer_value;
    vector<long> coeff;
    vector<string> symbol;
    
    // equ can be declared in or outside a section
    bool in_section;

    long symbol_table_index;
} s_expr;

// heap allocates a new expression
s_expr* new_expression();
// returns if the expression is valid, its valid if it only depends on one section 
bool expression_is_valid(s_expr* expression);
// simplifies the expression so that it has 0 or 1 symbols
void simplify_expression(s_expr* expression);
// adds a literal to the expression
void add_literal_to_expression(s_expr* expression, long literal);
// adds a symbol to the expression
void add_symbol_to_expression(s_expr* expression, string symbol, int sign);