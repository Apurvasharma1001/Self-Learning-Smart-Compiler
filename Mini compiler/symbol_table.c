#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

int semantic_errors = 0;

static char symbols[MAX_SYMBOLS][64];
static int symbol_count = 0;

static ASTNode *alloc_node(NodeKind kind) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Fatal: out of memory while creating AST node\n");
        exit(1);
    }
    memset(node, 0, sizeof(ASTNode));
    node->kind = kind;
    return node;
}

ASTNode *make_stmt_list(ASTNode *stmt, ASTNode *next) {
    ASTNode *node = alloc_node(NODE_STMT_LIST);
    node->data.stmt_list.statement = stmt;
    node->data.stmt_list.next = next;
    return node;
}

ASTNode *make_decl(const char *name) {
    ASTNode *node = alloc_node(NODE_DECL);
    strncpy(node->data.decl.name, name, sizeof(node->data.decl.name) - 1);
    return node;
}

ASTNode *make_assign(const char *name, ASTNode *expr) {
    ASTNode *node = alloc_node(NODE_ASSIGN);
    strncpy(node->data.assign.name, name, sizeof(node->data.assign.name) - 1);
    node->data.assign.expr = expr;
    return node;
}

ASTNode *make_block(ASTNode *statements) {
    ASTNode *node = alloc_node(NODE_BLOCK);
    node->data.block.statements = statements;
    return node;
}

ASTNode *make_if(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch) {
    ASTNode *node = alloc_node(NODE_IF);
    node->data.if_stmt.condition = cond;
    node->data.if_stmt.then_branch = then_branch;
    node->data.if_stmt.else_branch = else_branch;
    return node;
}

ASTNode *make_while(ASTNode *cond, ASTNode *body) {
    ASTNode *node = alloc_node(NODE_WHILE);
    node->data.while_stmt.condition = cond;
    node->data.while_stmt.body = body;
    return node;
}

ASTNode *make_binop(OpType op, ASTNode *left, ASTNode *right) {
    ASTNode *node = alloc_node(NODE_BINOP);
    node->data.binop.op = op;
    node->data.binop.left = left;
    node->data.binop.right = right;
    return node;
}

ASTNode *make_num(int value) {
    ASTNode *node = alloc_node(NODE_NUM);
    node->data.num.value = value;
    return node;
}

ASTNode *make_ident(const char *name) {
    ASTNode *node = alloc_node(NODE_IDENT);
    strncpy(node->data.ident.name, name, sizeof(node->data.ident.name) - 1);
    return node;
}

void free_ast(ASTNode *node) {
    if (!node) {
        return;
    }

    switch (node->kind) {
        case NODE_STMT_LIST:
            free_ast(node->data.stmt_list.statement);
            free_ast(node->data.stmt_list.next);
            break;
        case NODE_ASSIGN:
            free_ast(node->data.assign.expr);
            break;
        case NODE_BLOCK:
            free_ast(node->data.block.statements);
            break;
        case NODE_IF:
            free_ast(node->data.if_stmt.condition);
            free_ast(node->data.if_stmt.then_branch);
            free_ast(node->data.if_stmt.else_branch);
            break;
        case NODE_WHILE:
            free_ast(node->data.while_stmt.condition);
            free_ast(node->data.while_stmt.body);
            break;
        case NODE_BINOP:
            free_ast(node->data.binop.left);
            free_ast(node->data.binop.right);
            break;
        case NODE_DECL:
        case NODE_NUM:
        case NODE_IDENT:
            break;
    }

    free(node);
}

void reset_symbol_table(void) {
    symbol_count = 0;
    memset(symbols, 0, sizeof(symbols));
}

int symbol_exists(const char *name) {
    int i;
    for (i = 0; i < symbol_count; i++) {
        if (strcmp(symbols[i], name) == 0) {
            return 1;
        }
    }
    return 0;
}

int add_symbol(const char *name) {
    if (symbol_exists(name)) {
        return 0;
    }
    if (symbol_count >= MAX_SYMBOLS) {
        fprintf(stderr, "Symbol table overflow\n");
        return 0;
    }
    strncpy(symbols[symbol_count], name, sizeof(symbols[symbol_count]) - 1);
    symbol_count++;
    return 1;
}

void print_symbol_table(void) {
    int i;
    printf("\nSymbol Table:\n");
    for (i = 0; i < symbol_count; i++) {
        printf("  %s\n", symbols[i]);
    }
}