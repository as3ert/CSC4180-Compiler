/**
 * --------------------------------------
 * CUHK-SZ CSC4180: Compiler Construction
 * Assignment 1: Micro Language Compiler
 * --------------------------------------
 * Author: Mr.Liu Yuxuan
 * Position: Teaching Assisant
 * Date: January 25th, 2024
 * Email: yuxuanliu1@link.cuhk.edu.cn
 * 
 * This file implements some lexical analysis rules to scan the input file 
 * and return the tokens which will be used for parser 
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
{COMMENT}       { /* Ignore comments */ }
{BEGIN_}        {printf("<BEGIN_, %s>\n", yytext); return T_BEGIN_;}
{END}           {printf("<END, %s>\n", yytext); return T_END;}
{READ}          {printf("<READ, %s>\n", yytext); return T_READ;}
{WRITE}         {printf("<WRITE, %s>\n", yytext); return T_WRITE;}
{LPAREN}        {printf("<LPAREN, %s>\n", yytext); return T_LPAREN;}
{RPAREN}        {printf("<RPAREN, %s>\n", yytext); return T_RPAREN;}
{SEMICOLON}     {printf("<SEMICOLON, %s>\n", yytext); return T_SEMICOLON;}
{COMMA}         {printf("<COMMA, %s>\n", yytext); return T_COMMA;}
{ASSIGNOP}      {printf("<ASSIGNOP, %s>\n", yytext); return T_ASSIGNOP;}
{PLUSOP}        {printf("<PLUSOP, %s>\n", yytext); return T_PLUSOP;}
{MINUSOP}       {printf("<MINUSOP, %s>\n", yytext); return T_MINUSOP;}
{ID}            {printf("<ID, %s>\n", yytext); return T_ID;}
{INTLITERAL}    {printf("<INTLITERAL, %s>\n", yytext); return T_INTLITERAL;}
<<EOF>>         {printf("<SCANEOF, %s>\n", yytext); yyterminate();}
.

%%

/* This line generates repeated <SCANEOF, <<EOF>>> outputs caused by the return value */
/* TODO: Fix this return issue */
/* <<EOF>>         {printf("<SCANEOF, <<EOF>>>\n"); return T_SCANEOF; yyterminate();} */

int yywrap(void) {
    return 1;
}
