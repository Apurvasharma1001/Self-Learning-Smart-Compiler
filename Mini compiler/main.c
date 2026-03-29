#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern FILE *yyin;
int yyparse(void);

int main(int argc, char **argv) {
    FILE *input;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Failed to open input file: %s\n", argv[1]);
        return 1;
    }

    reset_symbol_table();
    yyin = input;

    if (yyparse() != 0) {
        fclose(input);
        return 1;
    }

    fclose(input);

    if (semantic_errors > 0) {
        fprintf(stderr, "Compilation failed with %d semantic error(s).\n", semantic_errors);
        free_ast(ast_root);
        return 1;
    }

    reset_ir();
    generate_ir(ast_root);
    print_ir();

    write_c_from_ir("output.c");
    printf("\nGenerated C code written to output.c\n");

    free_ast(ast_root);
    return 0;
}