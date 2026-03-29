#ifndef AST_H
#define AST_H

#include <stdio.h>

#define MAX_SYMBOLS 256
#define MAX_IR 1024

typedef enum {
    NODE_STMT_LIST,
    NODE_DECL,
    NODE_ASSIGN,
    NODE_BLOCK,
    NODE_IF,
    NODE_WHILE,
    NODE_BINOP,
    NODE_NUM,
    NODE_IDENT
} NodeKind;

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_LT,
    OP_GT,
    OP_LE,
    OP_GE,
    OP_EQ
} OpType;

typedef struct ASTNode ASTNode;

struct ASTNode {
    NodeKind kind;
    union {
        struct {
            ASTNode *statement;
            ASTNode *next;
        } stmt_list;

        struct {
            char name[64];
        } decl;

        struct {
            char name[64];
            ASTNode *expr;
        } assign;

        struct {
            ASTNode *statements;
        } block;

        struct {
            ASTNode *condition;
            ASTNode *then_branch;
            ASTNode *else_branch;
        } if_stmt;

        struct {
            ASTNode *condition;
            ASTNode *body;
        } while_stmt;

        struct {
            OpType op;
            ASTNode *left;
            ASTNode *right;
        } binop;

        struct {
            int value;
        } num;

        struct {
            char name[64];
        } ident;
    } data;
};

typedef struct {
    char op[16];
    char arg1[64];
    char arg2[64];
    char result[64];
} IRInstr;

extern ASTNode *ast_root;
extern int semantic_errors;

/* AST constructors */
ASTNode *make_stmt_list(ASTNode *stmt, ASTNode *next);
ASTNode *make_decl(const char *name);
ASTNode *make_assign(const char *name, ASTNode *expr);
ASTNode *make_block(ASTNode *statements);
ASTNode *make_if(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch);
ASTNode *make_while(ASTNode *cond, ASTNode *body);
ASTNode *make_binop(OpType op, ASTNode *left, ASTNode *right);
ASTNode *make_num(int value);
ASTNode *make_ident(const char *name);
void free_ast(ASTNode *node);

/* Symbol table */
void reset_symbol_table(void);
int symbol_exists(const char *name);
int add_symbol(const char *name);
void print_symbol_table(void);

/* IR */
void reset_ir(void);
void generate_ir(ASTNode *root);
void print_ir(void);
int get_ir_count(void);
IRInstr *get_ir_list(void);

/* Code generation */
void write_c_from_ir(const char *filename);

#endif