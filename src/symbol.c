#include "symbol.h"
#include "semantics.h"
ListHead type_table;

SymbolTable * symbol_table[MAX_SYMBOL];
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
	list_init(&type_table);
	
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

void addType(SymbolType * ntype) {
	SymbolType * type = getType(ntype->name);
	if(type != NULL) {
//		serror(16)
		return;
	}
	list_add_before(&type_table, &ntype->list);
}

SymbolType * getType(char * name) {
	printf("gettype\n");
	ListHead * ptr;
	list_foreach(ptr, &type_table) {
		SymbolType * tmp = list_entry(ptr, SymbolType, list);
		if(!strcmp(tmp->name, name)) return tmp;
	}
	return NULL;
}

void addSymbol(char * name, SymbolType * type, int line_no) {
	printf("add %s\n", name);
	unsigned int h = hash(name);
	while(symbol_table[h] != NULL) {
		if(!strcmp(symbol_table[h]->name, name)){
			serror(3, line_no, name);
		};
		return;
		h ++;
	}
	symbol_table[h] = newp(SymbolTable);
	symbol_table[h]->type = type;
	str_cpy(symbol_table[h]->name, name);
}

bool neqType(SymbolType * t1, SymbolType * t2) {
	if(t1 == NULL || t2 == NULL) return true;
	if(t1->type != t2->type) return true;
	if(t1->type == ARRAY_TYPE) {
		return neqType(t1->elm, t2->elm);
	}
	return strcmp(t1->name, t2->name);
}

SymbolType * FindSymbol(char * name) {
	printf("find %s\n", name);
	unsigned int h = hash(name);
	while(symbol_table[h] != NULL) {
		if(!strcmp(symbol_table[h]->name, name)) {
			SymbolType * type = newp(SymbolType);
			memcpy(type, symbol_table[h]->type, sizeof(SymbolType));
			return type;
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
