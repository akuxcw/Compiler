%option yylineno
%{
#include "syntax_tree.h"	
#define YYSTYPE syntax_tree_type*
#include "syntax.h"
int lines;
extern int error_state;
int yycolumn = 1;
char errmsg[] = "Mysterious characters \' \'.";
#define YY_USER_ACTION \
	yylloc.first_line = yylloc.last_line = yylineno; \
	yylloc.first_column = yycolumn; \
	yylloc.last_column = yycolumn + yyleng - 1; \
	yycolumn += yyleng;

#define work(token) \
	yylval = (YYSTYPE)malloc(sizeof(syntax_tree_type)); \
	yylval->name = toStr(#token); \
	yylval->line_no = yylineno; \
	yylval->type = TERMINAL; \
	if(!strcmp(#token, "INT")) yylval->int_val = toInt(yytext); else \
	if(!strcmp(#token, "FLOAT")) yylval->float_val = toFloat(yytext); else \
	yylval->str_val = toStr(yytext); \
	return token;

%}
letter [_a-zA-Z]
digit [0-9]
%%
"//".*$ {/*work(NOTE;*/}
\/\* {
	char ch1 = 0, ch2 = input();
	while (ch2 > 0 && !((ch1 == '*')&&(ch2 == '/'))) {
		ch1 = ch2;
		ch2 = input();
	}
//	printf("%s\n", yytext);/*work(NOTES;*/
}
(([0-9]+{digit}*)|(0(x|X)({digit}|[a-zA-Z])+)) {work(INT)}
{digit}+\.{digit}+([eE][+-]?{digit}+)? {work(FLOAT)}
; {work(SEMI)}
, {work(COMMA)}
= {work(ASSIGNOP)}
>|<|(>=)|(<=)|(==)|(!=) {work(RELOP)}
\+ {work(PLUS)}
- {work(MINUS)}
\* {work(STAR)}
\/ {work(DIV)}
&& {work(AND)}
\|\| {work(OR)}
\. {work(DOT)}
! {work(NOT)}
(int)|(float) {work(TYPE)}
\( {work(LP)}
\) {work(RP)}
"[" {work(LB)}
"]" {work(RB)}
\{ {work(LC)}
\} {work(RC)}
struct {work(STRUCT)}
return {work(RETURN)}
if {work(IF)}
else {work(ELSE)}
while {work(WHILE)}
{letter}({letter}|{digit})* {work(ID)}
" "|\t {}
\n {lines ++; yycolumn = 1;}
. {errmsg[23] = yytext[0]; lerror(errmsg);}
%%
void lerror(const char * str) {
	printf("Error type A at Line %d: %s\n", lines, str); 
	error_state = 1;
}
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
