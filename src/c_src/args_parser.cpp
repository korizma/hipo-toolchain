#include "args_parser.hpp"

using namespace std;

string find_first_option_value_equals(vector<string> args, string option)
{
    for (int i = 0; i < args.size(); i++)
    {
        if (args[i].substr(0, option.size()) == option)
            return args[i].substr(option.size() + 1);   // skip -option=
    }
    return "";
}

vector<string> find_all_option_values_equals(vector<string> args, string option)
{
    vector<string> values;
    for (int i = 0; i < args.size(); i++)
    {
        if (args[i].substr(0, option.size()) == option)
            values.push_back(args[i].substr(option.size() + 1));
    }
    return values;
}

string find_first_option_value_seperate(vector<string> args, string option)
{
    vector<string> values;
    for (int i = 0; i < args.size(); i++)
    {
        if (args[i] == option && i + 1 < args.size())
            return args[i + 1];
    }
    return "";
}

vector<string> find_all_option_values_seperate(vector<string> args, string option)
{
    vector<string> values;
    for (int i = 0; i < args.size(); i++)
    {
        if (args[i] == option && i + 1 < args.size())
            values.push_back(args[i + 1]);
    }
    return values;
}

vector<string> find_all_non_options(vector<string> args)
{
    vector<string> non_options;
    for (const auto& arg : args)
    {
        if (arg.substr(0, 2) != "--" && arg.substr(0, 1) != "-")
            non_options.push_back(arg);
    }
    return non_options;
}

bool find_option(vector<string> args, string option)
{
    for (const auto& arg : args)
    {
        if (arg == option)
            return true;
    }
    return false;
}