/*
 * Copyright (c) 2024 Guangxin Zhao <https://github.com/as3ert>
 * 
 * File Created: 19th February 2024
 * Author: Guangxin Zhao (120090244@link.cuhk.edu.cn)
 * Student ID: 120090244
 * 
 * Description: This file implements some lexical analysis rules to scan the input file 
 *              and return the tokens which will be used for parser.
 */

%{
/* C/C++ Stuff (headers, declarations, variables, etc.) */
#include <cstdio>
#include <string>
#include <vector>
#include "parser.hpp"
#include "node.hpp"

int yyerror (char *s);

int yylex();

/**
 * scan_only indicates whehter the flex scanner should print out the token information for debug.
 * The token information is in the following form:
 *   <token-class, lexeme>
 */
extern int scan_only;
%}

EOLN        "\n"
TAB         "\t"
COMMENT     --.*\n
BEGIN_      "begin"
END         "end"
READ        "read"
WRITE       "write"
LPAREN      "("
RPAREN      ")"
SEMICOLON   ";"
COMMA       ","
ASSIGNOP    ":="
PLUSOP      "+"
MINUSOP     "-"
ID          [a-zA-Z][a-zA-Z0-9_]{0,31}
INTLITERAL  -?[0-9]+

%%

{EOLN}          { /* Ignore newlines */ }
{TAB}           { /* Ignore tabs */ }
{COMMENT}       { /* Ignore comments */ }
{BEGIN_}        { if(scan_only == 1) { printf("<BEGIN_, %s>\n", yytext); } yylval.strval = strdup(yytext); return T_BEGIN_; }
{END}           { if(scan_only == 1) { printf("<END, %s>\n", yytext); } yylval.strval = strdup(yytext); return T_END; }
{READ}          { if(scan_only == 1) { printf("<READ, %s>\n", yytext); } yylval.strval = strdup(yytext); return T_READ; }
{WRITE}         { if(scan_only == 1) { printf("<WRITE, %s>\n", yytext); } yylval.strval = strdup(yytext); return T_WRITE; }
{LPAREN}        { if(scan_only == 1) { printf("<LPAREN, %s>\n", yytext); } yylval.strval = strdup(yytext); return T_LPAREN; }
{RPAREN}        { if(scan_only == 1) { printf("<RPAREN, %s>\n", yytext); } yylval.strval = strdup(yytext); return T_RPAREN; }
{SEMICOLON}     { if(scan_only == 1) { printf("<SEMICOLON, %s>\n", yytext); } yylval.strval = strdup(yytext); return T_SEMICOLON; }
{COMMA}         { if(scan_only == 1) { printf("<COMMA, %s>\n", yytext); } yylval.strval = strdup(yytext); return T_COMMA; }
{ASSIGNOP}      { if(scan_only == 1) { printf("<ASSIGNOP, %s>\n", yytext); } yylval.strval = strdup(yytext); return T_ASSIGNOP; }
{PLUSOP}        { if(scan_only == 1) { printf("<PLUSOP, %s>\n", yytext); } yylval.strval = strdup(yytext); return T_PLUSOP; }
{MINUSOP}       { if(scan_only == 1) { printf("<MINUSOP, %s>\n", yytext); } yylval.strval = strdup(yytext); return T_MINUSOP; }
{ID}            { if(scan_only == 1) { printf("<ID, %s>\n", yytext); } yylval.strval = strdup(yytext); return T_ID; }
{INTLITERAL}    { if(scan_only == 1) { printf("<INTLITERAL, %s>\n", yytext); } yylval.intval = std::atoi(yytext); return T_INTLITERAL; }
<<EOF>>         { if(scan_only == 1) { printf("<SCANEOF>\n"); return 0; } return T_SCANEOF; }

.

%%

int yywrap(void) {
    return 1;
}
