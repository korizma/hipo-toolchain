#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "asm.hpp"
#include "grammar.tab.h"

using namespace std;

vector<s_asm_line> lines;
extern FILE *yyin;

int yylex_destroy(void);

static string make_default_output_file(string input_file)
{
    size_t slash_pos = input_file.find_last_of("/\\");
    size_t dot_pos = input_file.find_last_of('.');

    if (dot_pos != string::npos && (slash_pos == string::npos || dot_pos > slash_pos))
        return input_file.substr(0, dot_pos) + ".bin";

    return input_file + ".bin";
}

int main(int argc, char **argv)
{
    string input_file;
    string output_file;

    if (argc == 2) {
        input_file = argv[1];
        output_file = make_default_output_file(input_file);
    } else if (argc == 4 && string(argv[1]) == "-o") {
        output_file = argv[2];
        input_file = argv[3];
    } else {
        fprintf(stderr, "Usage: %s [-o output-file] <input-file>\n", argv[0]);
        return 1;
    }

    yyin = fopen(input_file.c_str(), "r");
    if (yyin == NULL) {
        perror(input_file.c_str());
        return 1;
    }

    if (yyparse() == 0) {
        printf("Parse succeeded.\n");
    } else {
        printf("Parse failed.\n");
        return -1;
    }

    fclose(yyin);
    yylex_destroy();

    if (!assemble_program_to_file(output_file, lines)) {
        fprintf(stderr, "Assembly failed.\n");
        return 1;
    }

    return 0;
}
