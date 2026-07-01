#include "error.hpp"

s_error new_error(char type, string info)
{
    s_error err;
    err.type = type;
    err.additional_info = info;
    err.no_error = false;
    return err;
}

s_error new_no_error()
{
    s_error err;
    err.no_error = true;
    return err;
}

string error_type_message(char type)
{
    switch (type)
    {
    case ERR_DOUBLE_SYM_DECL:
        return "Double symbol declaration";
    
    case ERR_LINE_OUTSIDE_SECTION:
        return "Line not in section";

    case ERR_LITERAL_TOO_LARGE_LS:
        return "Literal can't fit in 12 bits";

    case ERR_STORE_IMM_VALUE:
        return "Store can't be used with IMM values";

    case ERR_SYMBOL_TOO_LARGE_LS:
        return "Symbol can't fit in 12 bits, or is not known at assembly time";

    case ERR_TRAMP_OFFSET_12b:
        return "Trampoline offset is larger than 12 bits";

    case ERR_UNDEFINED_SYM:
        return "Undefined symbol";

    case ERR_EQU_NOT_CALCULATABLE:
        return "EQU expression is not calculatable";

    case ERR_SECTION_SYMBOL_CONFLICT:
        return "Section symbol conflict";

    case ERR_EXTERN_LOCAL_CONFLICT:
        return "Extern local conflict";

    case ERR_SYMBOL_CONFLICT:
        return "Symbol conflict";

    case ERR_LINK_FAIL_SYM_CONFLICT:
        return "Link failed due to symbol conflict";

    case ERR_OUTPUT_FILE_OPEN_FAIL:
        return "Failed to open output file";

    default:
        return "Unknown error";
    }
}

string error_to_string(s_error error)
{
    string err_msg = error_type_message(error.type);

    if (error.additional_info != "")
        err_msg += ": ";
    return err_msg + error.additional_info;
}