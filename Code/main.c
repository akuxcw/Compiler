extern int lines;
void yyparse();
int main() {
	lines = 1;
	yyparse();
	return 0;
}

