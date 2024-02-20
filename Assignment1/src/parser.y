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
Node* temp = nullptr;
%}

/* Define yylval data types with %union */
%union {
	/* Data */
	int intval;
	const char* strval;
	struct Node* nodeval;
};

/* Define terminal symbols with %token. Remember to set the type. */
%token <intval> T_INTLITERAL
%token <strval> T_BEGIN_ T_END T_READ T_WRITE T_LPAREN T_RPAREN T_SEMICOLON 
%token <strval> T_COMMA T_ASSIGNOP T_PLUSOP T_MINUSOP T_SCANEOF T_ID

/* Start Symbol */
%start start

/* Define Non-Terminal Symbols with %type. Remember to set the type. */
%type <nodeval> program statement_list statement id_list expr_list expression primary

%%

/**
 * Format:
 * Non-Terminal  :  [Non-Terminal, Terminal]+ (production rule 1)   { parser actions in C++ }
 *                  | [Non-Terminal, Terminal]+ (production rule 2) { parser actions in C++ }
 *                  ;
 */

/* Production rule */
/* The tree generation logic should be in the operation block of each production rule */
start 	: program T_SCANEOF 
	  	{
			// printf("Processing start rule\n");

			if (cst_only == 1) {
				Node* start = new Node(SymbolClass::START);

				start->append_child($1);
				start->append_child(new Node(SymbolClass::SCANEOF));

				root_node = start;
			} else {
				root_node = $1;
			}

	  		return 0; 
	  	}
	  	;

program : T_BEGIN_ statement_list T_END 
		{
			// printf("Processing program rule\n");

			Node* program = new Node(SymbolClass::PROGRAM);

			if (cst_only == 1) {
				program->append_child(new Node(SymbolClass::BEGIN_));
				program->append_child($2);
				program->append_child(new Node(SymbolClass::END));
			} else {
				program->append_child($2);
			}

			$$ = program;
		}
		;

statement_list 	: statement
				{
					// printf("Processing statement_list rule (single statement)\n");

					Node* statement_list = new Node(SymbolClass::STATEMENT_LIST);

					statement_list->append_child($1);

					$$ = statement_list;
				}
			   	| statement_list statement
			   	{
					// printf("Processing statement_list rule (additional statement)\n");

					if (cst_only == 1) {
						Node* statement_list = new Node(SymbolClass::STATEMENT_LIST);

						statement_list->append_child($1);
						statement_list->append_child($2);

						$$ = statement_list;
					} else {
						$1->append_child($2);

						$$ = $1;
					}
			   	}
			   	;

statement 	: T_ID T_ASSIGNOP expression T_SEMICOLON 
			{
				// printf("Processing assignment statement rule\n");

				if (cst_only == 1) {
					Node* statement = new Node(SymbolClass::STATEMENT);

					statement->append_child(new Node(SymbolClass::ID));
					statement->append_child(new Node(SymbolClass::ASSIGNOP));
					statement->append_child($3);
					statement->append_child(new Node(SymbolClass::SEMICOLON));

					$$ = statement;
				} else {
					Node* assignop = new Node(SymbolClass::ASSIGNOP, $2);

					assignop->append_child(new Node(SymbolClass::ID, $1));
					assignop->append_child($3);

					$$ = assignop;
				}
		  	}
		  	| T_READ T_LPAREN id_list T_RPAREN T_SEMICOLON 
			{
				// printf("Processing read statement rule\n");
				
				if (cst_only == 1) {
					Node* statement = new Node(SymbolClass::STATEMENT);

					statement->append_child(new Node(SymbolClass::READ));
					statement->append_child(new Node(SymbolClass::LPAREN));
					statement->append_child($3);
					statement->append_child(new Node(SymbolClass::RPAREN));
					statement->append_child(new Node(SymbolClass::SEMICOLON));

					$$ = statement;
				} else {
					Node* read = new Node(SymbolClass::READ, $1);

					if ($3->children.size()) {
						read->children = $3->children;
					} else {
						read->append_child($3);
					}

					$$ = read;
				}
		  	}
		  	| T_WRITE T_LPAREN expr_list T_RPAREN T_SEMICOLON
			{
				// printf("Processing write statement rule\n");

				if (cst_only == 1) {
					Node* statement = new Node(SymbolClass::STATEMENT);

					statement->append_child(new Node(SymbolClass::WRITE));
					statement->append_child(new Node(SymbolClass::LPAREN));
					statement->append_child($3);
					statement->append_child(new Node(SymbolClass::RPAREN));
					statement->append_child(new Node(SymbolClass::SEMICOLON));

					$$ = statement;
				} else {
					Node* write = new Node(SymbolClass::WRITE, $1);

					if ($3->children.size() && !$3->should_preserver_in_ast()) {
						write->children = $3->children;
					} else {
						write->append_child($3);
					}

					$$ = write;
				}
			}
		  	;

id_list : T_ID
		{
			// printf("Processing id_list rule (single ID)\n");
				
			Node* id_list = new Node(SymbolClass::ID_LIST);

			if (cst_only == 1) {
				id_list->append_child(new Node(SymbolClass::ID));
			} else {
				id_list->append_child(new Node(SymbolClass::ID, $1));
			}

			$$ = id_list;
		}
		| id_list T_COMMA T_ID
		{
			// printf("Processing id_list rule (additional ID)\n");

			Node* id_list = new Node(SymbolClass::ID_LIST);

			if (cst_only == 1) {
				id_list->append_child(new Node(SymbolClass::ID));
				id_list->append_child(new Node(SymbolClass::COMMA));
				id_list->append_child($1);
			} else {
				if ($1->children.size()) {
					id_list->children = $1->children;
					id_list->append_child(new Node(SymbolClass::ID, $3));
				} else {
					Node* id_list = new Node(SymbolClass::ID_LIST);

					id_list->append_child($1);
					id_list->append_child(new Node(SymbolClass::ID, $3));
				}
			}

			$$ = id_list;
		}
		;

expr_list 	: expression
			{
				// printf("Processing expr_list rule (single expression)\n");

				if (cst_only == 1) {
					Node* expr_list = new Node(SymbolClass::EXPRESSION_LIST);

					expr_list->append_child($1);

					$$ = expr_list;
				} else {
					$$ = $1;
				}
			}
		  	| expr_list T_COMMA expression
		  	{
				// printf("Processing expr_list rule (additional expression)\n");

				if (cst_only == 1) {
					Node* expr_list = new Node(SymbolClass::EXPRESSION_LIST);

					expr_list->append_child($1);
					expr_list->append_child(new Node(SymbolClass::COMMA));
					expr_list->append_child($3);

					$$ = expr_list;
				} else {
					if ($1->children.size()) {
						$1->append_child($3);

						$$ = $1;
					} else {
						Node* expr_list = new Node(SymbolClass::EXPRESSION_LIST);

						expr_list->append_child($1);
						expr_list->append_child($3);

						$$ = expr_list;
					}
				}
		  	}
		  	;

expression 	: primary
			{
				// printf("Processing expression rule (primary)\n");

				if (cst_only == 1) {
					Node* expression = new Node(SymbolClass::EXPRESSION);

					expression->append_child($1);

					$$ = expression;
				} else {
					$$ = $1;
				}
			}
		   	| expression T_PLUSOP primary 
		   	{
				// printf("Processing plus expression rule (expression + primary)\n");

				if (cst_only == 1) {
					Node* expression = new Node(SymbolClass::EXPRESSION);

					expression->append_child($1);
					expression->append_child(new Node(SymbolClass::PLUSOP));
					expression->append_child($3);

					$$ = expression;
				} else {
					Node* plusop = new Node(SymbolClass::PLUSOP, $2);

					plusop->append_child($1);
					plusop->append_child($3);

					$$ = plusop;
				}
		   	}
			| expression T_MINUSOP primary
			{
				// printf("Processing minus expression rule (expression + primary)\n");

				if (cst_only == 1) {
					Node* expression = new Node(SymbolClass::EXPRESSION);

					expression->append_child($1);
					expression->append_child(new Node(SymbolClass::MINUSOP));
					expression->append_child($3);

					$$ = expression;
				} else {
					Node* minusop = new Node(SymbolClass::MINUSOP, $2);

					minusop->append_child($1);
					minusop->append_child($3);

					$$ = minusop;
				}
			}
		   	;

primary : T_LPAREN expression T_RPAREN
		{
			// printf("Processing primary rule (parenthesized expression)\n");

			if (cst_only == 1) {
				Node* primary = new Node(SymbolClass::PRIMARY);

				primary->append_child(new Node(SymbolClass::LPAREN));
				primary->append_child($2);
				primary->append_child(new Node(SymbolClass::RPAREN));

				$$ = primary;
			} else {
				$$ = $2;
			}
		}
		| T_ID
		{
			// printf("Processing primary rule (ID)\n");

			if (cst_only == 1) {
				Node* primary = new Node(SymbolClass::PRIMARY);

				primary->append_child(new Node(SymbolClass::ID));

				$$ = primary;
			} else {
				$$ = new Node(SymbolClass::ID, $1);
			}
		}
		| T_INTLITERAL
		{
			// printf("Processing primary rule (integer literal)\n");

			if (cst_only == 1) {
				Node* primary = new Node(SymbolClass::PRIMARY);

				primary->append_child(new Node(SymbolClass::INTLITERAL));

				$$ = primary;
			} else {
				char intstr[11];
				sprintf(intstr, "%d", $1);

				$$ = new Node(SymbolClass::INTLITERAL, intstr);
			}
		}
		;

%%

int yyerror(char *s) {
	printf("Syntax Error on line %s\n", s);
	return 1;
}