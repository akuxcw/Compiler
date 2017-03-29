%option yylineno
%{
#define YYSTYPE double
#include "syntax.h"
int lines;
int yycolumn = 1;
#define YY_USER_ACTION \
	yylloc.first_line = yylloc.last_line = yylineno; \
	yylloc.first_column = yycolumn; \
	yylloc.last_column = yycolumn + yyleng - 1; \
	yycolumn += yyleng;
%}
letter [_a-zA-Z]
digit [0-9]
%%
"//".*$ {/*return NOTE;*/}
"/*"(.|\n)*"*/" {/*return NOTES;*/}
(0|([1-9]+{digit}*)|(0[0-7]+)|(0(x|X)({digit}|[a-fA-F])+)) {yylval = atoi(yytext); return INT;}
{digit}+\.{digit}+([eE][+-]?{digit}+)? {return FLOAT;}
; {return SEMI;}
, {return COMMA;}
= {return ASSIGNOP;}
>|<|(>=)|(<=)|(==)|(!=) {return RELOP;}
\+ {return PLUS;}
- {return MINUS;}
\* {return STAR;}
\/ {return DIV;}
&& {return AND;}
\|\| {return OR;}
\. {return DOT;}
! {return NOT;}
(int)|(float) {return TYPE;}
\( {return LP;}
\) {return RP;}
"[" {return LB;}
"]" {return RB;}
\{ {return LC;}
\} {return RC;}
struct {return STRUCT;}
return {return RETURN;}
if {return IF;}
else {return ELSE;}
while {return WHILE;}
{letter}({letter}|{digit})* {return ID;}
" "|\t {}
\n {lines ++; yycolumn = 1;}
. {printf("Error type A at Line %d: Mysterious characters \'%s\'\n", lines, yytext);}
%%
/*int main(int argc, char** argv) {
	if (argc > 1) {
		if (!(yyin = fopen(argv[1], "r"))) {
			perror(argv[1]);
			return 1;
		}
	}
	yylineno = 1;
	lines = 1;
	yylex();
	return 0;
}*/
