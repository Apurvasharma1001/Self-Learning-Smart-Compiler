%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
%}

%option noyywrap yylineno

%%

"int"                   { return INT; }          /* Keyword for integer declarations. */
"if"                    { return IF; }           /* Keyword for if statements. */
"else"                  { return ELSE; }         /* Keyword for else branch. */
"while"                 { return WHILE; }        /* Keyword for while loops. */

"<="                    { return LE; }           /* Relational operator: less-than-or-equal. */
">="                    { return GE; }           /* Relational operator: greater-than-or-equal. */
"=="                    { return EQ; }           /* Relational operator: equality. */
"<"                     { return LT; }           /* Relational operator: less-than. */
">"                     { return GT; }           /* Relational operator: greater-than. */

[0-9]+                   { yylval.ival = atoi(yytext); return NUMBER; } /* Integer literals. */
[a-zA-Z_][a-zA-Z0-9_]*   { yylval.strval = strdup(yytext); return IDENTIFIER; } /* Identifiers. */

"{"|"}"|"("|")"|";"|"="|"+"|"-"|"*"|"/"  { return yytext[0]; } /* Single-character tokens. */

[ \t\r\n]+              { /* Ignore whitespace; yylineno is tracked by Flex. */ }

.                        {
                            fprintf(stderr, "Lexical error at line %d: unexpected character '%s'\n", yylineno, yytext);
                          }

%%
