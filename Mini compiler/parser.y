%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

int yylex(void);
extern int yylineno;
void yyerror(const char *s);

ASTNode *ast_root = NULL;
%}

%code requires {
#include "ast.h"
}

%union {
    int ival;
    char *strval;
    ASTNode *node;
    int op;
}

%token INT IF ELSE WHILE
%token <strval> IDENTIFIER
%token <ival> NUMBER
%token LT GT LE GE EQ

%type <node> program stmt_list stmt declaration assignment block if_stmt while_stmt expr condition
%type <op> relop

%left '+' '-'
%left '*' '/'
%nonassoc LT GT LE GE EQ
%nonassoc IFX
%nonassoc ELSE

%%

program
    : stmt_list {
        /* Entire program is a sequence of statements; keep it as AST root. */
        ast_root = $1;
      }
    ;

stmt_list
    : stmt stmt_list {
        /* Link current statement to the rest of the list. */
        $$ = make_stmt_list($1, $2);
      }
    | /* empty */ {
        /* Empty block/program becomes a NULL statement list. */
        $$ = NULL;
      }
    ;

stmt
    : declaration {
        /* A declaration is a standalone statement. */
        $$ = $1;
      }
    | assignment {
        /* Assignment statement. */
        $$ = $1;
      }
    | if_stmt {
        /* If/if-else statement. */
        $$ = $1;
      }
    | while_stmt {
        /* While loop statement. */
        $$ = $1;
      }
    | block {
        /* Compound statement surrounded by braces. */
        $$ = $1;
      }
    ;

declaration
    : INT IDENTIFIER ';' {
        /* Semantic check: reject duplicate declarations in this simple global scope. */
        if (!add_symbol($2)) {
            fprintf(stderr, "Semantic error at line %d: duplicate declaration of '%s'\n", yylineno, $2);
            semantic_errors++;
        }
        $$ = make_decl($2);
        free($2);
      }
    ;

assignment
    : IDENTIFIER '=' expr ';' {
        /* Semantic check: assignment target must be declared. */
        if (!symbol_exists($1)) {
            fprintf(stderr, "Semantic error at line %d: undeclared variable '%s'\n", yylineno, $1);
            semantic_errors++;
        }
        $$ = make_assign($1, $3);
        free($1);
      }
    ;

if_stmt
    : IF '(' condition ')' stmt %prec IFX {
        /* if (cond) stmt */
        $$ = make_if($3, $5, NULL);
      }
    | IF '(' condition ')' stmt ELSE stmt {
        /* if (cond) stmt else stmt */
        $$ = make_if($3, $5, $7);
      }
    ;

while_stmt
    : WHILE '(' condition ')' stmt {
        /* while (cond) stmt */
        $$ = make_while($3, $5);
      }
    ;

block
    : '{' stmt_list '}' {
        /* Braced block containing zero or more statements. */
        $$ = make_block($2);
      }
    ;

condition
    : expr relop expr {
        /* Relational condition represented as a binary op node. */
        $$ = make_binop((OpType)$2, $1, $3);
      }
    ;

relop
    : LT { $$ = OP_LT; }
    | GT { $$ = OP_GT; }
    | LE { $$ = OP_LE; }
    | GE { $$ = OP_GE; }
    | EQ { $$ = OP_EQ; }
    ;

expr
    : expr '+' expr {
        /* Addition node. */
        $$ = make_binop(OP_ADD, $1, $3);
      }
    | expr '-' expr {
        /* Subtraction node. */
        $$ = make_binop(OP_SUB, $1, $3);
      }
    | expr '*' expr {
        /* Multiplication node. */
        $$ = make_binop(OP_MUL, $1, $3);
      }
    | expr '/' expr {
        /* Division node. */
        $$ = make_binop(OP_DIV, $1, $3);
      }
    | '(' expr ')' {
        /* Parenthesized expression. */
        $$ = $2;
      }
    | NUMBER {
        /* Integer literal expression. */
        $$ = make_num($1);
      }
    | IDENTIFIER {
        /* Semantic check: variables in expressions must be declared first. */
        if (!symbol_exists($1)) {
            fprintf(stderr, "Semantic error at line %d: undeclared variable '%s'\n", yylineno, $1);
            semantic_errors++;
        }
        $$ = make_ident($1);
        free($1);
      }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
}
