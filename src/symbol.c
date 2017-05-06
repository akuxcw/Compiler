#include "symbol.h"
#include "semantics.h"
ListHead type_table;

ListHead symbol_table[MAX_SYMBOL];
ListHead lv_table[MAX_LV];
SymbolType * int_type, * float_type;

static unsigned int hash(char * str) {
	unsigned int val = 0, i;
	for(; *str; ++ str) {
		val = (val << 2) + *str;
		if(i = val & ~MAX_SYMBOL) val = (val ^ (i >> 12)) & MAX_SYMBOL;
	}
	return val;
}

void init_symbol() {
	int i;
	list_init(&type_table);
	for(i = 0; i < MAX_SYMBOL; ++ i) list_init(&symbol_table[i]);
	for(i = 0; i < MAX_LV; ++ i) list_init(&lv_table[i]);

	int_type = newp(SymbolType);
	str_cpy(int_type->name, "int");
	int_type->type = INT_TYPE;

	float_type = newp(SymbolType);
	str_cpy(float_type->name, "float");
	float_type->type = FLOAT_TYPE;
//	list_add_after(&type_table, &float_type->list);
//	list_add_after(&type_table, &int_type->list);
	SymbolType * int_type = newp(SymbolType);
}

bool isID(SyntaxTreeType * node) {
	if(!strcmp(node->name, "ID") && node->next == NULL) return true;
	if(node->next == NULL) return false;
	if(!strcmp(node->next->name, "DOT") || !strcmp(node->next->name, "LB")) return true;
	return false;
}

void addType(SymbolType * ntype, int line_no) {
	printf("addtype\n");
	SymbolType * type = getType(ntype->name);
	if(type != NULL) {
		serror(16, line_no, "redef");
		return;
	}
	list_add_before(&type_table, &ntype->list);
}

SymbolType * getType(char * name) {
	printf("gettype %s\n", name);
	ListHead * ptr;
	list_foreach(ptr, &type_table) {
		SymbolType * tmp = list_entry(ptr, SymbolType, list);
		if(!strcmp(tmp->name, name)) return tmp;
	}
	return NULL;
}

void addSymbol(char * name, SymbolType * type, int line_no, int lv) {
	printf("add %s\n", name);
	unsigned int h = hash(name);
	while(!list_empty(&symbol_table[h])) {
		SymbolTable * tmp = list_entry(symbol_table[h].next, SymbolTable, list);
		if(!strcmp(tmp->name, name)){
			if(tmp->lv == lv) {
				if(tmp->type->fun) serror(4, line_no, name);
				else serror(3, line_no, name);
			} else break;
		};
		return;
		h ++;
	}
	SymbolTable * tmp = newp(SymbolTable);
	tmp->lv = lv;
	tmp->type = type;
	str_cpy(tmp->name, name);
	list_add_after(&symbol_table[h], &tmp->list);
	list_add_after(&lv_table[lv], &tmp->lv_list);
}

bool neqType(SymbolType * t1, SymbolType * t2) {
	if(t1 == NULL || t2 == NULL) return true;
	if(t1->type != t2->type) return true;
	if(t1->type == ARRAY_TYPE) {
		return neqType(t1->elm, t2->elm);
	}
	if(t1->name != NULL && t2->name != NULL && !strcmp(t1->name, t2->name))return false;
	ListHead * ptr1, * ptr2 = t2->structure.next;
	list_foreach(ptr1, &t1->structure) {
		if(ptr2 == &t2->structure) return true;
		SymbolType * tmp1 = list_entry(ptr1, Struct, list)->type;
		SymbolType * tmp2 = list_entry(ptr2, Struct, list)->type;
		if(neqType(tmp1, tmp2))return true;
		ptr2 = ptr2->next;
	}
	if(ptr2 != &t2->structure) return true;
	return false;
}

SymbolType * FindSymbol(char * name) {
	printf("find %s\n", name);
	unsigned int h = hash(name);
	while(!list_empty(&symbol_table[h])) {
		SymbolTable * tmp = list_entry(symbol_table[h].next, SymbolTable, list);
		if(!strcmp(tmp->name, name)) {
			return tmp->type;
		}
		h ++;
	}
	return NULL;
}

SymbolType * FindStructFiled(SymbolType * type, char * name, int line_no) {
	printf("findfiled\n");
	if(type->type != STRUCT_TYPE) {
		serror(13, line_no, "dot");
		return NULL;
	}
	ListHead * ptr;
	list_foreach(ptr, &type->structure) {
		Struct * tmp = list_entry(ptr, Struct, list);
		if(!strcmp(tmp->name, name)) return tmp->type;
	}
	serror(14, line_no, "nodef");
	return NULL;
}

void delLv(int lv) {
	while(!list_empty(&lv_table[lv])) {
		SymbolTable * tmp = list_entry(lv_table[lv].next, SymbolTable, lv_list);
		list_del(&tmp->list);
		list_del(&tmp->lv_list);
	}
}
