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
		CompSt(node->child->next->next, type);
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
		type->type = STRUCT_TYPE;
		list_init(&type->structure);
		SyntaxTreeType * opttag = node->child->next;
		if(opttag != NULL) {
			str_cpy(type->name, str_cat("struct ", opttag->child->str_val));
		}
		SyntaxTreeType * deflist = node->child->next->next->next;
//return NULL;
		while(deflist != NULL) {
			Struct * s = newp(Struct);
			s->type = Specifier(deflist->child->child);
			SymbolType * stype = newp(SymbolType);
			memcpy(stype, s->type, sizeof(SymbolType));	
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
					SymbolType * arr = newp(SymbolType);
					arr->size = vardec->child->next->next->int_val;
					arr->elm = s->type;
					arr->type = ARRAY_TYPE;
					s->type = arr;
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
					s = newp(Struct);
					s->type = stype;
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
		exptype = Exp(node->next->next);
	}
	SymbolType * t = type;

	while(node != NULL) {
		if(!strcmp(node->child->name, "ID")) {
			addSymbol(node->child->str_val, t, node->child->line_no);
			break;
		}
		SymbolType * arr = newp(SymbolType);
		arr->size = node->child->next->next->int_val;
		arr->elm = t;
		arr->type = ARRAY_TYPE;
		t = arr;

		node = node->child;
	}
	if(exptype != NULL) {
		if(neqType(type, exptype)) {
			serror(5, node->line_no, node->child->str_val);
		}
	}
}


void FunDec(SyntaxTreeType * node, SymbolType * type) {
	if(node == NULL) return;
	printf("FunDec %d\n", node->line_no);
	SymbolType * t = newp(SymbolType);
	memcpy(t, type, sizeof(SymbolType));
	t->fun = true;
	list_init(&t->func);
	if(!strcmp(node->child->next->next->name, "VarList")) {
		SyntaxTreeType * varlist = node->child->next->next;
//return NULL;
		Func * f = newp(Func);
		while(varlist != NULL) {
			f->type = Specifier(varlist->child->child);
			SyntaxTreeType * vardec = varlist->child->child->next;
			VarDec(vardec, f->type);
//			bool ferr = false;
			while(strcmp(vardec->child->name, "ID")) {
				SymbolType * arr = newp(SymbolType);
				arr->size = vardec->child->next->next->int_val;
				arr->elm = f->type;
				f->type = arr;

				vardec = vardec->child;
			}
			str_cpy(f->name, vardec->child->str_val);
			list_add_before(&t->func, &f->list);
			if(varlist->child->next != NULL) {
				varlist = varlist->child->next->next;
			} else break;
		}

	}
	addSymbol(node->child->str_val, t, node->line_no);
}

void CompSt(SyntaxTreeType * node, SymbolType * return_type) {
	if(node == NULL) return;
	printf("CompSt %d\n", node->line_no);
	if(!strcmp(node->child->next->name, "DefList")) {
		DefList(node->child->next);
		if(!strcmp(node->child->next->next->name, "StmtList")) 
			StmtList(node->child->next->next, return_type);
	} if(!strcmp(node->child->next->name, "StmtList")) 
		StmtList(node->child->next, return_type);
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

void StmtList(SyntaxTreeType * node, SymbolType * return_type) {
	if(node == NULL) return;
	printf("StmtList %d\n", node->line_no);
	Stmt(node->child, return_type);
	StmtList(node->child->next, return_type);
}

void Stmt(SyntaxTreeType * node, SymbolType * return_type) {
	if(node == NULL) return;
	printf("Stmt %d %s\n", node->line_no, node->name);
	if(!strcmp(node->child->name, "Exp")) {
		Exp(node->child);
	} else if(!strcmp(node->child->name, "CompSt")) {
		CompSt(node->child, return_type);
	} else if(!strcmp(node->child->name, "RETURN")) {
		SymbolType * type = Exp(node->child->next);
		if(neqType(return_type, type)) {
			serror(8, node->line_no, "");
		}
	} else if(!strcmp(node->child->name, "IF") || !strcmp(node->child->name, "WHILE")) {
		SymbolType * type = Exp(node->child->next->next);
		if(type == NULL || type->type != INT_TYPE) {
			serror(7, node->line_no, "if");
		}
		SyntaxTreeType * tnode = node->child->next->next->next->next;
		Stmt(tnode, return_type);
		if(tnode->next != NULL) {
			Stmt(tnode->next->next, return_type);
		}
	}
}

SymbolType * Exp(SyntaxTreeType * node) {
	if(node == NULL) return NULL;
	printf("Exp %d %s\n", node->line_no, node->name);
	if(!strcmp(node->child->name, "Exp")) {
		SymbolType * type1 = Exp(node->child), * type2 = NULL;
		if(!strcmp(node->child->next->next->name, "Exp")) {
			type2 = Exp(node->child->next->next);
			if(type2 == NULL) return NULL;
		}
		if(type1 == NULL) return NULL;
		char * s = node->child->next->name;
		if(!strcmp(s, "ASSIGNOP")) {
			if(neqType(type1, type2) || type1->exp) {
				printf("%d\n", type1->exp);
				serror(5, node->line_no, "exp");
				return NULL;
			}
			return type2;
		} else if(!strcmp(s, "AND") || !strcmp(s, "OR") || !strcmp(s, "RELOP")) {
			if(type1->type != INT_TYPE || type2->type != INT_TYPE) {
				serror(7, node->line_no, "relop");
			}
			type1->exp = true;
			return type1;
		} else if(!strcmp(s, "PLUS") || !strcmp(s, "MINUS") || !strcmp(s, "STAR") || !strcmp(s, "DIV")) {
			if((type1->type != INT_TYPE && type1->type != FLOAT_TYPE) || (type2->type != INT_TYPE && type2->type != FLOAT_TYPE) || type1->type != type2->type) {
				serror(7, node->line_no, "plus");
				return NULL;
			}
			type1->exp = true;
			return type1;
		} else if(!strcmp(s, "LB")) {
			if(type1->type != ARRAY_TYPE) {
				serror(10, node->line_no, "array");
				return NULL;
			}
			if(type2->type != INT_TYPE) {
				serror(12, node->line_no, "a");
				return NULL;
			}
			type1 = type1->elm;
			return type1;
		} else if(!strcmp(s, "DOT")){
			return FindStructFiled(type1, node->child->next->next->str_val, node->child->next->next->line_no);
		}
	} else if(!strcmp(node->child->name, "MINUS") || !strcmp(node->child->name, "LP")) {
		return Exp(node->child->next);
	} else if(!strcmp(node->child->name, "NOT")) {
		SymbolType * type = Exp(node->child->next);
		if(type->type != INT_TYPE) {
			serror(7, node->line_no, "not");
			return NULL;
		}
		type->exp = true;
		return type;
	} else if(!strcmp(node->child->name, "ID")) {
		SymbolType * type = FindSymbol(node->child->str_val);
		if(node->child->next == NULL) {
			return type;
		} else {
//			Args(node->child->next->next, type);
		}
	} else if(!strcmp(node->child->name, "INT")) {
		SymbolType * type = newp(SymbolType);
		memcpy(type, int_type, sizeof(SymbolType));
		return type;
	} else if(!strcmp(node->child->name, "FLOAT")) {
		SymbolType * type = newp(SymbolType);
		memcpy(type, float_type, sizeof(SymbolType));
		return type;
	}

}




