#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static IRInstr ir[MAX_IR];
static int ir_count = 0;
static int temp_count = 0;
static int label_count = 0;

static void emit(const char *op, const char *arg1, const char *arg2, const char *result) {
    if (ir_count >= MAX_IR) {
        fprintf(stderr, "IR buffer overflow\n");
        exit(1);
    }

    strncpy(ir[ir_count].op, op ? op : "", sizeof(ir[ir_count].op) - 1);
    strncpy(ir[ir_count].arg1, arg1 ? arg1 : "", sizeof(ir[ir_count].arg1) - 1);
    strncpy(ir[ir_count].arg2, arg2 ? arg2 : "", sizeof(ir[ir_count].arg2) - 1);
    strncpy(ir[ir_count].result, result ? result : "", sizeof(ir[ir_count].result) - 1);
    ir_count++;
}

static void new_temp(char *out) {
    sprintf(out, "t%d", temp_count++);
}

static void new_label(char *out) {
    sprintf(out, "L%d", label_count++);
}

static void gen_expr(ASTNode *node, char *out);
static void gen_stmt(ASTNode *node);

static int is_relop(OpType op) {
    return op == OP_LT || op == OP_GT || op == OP_LE || op == OP_GE || op == OP_EQ;
}

static void gen_condition(ASTNode *cond, const char *true_label, const char *false_label) {
    char left[64];
    char right[64];

    if (cond && cond->kind == NODE_BINOP && is_relop(cond->data.binop.op)) {
        gen_expr(cond->data.binop.left, left);
        gen_expr(cond->data.binop.right, right);

        switch (cond->data.binop.op) {
            case OP_LT: emit("IF_LT", left, right, true_label); break;
            case OP_GT: emit("IF_GT", left, right, true_label); break;
            case OP_LE: emit("IF_LE", left, right, true_label); break;
            case OP_GE: emit("IF_GE", left, right, true_label); break;
            case OP_EQ: emit("IF_EQ", left, right, true_label); break;
            default: break;
        }
        emit("GOTO", "", "", false_label);
        return;
    }

    gen_expr(cond, left);
    emit("IF_NE", left, "0", true_label);
    emit("GOTO", "", "", false_label);
}

static void gen_stmt_list(ASTNode *list) {
    ASTNode *cursor = list;
    while (cursor) {
        if (cursor->kind != NODE_STMT_LIST) {
            gen_stmt(cursor);
            break;
        }
        gen_stmt(cursor->data.stmt_list.statement);
        cursor = cursor->data.stmt_list.next;
    }
}

static void gen_expr(ASTNode *node, char *out) {
    char left[64];
    char right[64];
    char temp[64];

    if (!node) {
        strcpy(out, "0");
        return;
    }

    switch (node->kind) {
        case NODE_NUM:
            sprintf(out, "%d", node->data.num.value);
            break;
        case NODE_IDENT:
            strcpy(out, node->data.ident.name);
            break;
        case NODE_BINOP:
            if (is_relop(node->data.binop.op)) {
                /* Relational expressions are usually compiled via gen_condition. */
                gen_expr(node->data.binop.left, left);
                gen_expr(node->data.binop.right, right);
                new_temp(temp);
                emit("CMP", left, right, temp);
                strcpy(out, temp);
                break;
            }

            gen_expr(node->data.binop.left, left);
            gen_expr(node->data.binop.right, right);
            new_temp(temp);
            switch (node->data.binop.op) {
                case OP_ADD: emit("ADD", left, right, temp); break;
                case OP_SUB: emit("SUB", left, right, temp); break;
                case OP_MUL: emit("MUL", left, right, temp); break;
                case OP_DIV: emit("DIV", left, right, temp); break;
                default: break;
            }
            strcpy(out, temp);
            break;
        default:
            strcpy(out, "0");
            break;
    }
}

static void gen_stmt(ASTNode *node) {
    char value[64];
    char ltrue[64];
    char lfalse[64];
    char lend[64];
    char lstart[64];
    char lbody[64];

    if (!node) {
        return;
    }

    switch (node->kind) {
        case NODE_STMT_LIST:
            gen_stmt_list(node);
            break;
        case NODE_DECL:
            emit("DECL", "", "", node->data.decl.name);
            break;
        case NODE_ASSIGN:
            gen_expr(node->data.assign.expr, value);
            emit("ASSIGN", value, "", node->data.assign.name);
            break;
        case NODE_BLOCK:
            gen_stmt_list(node->data.block.statements);
            break;
        case NODE_IF:
            if (node->data.if_stmt.else_branch) {
                new_label(ltrue);
                new_label(lfalse);
                new_label(lend);

                gen_condition(node->data.if_stmt.condition, ltrue, lfalse);
                emit("LABEL", "", "", ltrue);
                gen_stmt(node->data.if_stmt.then_branch);
                emit("GOTO", "", "", lend);
                emit("LABEL", "", "", lfalse);
                gen_stmt(node->data.if_stmt.else_branch);
                emit("LABEL", "", "", lend);
            } else {
                new_label(ltrue);
                new_label(lend);

                gen_condition(node->data.if_stmt.condition, ltrue, lend);
                emit("LABEL", "", "", ltrue);
                gen_stmt(node->data.if_stmt.then_branch);
                emit("LABEL", "", "", lend);
            }
            break;
        case NODE_WHILE:
            new_label(lstart);
            new_label(lbody);
            new_label(lend);

            emit("LABEL", "", "", lstart);
            gen_condition(node->data.while_stmt.condition, lbody, lend);
            emit("LABEL", "", "", lbody);
            gen_stmt(node->data.while_stmt.body);
            emit("GOTO", "", "", lstart);
            emit("LABEL", "", "", lend);
            break;
        default:
            break;
    }
}

void reset_ir(void) {
    memset(ir, 0, sizeof(ir));
    ir_count = 0;
    temp_count = 0;
    label_count = 0;
}

void generate_ir(ASTNode *root) {
    gen_stmt(root);
}

void print_ir(void) {
    int i;
    printf("\nThree Address Code (IR):\n");
    for (i = 0; i < ir_count; i++) {
        printf("%3d: (%s, %s, %s, %s)\n", i, ir[i].op, ir[i].arg1, ir[i].arg2, ir[i].result);
    }
}

int get_ir_count(void) {
    return ir_count;
}

IRInstr *get_ir_list(void) {
    return ir;
}