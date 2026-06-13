#include <stdio.h>
#include <stdlib.h>
#include "asm/code.h"

#include "grammar/grammar.tab.h"
#include "asm/asm_main.h"

extern FILE *yyin;

int yylex_destroy(void);

extern s_asm_file asm_file;

int main(int argc, char **argv)
{
    init_asm_file();

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror(argv[1]);
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

    for (int i = 0; i < asm_file.curr_line; i++)
    {
        char* line = asm_line_to_string(asm_file.lines[i]);
        if (line != NULL) {
            printf("%s", line);
            free(line);
        }
    }

    assemble_file("a");

    return 0;
}
