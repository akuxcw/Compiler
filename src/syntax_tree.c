#include <stdio.h>
#include "syntax_tree.h"

char * toStr(const char * str) {
	char * tmp = malloc(sizeof(str));
	strcpy(tmp, str);
	return tmp;
}

int toInt(const char * str) {
	return atoi(str);
}

double toFloat(const char * str) {
	return 1.4;
}

void build_tree(syntax_tree_type ** node, const char * name, int carg, ...) {
	*node = (syntax_tree_type *)malloc(sizeof(syntax_tree_type));
	syntax_tree_type * root = *node; 
	va_list ap;
	va_start(ap, carg);
	int i;
	syntax_tree_type * child;
	root->type = NONTERMINAL;
	for(i = 0; i < carg; ++ i) {
		child = va_arg(ap, syntax_tree_type *);
		if(root->child == NULL) {
			root->name = toStr(name);
			root->line_no = child->line_no;
			root->child = child;
		} else {
			syntax_tree_type * tmp = root->child;
			while(tmp->next != NULL) tmp = tmp->next;
			tmp->next = child;
		}
	}
	va_end(ap);
}

void print_tree(syntax_tree_type * root, int k) {
	if(root == NULL) return;
	int i;
	for(i = 1; i <= k; ++ i) printf("  ");
	printf("%s", root->name);
	if(root->type == NONTERMINAL) printf(" (%d)", root->line_no); else 
	if(!strcmp(root->name, "ID") || !strcmp(root->name, "TYPE")) printf(": %s", root->str_val); else
	if(!strcmp(root->name, "INT")) printf(": %d", root->int_val); else 
	if(!strcmp(root->name, "FLOAT")) printf(": %lf", root->float_val);
	printf("\n");
	print_tree(root->child, k+1);
	print_tree(root->next, k);
}
