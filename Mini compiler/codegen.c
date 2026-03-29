#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static int is_temp_name(const char *s) {
    return s && s[0] == 't' && s[1] >= '0' && s[1] <= '9';
}

static void add_unique(char names[][64], int *count, const char *value) {
    int i;
    if (!value || value[0] == '\0') {
        return;
    }
    for (i = 0; i < *count; i++) {
        if (strcmp(names[i], value) == 0) {
            return;
        }
    }
    strncpy(names[*count], value, 63);
    names[*count][63] = '\0';
    (*count)++;
}

void write_c_from_ir(const char *filename) {
    FILE *out;
    IRInstr *code;
    int n;
    int i;
    char vars[MAX_SYMBOLS][64];
    int var_count = 0;
    char temps[MAX_IR][64];
    int temp_count = 0;

    code = get_ir_list();
    n = get_ir_count();

    for (i = 0; i < n; i++) {
        if (strcmp(code[i].op, "DECL") == 0) {
            add_unique(vars, &var_count, code[i].result);
        }
        if (is_temp_name(code[i].result)) {
            add_unique(temps, &temp_count, code[i].result);
        }
    }

    out = fopen(filename, "w");
    if (!out) {
        fprintf(stderr, "Failed to open output file: %s\n", filename);
        return;
    }

    fprintf(out, "#include <stdio.h>\n\n");
    fprintf(out, "int main(void) {\n");

    if (var_count > 0 || temp_count > 0) {
        fprintf(out, "    int ");
        for (i = 0; i < var_count; i++) {
            fprintf(out, "%s", vars[i]);
            if (i != var_count - 1 || temp_count > 0) {
                fprintf(out, ", ");
            }
        }
        for (i = 0; i < temp_count; i++) {
            fprintf(out, "%s", temps[i]);
            if (i != temp_count - 1) {
                fprintf(out, ", ");
            }
        }
        fprintf(out, ";\n");
    }

    for (i = 0; i < n; i++) {
        if (strcmp(code[i].op, "DECL") == 0) {
            continue;
        } else if (strcmp(code[i].op, "ASSIGN") == 0) {
            fprintf(out, "    %s = %s;\n", code[i].result, code[i].arg1);
        } else if (strcmp(code[i].op, "ADD") == 0) {
            fprintf(out, "    %s = %s + %s;\n", code[i].result, code[i].arg1, code[i].arg2);
        } else if (strcmp(code[i].op, "SUB") == 0) {
            fprintf(out, "    %s = %s - %s;\n", code[i].result, code[i].arg1, code[i].arg2);
        } else if (strcmp(code[i].op, "MUL") == 0) {
            fprintf(out, "    %s = %s * %s;\n", code[i].result, code[i].arg1, code[i].arg2);
        } else if (strcmp(code[i].op, "DIV") == 0) {
            fprintf(out, "    %s = %s / %s;\n", code[i].result, code[i].arg1, code[i].arg2);
        } else if (strcmp(code[i].op, "LABEL") == 0) {
            fprintf(out, "%s:\n", code[i].result);
        } else if (strcmp(code[i].op, "GOTO") == 0) {
            fprintf(out, "    goto %s;\n", code[i].result);
        } else if (strcmp(code[i].op, "IF_LT") == 0) {
            fprintf(out, "    if (%s < %s) goto %s;\n", code[i].arg1, code[i].arg2, code[i].result);
        } else if (strcmp(code[i].op, "IF_GT") == 0) {
            fprintf(out, "    if (%s > %s) goto %s;\n", code[i].arg1, code[i].arg2, code[i].result);
        } else if (strcmp(code[i].op, "IF_LE") == 0) {
            fprintf(out, "    if (%s <= %s) goto %s;\n", code[i].arg1, code[i].arg2, code[i].result);
        } else if (strcmp(code[i].op, "IF_GE") == 0) {
            fprintf(out, "    if (%s >= %s) goto %s;\n", code[i].arg1, code[i].arg2, code[i].result);
        } else if (strcmp(code[i].op, "IF_EQ") == 0) {
            fprintf(out, "    if (%s == %s) goto %s;\n", code[i].arg1, code[i].arg2, code[i].result);
        } else if (strcmp(code[i].op, "IF_NE") == 0) {
            fprintf(out, "    if (%s != %s) goto %s;\n", code[i].arg1, code[i].arg2, code[i].result);
        }
    }

    fprintf(out, "    return 0;\n");
    fprintf(out, "}\n");

    fclose(out);
}