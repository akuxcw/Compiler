#include "syntax_tree.h"

extern int lines;
void yyparse();
extern syntax_tree_type * syntax_tree_root;
int main() {
	lines = 1;
	yyparse();
	print_tree(syntax_tree_root, 0);
	return 0;
}

