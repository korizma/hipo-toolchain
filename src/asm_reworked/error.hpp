#pragma once

#include <string>
#include <vector>

using namespace std;

enum
{
    ERR_DOUBLE_SYM_DECL = 0,
    ERR_DOUBLE_SECT_DECL,
    ERR_LINE_OUTSIDE_SECTION,
    ERR_UNDEFINED_SYM,
    ERR_TRAMP_OFFSET_12b
};

typedef struct s_error
{
    char type;
    string additional_info;
} s_error;

// allocates a new error with the type and info
s_error* new_error(char type, string info = "");

// returns a string representation of the error
string error_to_string(s_error* error);

// returns the error message for the given type
string error_type_message(char type);