#pragma once

#include <string>
#include <vector>

using namespace std;

enum
{
    ERR_DOUBLE_SYM_DECL = 0,
    ERR_LINE_OUTSIDE_SECTION,
    ERR_UNDEFINED_SYM,
    ERR_TRAMP_OFFSET_12b,
    ERR_STORE_IMM_VALUE,
    ERR_LITERAL_TOO_LARGE_LS,
    ERR_SYMBOL_TOO_LARGE_LS,
    ERR_EQU_NOT_CALCULATABLE,
    ERR_SECTION_SYMBOL_CONFLICT,
    ERR_EXTERN_LOCAL_CONFLICT,
    ERR_SYMBOL_CONFLICT,
    ERR_LINK_FAIL_SYM_CONFLICT,
    ERR_OUTPUT_FILE_OPEN_FAIL,
    ERR_LINK_FAIL_SYM_UNRESOLVED,
    ERR_INVALID_SECTION_TO_PLACE,
    ERR_SECTION_PLACEMENT_CONFLICT
};

typedef struct s_error
{
    char type;
    string additional_info;

    bool no_error;
} s_error;

// allocates a new error with the type and info
s_error new_error(char type, string info = "");

// allocates a new error with the type and info
s_error new_no_error();

// returns a string representation of the error
string error_to_string(s_error error);

// returns the error message for the given type
string error_type_message(char type);