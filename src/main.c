#include <stdio.h>
#include "asm/code.h"

#include "grammar/grammar.tab.h"

extern FILE *yyin;

int yylex_destroy(void);

asm_line* lines[1024];
int curr_line = 0;

int main(int argc, char **argv)
{
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
    }

    fclose(yyin);
    yylex_destroy();

    for (int i = 0; i < curr_line; i++)
    {
        print_asm_line(lines[i]);
    }

    return 0;
}
