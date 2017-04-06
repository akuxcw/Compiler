#ifndef COMPILER_SYNTAX_TREE_H_
#define COMPILER_SYNTAX_TREE_H_
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
enum {TERMINAL, NONTERMINAL};
typedef struct syntax_tree_type {
	int line_no, type; 	//type = 0 terminal symbol, type=1 nonterminal symbol
	char * name;
	union {
		int int_val;
		double float_val;
		char * str_val;
	};
	struct syntax_tree_type *child, *next;
} syntax_tree_type;

void build_tree(syntax_tree_type **, const char *, int, ...);
void print_tree(syntax_tree_type *, int);
char * toStr(const char *);
int toInt(const char *);
double toFloat(const char *);
int toupper(int);
void lerror(const char *);
int fileno(FILE *);
int yylex();
int yyerror(const char *);

#endif // COMPILER_SYNTAX_TREE_H_
