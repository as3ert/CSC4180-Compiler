/*
 * Copyright (c) 2024 Guangxin Zhao <https://github.com/as3ert>
 * 
 * File Created: 28th February 2024
 * Author: Guangxin Zhao (120090244@link.cuhk.edu.cn)
 * Student ID: 120090244
 * 
 * Description: This file implements some lexical analysis rules to scan the input file 
 *              and return the tokens which will be used later for parser
 */

%{
/* C/C++ Stuff (headers, declarations, variables, etc.) */
#include <cstdio>
#include <string>
#include <vector>

int yyerror (char *s);

int yylex();

int end = 0;
%}

EOLN            "\n"
TAB             "\t"
COMMENT         \/\*([^*]|\*+[^*/])*\*+\/
GLOBAL          "global"
TINT            "int"
TBOOL           "bool"
TSTRING         "str"
NULL            "null"
TRUE            "true"
FALSE           "false"
NEW             "new"
RETURN          "return"
FOR             "for"
WHILE           "while"
IF              "if"
ELSE            "else"
VAR             "var"
TVOID           "void"
LPAREN          "("
RPAREN          ")"
LBRACE          "{"
RBRACE          "}"
LBRACKET        "["
RBRACKET        "]"
SEMICOLON       ";"
COMMA           ","
ASSIGN          "="
STAR            "*"
PLUS            "+"
MINUS           "-"
LSHIFT          "<<"
RLSHIFT          ">>"
RASHIFT         ">>>"
LESS            "<"
LESSEQ          "<="
GREAT           ">"
GREATEQ         ">="
EQ              "=="
NEQ             "!="
LAND            "&"
LOR             "|"
BAND            "[&]"
BOR             "[|]"
NOT             "!"
TILDE           "~"
ID              [a-zA-Z][a-zA-Z0-9_]*
INTLITERAL      [0-9]+
STRINGLITERAL   \"(.)*\"

%%

{EOLN}          { /* Ignore newlines */ }
{TAB}           { /* Ignore tabs */ }
{COMMENT}       { /* Ignore comments */ }
{GLOBAL}        { printf("GLOBAL %s\n", yytext); }
{TINT}          { printf("TINT %s\n", yytext); }
{TBOOL}         { printf("TBOOL %s\n", yytext); }
{TSTRING}       { printf("TSTRING %s\n", yytext); }
{NULL}          { printf("NULL %s\n", yytext); }
{TRUE}          { printf("TRUE %s\n", yytext); }
{FALSE}         { printf("FALSE %s\n", yytext); }
{NEW}           { printf("NEW %s\n", yytext); }
{RETURN}        { printf("RETURN %s\n", yytext); }
{FOR}           { printf("FOR %s\n", yytext); }
{WHILE}         { printf("WHILE %s\n", yytext); }
{IF}            { printf("IF %s\n", yytext); }
{ELSE}          { printf("ELSE %s\n", yytext); }
{VAR}           { printf("VAR %s\n", yytext); }
{TVOID}         { printf("TVOID %s\n", yytext); }
{LPAREN}        { printf("LPAREN %s\n", yytext); }
{RPAREN}        { printf("RPAREN %s\n", yytext); }
{LBRACE}        { printf("LBRACE %s\n", yytext); }
{RBRACE}        { printf("RBRACE %s\n", yytext); }
{LBRACKET}      { printf("LBRACKET %s\n", yytext); }
{RBRACKET}      { printf("RBRACKET %s\n", yytext); }
{SEMICOLON}     { printf("SEMICOLON %s\n", yytext); }
{COMMA}         { printf("COMMA %s\n", yytext); }
{ASSIGN}        { printf("ASSIGN %s\n", yytext); }
{STAR}          { printf("STAR %s\n", yytext); }
{PLUS}          { printf("PLUS %s\n", yytext); }
{MINUS}         { printf("MINUS %s\n", yytext); }
{LSHIFT}        { printf("LSHIFT %s\n", yytext); }
{RLSHIFT}       { printf("RLSHIFT %s\n", yytext); }
{RASHIFT}       { printf("RASHIFT %s\n", yytext); }
{LESS}          { printf("LESS %s\n", yytext); }
{LESSEQ}        { printf("LESSEQ %s\n", yytext); }
{GREAT}         { printf("GREAT %s\n", yytext); }
{GREATEQ}       { printf("GREATEQ %s\n", yytext); }
{EQ}            { printf("EQ %s\n", yytext); }
{NEQ}           { printf("NEQ %s\n", yytext); }
{LAND}          { printf("LAND %s\n", yytext); }
{LOR}           { printf("LOR %s\n", yytext); }
{BAND}          { printf("BAND %s\n", yytext); }
{BOR}           { printf("BOR %s\n", yytext); }
{NOT}           { printf("NOT %s\n", yytext); }
{TILDE}         { printf("TILDE %s\n", yytext); }
{ID}            { printf("ID %s\n", yytext); }
{INTLITERAL}    { printf("INTLITERAL %s\n", yytext); }
{STRINGLITERAL} { printf("STRINGLITERAL %s\n", yytext); }
<<EOF>>         { 
    if (!end) {
        end = 1;
        printf("EOF\n");
    }
    yyterminate();
}

.

%%

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE* file;
        file = fopen(argv[1], "r");
        if (! file) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            exit(1);
        }
        yyin = file;
    }
    
    yylex();
    yywrap();
}

int yywrap (void) { return 1; }
