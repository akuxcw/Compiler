#include "syntax_tree.h"
#include "semantics.h"
extern int lines;
void yyparse();
extern SyntaxTreeType * syntax_tree_root;

int main() {
	lines = 1;
	yyparse();
//	PrintTree(syntax_tree_root, 0);
	init_symbol();
	Program(syntax_tree_root);
	return 0;
}

