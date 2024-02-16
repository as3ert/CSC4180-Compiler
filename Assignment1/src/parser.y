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
 * This file implements some syntax analysis rules and works as a parser
 * The grammar tree is generated based on the rules and MIPS Code is generated
 * based on the grammar tree and the added structures and functions implemented
 * in File: added_structure_function.c
 */

%code requires {
#include "node.hpp"
}

%{
/* C declarations used in actions */
#include <cstdio>     
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <ctype.h>

#include "node.hpp"

int yyerror (char *s);

int yylex();

extern int cst_only;

Node* root_node = nullptr;
%}

/* TODO: define yylval data types with %union */
%union {
	/* data */
	int intval;
	const char* strval;
	struct Node* nodeval;
};

/* TODO: define terminal symbols with %token. Remember to set the type. */
%token <intval> T_INTLITERAL
%token <strval> T_ID
%token T_BEGIN_ T_END T_READ T_WRITE T_LPAREN T_RPAREN T_SEMICOLON T_COMMA T_ASSIGNOP T_PLUSOP T_MINUSOP T_SCANEOF

/* Start Symbol */
%start start

/* TODO: define Non-Terminal Symbols with %type. Remember to set the type. */
%type <nodeval> program statement_list statement id_list expr_list expression primary add_op

/* %type program statement_list statement id_list expr_list expression primary add_op */

%%

/**
 * Format:
 * Non-Terminal  :  [Non-Terminal, Terminal]+ (production rule 1)   { parser actions in C++ }
 *                  | [Non-Terminal, Terminal]+ (production rule 2) { parser actions in C++ }
 *                  ;
 */

/* TODO: your production rule here */
/* The tree generation logic should be in the operation block of each production rule */
start 	: program T_SCANEOF 
	  	{
			/* Create a new node with the start symbol */
			Node* start = new Node(SymbolClass::START);

			/* Set the children of the start node with the children of parsed program node */
			start->children = $1->children;

			/* Add scanEOF symbol as a child of start node */
			Node* scanEOF = new Node(SymbolClass::SCANEOF);
			start->children.push_back(scanEOF);

			/* Set the start node as the root node of the tree */
			root_node = start;

	  		return 0; 
	  	}
	  	;

program : T_BEGIN_ statement_list T_END 
		{
			/* Create a new node with the program symbol */
			Node* program = new Node(SymbolClass::PROGRAM);

			/* Add begin_ symbol as a child of program node */
			Node* begin_ = new Node(SymbolClass::BEGIN_);
			start->children.push_back(begin_);

			/* Set the children of the program node with the parsed statement list node */
			program->children = $2->children;

			/* Add end symbol as a child of program node */
			Node* end = new Node(SymbolClass::END);
			start->children.push_back(end);

			/* Set the program node as the root node of the tree */
			$$ = program;
		}
		;

statement_list 	: statement
				{
					/* Create a new node with the statement list symbol */
					Node* statement_list = new Node(SymbolClass::STATEMENT_LIST);

					/* Set the children of the statement list node with the children of parsed statement node */
					statement_list->children = $1->children;

					/* Set the statement list node as the root node of the tree */
					$$ = statement_list;
				}
			   	| statement_list statement
			   	{
					/* Set the children of the parsed statement list node with the children of the parsed statement node */
					$1->children.insert($1->children.end(), $2->children.begin(), $2->children.end());

					/* Set the statement list node as the root node of the tree */
					$$ = $1;
			   	}
			   	;

statement 	: T_ID T_ASSIGNOP expression T_SEMICOLON 
			{
		  	}
		  	| T_READ T_LPAREN id_list T_RPAREN T_SEMICOLON 
			{

		  	}
		  	| T_WRITE T_LPAREN expr_list T_RPAREN T_SEMICOLON
		  	;

id_list : T_ID
		| id_list T_COMMA T_ID
		;

expr_list 	: expression
		  	| expr_list T_COMMA expression
		  	;

expression 	: primary
		   	| expression add_op primary 
		   	;

primary : T_LPAREN expression T_RPAREN
		| T_ID
		| T_INTLITERAL
		;

add_op 	: T_PLUSOP { $$ = '+'; }
	   	| T_MINUSOP { $$ = '-'; }
	   	;

%%

int yyerror(char *s) {
	printf("Syntax Error on line %s\n", s);
	return 0;
}
