#include <iostream>
#include "args_parser.hpp"
#include "linker.hpp"
#include "misc.hpp"

#include <vector>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    vector<string> args;

    for (int i = 1; i < argc; ++i)
    {
        args.push_back(argv[i]);
    }

    bool isHex = find_option(args, "-hex");
    bool isRel = find_option(args, "-relocatable");

    vector<string> place_args = find_all_option_values_equals(args, "-place");
    vector<string> obj_files = find_all_non_options(args);
    string output_filename = find_first_option_value_seperate(args, "-o");

    if (output_filename == "")
    {
        output_filename = "a.out";
    }

    if (obj_files.size() == 0)
    {
        cout << "No object files provided." << endl;
        return 1;
    }

    if (isHex && isRel)
    {
        cout << "Cannot specify both -hex and -relocatable options." << endl;
        return 1;
    }

    if (!isHex && !isRel)
    {
        cout << "Must specify either -hex or -relocatable option." << endl;
        return 1;
    }

    char linker_type = isHex ? LINKER_HEX : LINKER_REL;

    vector<pair<string, long>> section_place_locations;

    for (string place_arg : place_args)
    {
        vector<string> parts = split_string(place_arg, '=');
        if (parts.size() != 2)
        {
            cout << "Invalid -place argument: " << place_arg << endl;
            return 1;
        }

        string section_name = parts[0];
        long place_location = stol(parts[1], nullptr, 16);

        section_place_locations.push_back(make_pair(section_name, place_location));
    }

    bool success = linker_execute(linker_type, obj_files, output_filename, section_place_locations);

    if (!success)
    {
        return 1;
    }

    return 0;
}
