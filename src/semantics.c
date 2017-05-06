#include "semantics.h"

extern SymbolType * int_type, * float_type;

void serror(int err_no, int line_no, const char * str) {
	printf("Error type %d at Line %d: %s.\n", err_no, line_no, str);
}

void Program(SyntaxTreeType * node) {
	if(node == NULL) return;
	printf("Program %d\n", node->line_no);
	ExtDefList(node->child);
}

void ExtDefList(SyntaxTreeType * node) {
	if(node == NULL) return;
	printf("ExtDefList %d\n", node->line_no);
	ExtDef(node->child);
	ExtDefList(node->child->next);
}

void ExtDef(SyntaxTreeType * node) {
	if(node == NULL) return;
	printf("ExtDef %d\n", node->line_no);
	SymbolType * type = Specifier(node->child);
	addType(type);
	if(!strcmp(node->child->next->name, "ExtDecList")) {
		ExtDecList(node->child->next, type);
	}
	if(!strcmp(node->child->next->name, "FunDec")) {
		FunDec(node->child->next, type);
		CompSt(node->child->next->next);
	}
}

SymbolType * Specifier(SyntaxTreeType * node) {
	if(node == NULL) return NULL;
	printf("Specifier %d\n", node->line_no);
	if(!strcmp(node->child->name, "TYPE")) {
		if(!strcmp(node->child->str_val, "int")) {
			return int_type;
		} else {
			return float_type;
		}
	} else {
		return StructSpecifier(node->child);
	}
}

SymbolType * StructSpecifier(SyntaxTreeType * node) {
	if(node == NULL) return NULL;
	printf("StructSpecifier %d\n", node->line_no);
	if(!strcmp(node->child->next->name, "Tag")) {
		return getType(node->child->next->child->str_val);
	} else {
		SymbolType * type = newp(SymbolType);
		list_init(&type->array);
		list_init(&type->structure);
		SyntaxTreeType * opttag = node->child->next;
		if(opttag != NULL) {
			str_cpy(type->name, str_cat("struct ", opttag->child->str_val));
		}
		Struct * s = newp(Struct);
		SyntaxTreeType * deflist = node->child->next->next->next;
//return NULL;
		while(deflist != NULL) {
			s->type = Specifier(deflist->child->child);
			SymbolType * stype = s->type;	
			SyntaxTreeType * declist = deflist->child->child->next;
			while(declist != NULL) {
				SyntaxTreeType * vardec = declist->child->child;
				bool ferr = false;
				if(vardec->next != NULL) {
					serror(15, vardec->line_no, vardec->child->str_val);
					if(declist->child->next != NULL) declist = declist->child->next->next;
					else break;
					continue;
				}	
				while(strcmp(vardec->child->name, "ID")) {
					Array * arr = newp(Array);
					arr->size = vardec->child->next->next->int_val;
					s->type = newp(SymbolType);
					memcpy(s->type, stype, sizeof(SymbolType));
					list_add_after(&s->type->array, &arr->list);
					vardec = vardec->child;
				}
				ListHead * ptr;
				list_foreach(ptr, &type->structure) {
					Struct * tmps = list_entry(ptr, Struct, list);
					if(!strcmp(tmps->name, vardec->child->str_val)) {
						serror(15, vardec->line_no, tmps->name);
						ferr = true;
						break;
					}
				}
				if(!ferr) {
					str_cpy(s->name, vardec->child->str_val);
					vardec = declist->child->child;
					list_add_before(&type->structure, &s->list);
				}
				if(declist->child->next != NULL) {
					Struct * tmps = newp(Struct);
					tmps->type = stype;
					s = tmps;
					declist = declist->child->next->next;
				} else break;
			}
			if(deflist->child->next != NULL) {
				deflist = deflist->child->next;
			} else break;
		}
		return type;
	}
}

void ExtDecList(SyntaxTreeType * node, SymbolType * type) {
	if(node == NULL) return;
	printf("ExtDecList %d\n", node->line_no);
	while(node != NULL) {
		VarDec(node->child, type);
		if(node->child->next != NULL) {
			node = node->child->next->next;
		} else break;
	}
}

void VarDec(SyntaxTreeType * node, SymbolType * type) {
	if(node == NULL) return;
	printf("VarDec %d\n", node->line_no);
	SymbolType * exptype = NULL;
	if(node->next != NULL) {
//		exptype = Exp(node->next->next);
	}
	SymbolType * t = newp(SymbolType);
	memcpy(t, type, sizeof(SymbolType));
	while(node != NULL) {
		if(!strcmp(node->child->name, "ID")) {
			addSymbol(node->child->str_val, t, node->child->line_no);
			break;
		}
		Array * arr = newp(Array);
		list_add_after(&t->array, &arr->list);
		arr->size = node->child->next->next->int_val;
		node = node->child;
	}
	if(exptype != NULL) {
//		if(neqType(type, exptype)) {
			serror(5, node->line_no, node->child->str_val);
//		}
	}
}


void FunDec(SyntaxTreeType * node, SymbolType * type) {
	if(node == NULL) return;
	printf("FunDec %d\n", node->line_no);
	SymbolType * t = newp(SymbolType);
	memcpy(t, type, sizeof(SymbolType));
	t->type = FUNC_TYPE;
	list_init(&t->func);
	if(!strcmp(node->child->next->next->name, "VarList")) {
		SyntaxTreeType * varlist = node->child->next->next;
//return NULL;
		Func * f = newp(Func);
		while(varlist != NULL) {
			f->type = Specifier(varlist->child->child);
			SyntaxTreeType * vardec = varlist->child->child->next;
//			bool ferr = false;
			while(strcmp(vardec->child->name, "ID")) {
				Array * arr = newp(Array);
				arr->size = vardec->child->next->next->int_val;
				SymbolType * tmpt = f->type;
				f->type = newp(SymbolType);
				memcpy(f->type, tmpt, sizeof(SymbolType));
				list_add_after(&f->type->array, &arr->list);
				vardec = vardec->child;
			}
/*			ListHead * ptr;
			list_foreach(ptr, &t->func) {
					Struct * tmps = list_entry(ptr, Struct, list);
					if(!strcmp(tmps->name, vardec->child->str_val)) {
						serror(15, vardec->line_no, tmps->name);
						ferr = true;
						break;
					}
				}
				if(!ferr) {
*/
			str_cpy(f->name, vardec->child->str_val);
			list_add_before(&t->func, &f->list);
			if(varlist->child->next != NULL) {
				varlist = varlist->child->next->next;
			} else break;
		}

	}
	addSymbol(node->child->str_val, t, node->line_no);
}

void CompSt(SyntaxTreeType * node) {
	if(node == NULL) return;
	printf("CompSt %d\n", node->line_no);
	DefList(node->child->next);
//	StmtList(node->child->next->next);
}

void DefList(SyntaxTreeType * node) {
	if(node == NULL) return;
	printf("DefList %d\n", node->line_no);
	Def(node->child);
	DefList(node->child->next);
}

void Def(SyntaxTreeType * node) {
	if(node == NULL) return;
	printf("Def %d\n", node->line_no);
	SymbolType * type = Specifier(node->child);
	addType(type);
	DecList(node->child->next, type);
}

void DecList(SyntaxTreeType * node, SymbolType * type) {
	if(node == NULL) return;
	printf("DecList %d %s\n", node->line_no, node->name);
	while(node != NULL) {
		VarDec(node->child->child, type);
		if(node->child->next != NULL) {
			node = node->child->next->next;
		} else break;
	}
}


