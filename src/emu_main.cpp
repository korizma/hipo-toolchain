#include <iostream>
#include "args_parser.hpp"
#include "emu.hpp"
#include "misc.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    vector<string> args;

    for (int i = 1; i < argc; ++i)
    {
        args.push_back(argv[i]);
    }

    vector<string> obj_files = find_all_non_options(args);
    if (obj_files.size() != 1)
    {
        cout << "Exactly one object file must be provided." << endl;
        return 1;
    }

    emulate_file(obj_files[0]);

    return 0;
}
