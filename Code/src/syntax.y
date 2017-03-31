%locations
%{
#include <stdio.h>
#include "syntax_tree.h"
syntax_tree_type * syntax_tree_root;
int error_state;
#define YYSTYPE syntax_tree_type*
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
	build_tree(&$$, "Program", 1, $1);
	if(!error_state) syntax_tree_root = $$; 
}
;
ExtDefList : ExtDef ExtDefList {build_tree(&$$, "ExtDefList", 2, $1, $2);}
|
;
ExtDef : Specifier ExtDecList SEMI {build_tree(&$$, "ExtDef", 3, $1, $2, $3);}
| Specifier SEMI {build_tree(&$$, "ExtDef", 2, $1, $2);}
| Specifier FunDec CompSt {build_tree(&$$, "ExtDef", 3, $1, $2, $3);}
;
ExtDecList : VarDec {build_tree(&$$, "ExtDecList", 1, $1);}
| VarDec COMMA ExtDecList {build_tree(&$$, "ExtDecList", 3, $1, $2, $3);}
;
/* Specifiers */
Specifier : TYPE {build_tree(&$$, "Specifier", 1, $1);}
| StructSpecifier {build_tree(&$$, "Specifier", 1, $1);}
;
StructSpecifier : STRUCT OptTag LC DefList RC {build_tree(&$$, "StructSpecifier", 5, $1, $2, $3, $4, $5);}
| STRUCT Tag {build_tree(&$$, "StructSpecifier", 2, $1, $2);}
;
OptTag : ID {build_tree(&$$, "OptTag", 1, $1);}
|
;
Tag : ID {build_tree(&$$, "Tag", 1, $1);}
;
/* Declarators */
VarDec : ID {build_tree(&$$, "VaeDec", 1, $1);}
| VarDec LB INT RB {build_tree(&$$, "VarDec", 4, $1, $2, $3, $4);}
;
FunDec : ID LP VarList RP {build_tree(&$$, "FunDec", 4, $1, $2, $3, $4);}
| ID LP RP {build_tree(&$$, "FunDec", 3, $1, $2, $3);}
;
VarList : ParamDec COMMA VarList {build_tree(&$$, "VarList", 3, $1, $2, $3);}
| ParamDec {build_tree(&$$, "VarList", 1, $1);}
;
ParamDec : Specifier VarDec {build_tree(&$$, "ParamDec", 2, $1, $2);}
;
/* Statements */
CompSt : LC DefList StmtList RC {build_tree(&$$, "CompSt", 4, $1, $2, $3, $4);}
;
StmtList : Stmt StmtList {build_tree(&$$, "StmtList", 2, $1, $2);}
| {$$ = NULL;}
;
Stmt : Exp SEMI {build_tree(&$$, "Stmt", 2, $1, $2);}
| CompSt {build_tree(&$$, "Stmt", 1, $1);}
| RETURN Exp SEMI {build_tree(&$$, "Stmt", 3, $1, $2, $3);}
| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {build_tree(&$$, "Stmt", 5, $1, $2, $3, $4, $5);}
| IF LP Exp RP Stmt ELSE Stmt {build_tree(&$$, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7);}
| WHILE LP Exp RP Stmt {build_tree(&$$, "Stmt", 5, $1, $2, $3, $4, $5);}
/*| error SEMI*/
;
/* Local Definitions */
DefList : Def DefList {build_tree(&$$, "DefList", 2, $1, $2);}
| {$$ = NULL;}
;
Def : Specifier DecList SEMI {build_tree(&$$, "Def", 3, $1, $2, $3);}
/*| error SEMI*/
;
DecList : Dec {build_tree(&$$, "DecList", 1, $1);}
| Dec COMMA DecList {build_tree(&$$, "DecList", 3, $1, $2, $3);}
;
Dec : VarDec {build_tree(&$$, "Dec", 1, $1);}
| VarDec ASSIGNOP Exp {build_tree(&$$, "Dec", 3, $1, $2, $3);}
;
/* Expressions */
Exp : Exp ASSIGNOP Exp {build_tree(&$$, "Exp", 3, $1, $2, $3);}
| Exp AND Exp {build_tree(&$$, "Exp", 3, $1, $2, $3);}
| Exp OR Exp {build_tree(&$$, "Exp", 3, $1, $2, $3);}
| Exp RELOP Exp {build_tree(&$$, "Exp", 3, $1, $2, $3);}
| Exp PLUS Exp {build_tree(&$$, "Exp", 3, $1, $2, $3);}
| Exp MINUS Exp {build_tree(&$$, "Exp", 3, $1, $2, $3);}
| Exp STAR Exp {build_tree(&$$, "Exp", 3, $1, $2, $3);}
| Exp DIV Exp {build_tree(&$$, "Exp", 3, $1, $2, $3);}
| LP Exp RP {build_tree(&$$, "Exp", 3, $1, $2, $3);}
| MINUS Exp {build_tree(&$$, "Exp", 2, $1, $2);}
| NOT Exp {build_tree(&$$, "Exp", 2, $1, $2);}
| ID LP Args RP {build_tree(&$$, "Exp", 4, $1, $2, $3, $4);}
| ID LP RP {build_tree(&$$, "Exp", 3, $1, $2, $3);}
| Exp LB Exp RB {build_tree(&$$, "Exp", 4, $1, $2, $3, $4);}
| Exp DOT ID {build_tree(&$$, "Exp", 3, $1, $2, $3);}
| ID {build_tree(&$$, "Exp", 1, $1);}
| INT {build_tree(&$$, "Exp", 1, $1);}
| FLOAT {build_tree(&$$, "Exp", 1, $1);}
;
Args : Exp COMMA Args {build_tree(&$$, "Args", 3, $1, $2, $3);}
| Exp {build_tree(&$$, "Args", 1, $1);}
;
%%
int yyerror(char* msg) {
	error_state = 1;
	fprintf(stderr, "error: %s %d\n", msg, yylloc.first_line);
	return 0;
}
