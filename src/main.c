#include <stdio.h>

#include "grammar/grammar.tab.h"

typedef struct yy_buffer_state *YY_BUFFER_STATE;

YY_BUFFER_STATE yy_scan_string(const char *yy_str);
void yy_delete_buffer(YY_BUFFER_STATE buffer);
int yylex_destroy(void);
void yyerror(const char *message);

static void print_source_with_line_numbers(const char *source)
{
    int line = 1;

    printf("Input assembly:\n");
    printf("%3d | ", line);

    for (const char *current = source; *current != '\0'; ++current) {
        putchar(*current);

        if (*current == '\n' && current[1] != '\0') {
            printf("%3d | ", ++line);
        }
    }
}

int main(void)
{
    const char *source =
        "halt\n"
        "int\n"
        "iret\n"
        "ret\n";

    YY_BUFFER_STATE buffer = yy_scan_string(source);

    print_source_with_line_numbers(source);

    if (yyparse() == 0) {
        printf("\nParse succeeded.\n");
    } else {
        printf("\nParse failed.\n");
    }

    yy_delete_buffer(buffer);
    yylex_destroy();

    return 0;
}

void yyerror(const char *message)
{
    fprintf(stderr, "Parser error: %s\n", message);
}
