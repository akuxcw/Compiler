%locations
%{
#include <stdio.h>
#include "syntax_tree.h"
SyntaxTreeType * syntax_tree_root;
int error_state;
#define YYSTYPE SyntaxTreeType*
%} 

%token  INT
%token  FLOAT
%token  SEMI COMMA
%token  ASSIGNOP RELOP PLUS MINUS STAR DIV 
%token  AND OR DOT NOT
%token  TYPE STRUCT RETURN IF ELSE WHILE
%token  LP RP LB RB LC RC
%token  ID

%right ASSIGNOP
%left AND OR
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%
/* High-level Definations */
Program : ExtDefList {
	BuildTree(&$$, "Program", 1, $1);
	if(!error_state) syntax_tree_root = $$; 
}
;
ExtDefList : ExtDef ExtDefList {BuildTree(&$$, "ExtDefList", 2, $1, $2);}
| {$$ = NULL;}
;
ExtDef : Specifier ExtDecList SEMI {BuildTree(&$$, "ExtDef", 3, $1, $2, $3);}
| Specifier SEMI {BuildTree(&$$, "ExtDef", 2, $1, $2);}
| Specifier FunDec CompSt {BuildTree(&$$, "ExtDef", 3, $1, $2, $3);}
| Error_state
;
ExtDecList : VarDec {BuildTree(&$$, "ExtDecList", 1, $1);}
| VarDec COMMA ExtDecList {BuildTree(&$$, "ExtDecList", 3, $1, $2, $3);}
;
/* Specifiers */
Specifier : TYPE {BuildTree(&$$, "Specifier", 1, $1);}
| StructSpecifier {BuildTree(&$$, "Specifier", 1, $1);}
;
StructSpecifier : STRUCT OptTag LC DefList RC {BuildTree(&$$, "StructSpecifier", 5, $1, $2, $3, $4, $5);}
| STRUCT Tag {BuildTree(&$$, "StructSpecifier", 2, $1, $2);}
;
OptTag : ID {BuildTree(&$$, "OptTag", 1, $1);}
| {$$ = NULL;}
;
Tag : ID {BuildTree(&$$, "Tag", 1, $1);}
;
/* Declarators */
VarDec : ID {BuildTree(&$$, "VarDec", 1, $1);}
| VarDec LB INT RB {BuildTree(&$$, "VarDec", 4, $1, $2, $3, $4);}
;
FunDec : ID LP VarList RP {BuildTree(&$$, "FunDec", 4, $1, $2, $3, $4);}
| ID LP RP {BuildTree(&$$, "FunDec", 3, $1, $2, $3);}
;
VarList : ParamDec COMMA VarList {BuildTree(&$$, "VarList", 3, $1, $2, $3);}
| ParamDec {BuildTree(&$$, "VarList", 1, $1);}
;
ParamDec : Specifier VarDec {BuildTree(&$$, "ParamDec", 2, $1, $2);}
;
/* Statements */
CompSt : LC DefList StmtList RC {BuildTree(&$$, "CompSt", 4, $1, $2, $3, $4);}
;
StmtList : Stmt StmtList {BuildTree(&$$, "StmtList", 2, $1, $2);}
| {$$ = NULL;}
;
Stmt : Exp SEMI {BuildTree(&$$, "Stmt", 2, $1, $2);}
| CompSt {BuildTree(&$$, "Stmt", 1, $1);}
| RETURN Exp SEMI {BuildTree(&$$, "Stmt", 3, $1, $2, $3);}
| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {BuildTree(&$$, "Stmt", 5, $1, $2, $3, $4, $5);}
| IF LP Exp RP Stmt ELSE Stmt {BuildTree(&$$, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7);}
| WHILE LP Exp RP Stmt {BuildTree(&$$, "Stmt", 5, $1, $2, $3, $4, $5);}
| Error_state
/*| error SEMI*/
;
/* Local Definitions */
DefList : Def DefList {BuildTree(&$$, "DefList", 2, $1, $2);}
| {$$ = NULL;}
;
Def : Specifier DecList SEMI {BuildTree(&$$, "Def", 3, $1, $2, $3);}
/*| error SEMI*/
;
DecList : Dec {BuildTree(&$$, "DecList", 1, $1);}
| Dec COMMA DecList {BuildTree(&$$, "DecList", 3, $1, $2, $3);}
;
Dec : VarDec {BuildTree(&$$, "Dec", 1, $1);}
| VarDec ASSIGNOP Exp {BuildTree(&$$, "Dec", 3, $1, $2, $3);}
;
/* Expressions */
Exp : Exp ASSIGNOP Exp {BuildTree(&$$, "Exp", 3, $1, $2, $3);}
| Exp AND Exp {BuildTree(&$$, "Exp", 3, $1, $2, $3);}
| Exp OR Exp {BuildTree(&$$, "Exp", 3, $1, $2, $3);}
| Exp RELOP Exp {BuildTree(&$$, "Exp", 3, $1, $2, $3);}
| Exp PLUS Exp {BuildTree(&$$, "Exp", 3, $1, $2, $3);}
| Exp MINUS Exp {BuildTree(&$$, "Exp", 3, $1, $2, $3);}
| Exp STAR Exp {BuildTree(&$$, "Exp", 3, $1, $2, $3);}
| Exp DIV Exp {BuildTree(&$$, "Exp", 3, $1, $2, $3);}
| LP Exp RP {BuildTree(&$$, "Exp", 3, $1, $2, $3);}
| MINUS Exp {BuildTree(&$$, "Exp", 2, $1, $2);}
| NOT Exp {BuildTree(&$$, "Exp", 2, $1, $2);}
| ID LP Args RP {BuildTree(&$$, "Exp", 4, $1, $2, $3, $4);}
| ID LP RP {BuildTree(&$$, "Exp", 3, $1, $2, $3);}
| Exp LB Exp RB {BuildTree(&$$, "Exp", 4, $1, $2, $3, $4);}
| Exp DOT ID {BuildTree(&$$, "Exp", 3, $1, $2, $3);}
| ID {BuildTree(&$$, "Exp", 1, $1);}
| INT {BuildTree(&$$, "Exp", 1, $1);}
| FLOAT {BuildTree(&$$, "Exp", 1, $1);}
;
Args : Exp COMMA Args {BuildTree(&$$, "Args", 3, $1, $2, $3);}
| Exp {BuildTree(&$$, "Args", 1, $1);}
Error_state : error SEMI;
;
%%
int yyerror(const char* msg) {
	error_state = 1;
	fprintf(stderr, "Error type B at Line %d: %s.\n", yylloc.first_line, msg);
	return 0;
}
