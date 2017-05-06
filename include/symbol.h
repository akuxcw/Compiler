#ifndef COMPILER_SYMBOL_H_
#define COMPILER_SYMBOL_H_
#include "common.h"
#include "list.h"
#include "syntax_tree.h"

#define MAX_SYMBOL 0x3fff
#define MAX_LV 100
enum {INT_TYPE, FLOAT_TYPE, ARRAY_TYPE, STRUCT_TYPE, FUNC_TYPE, BASIC_TYPE};

typedef struct SymbolType SymbolType;
typedef struct Struct Struct;
typedef struct Struct Func;
typedef struct Array Array;
typedef struct SymbolTable SymbolTable;

struct Struct {
	char * name;
	SymbolType * type;
	ListHead list;
};

struct Array {
	int size;
	ListHead list;
};


struct SymbolType {
	char * name;
	int type;
	bool exp;
	bool fun;
	SymbolType * elm;
	int size;
//	ListHead array;
	ListHead structure;
	SymbolType * ret;
	ListHead func;
	ListHead list;
};

struct SymbolTable {
	char * name;
	SymbolType * type;
	int lv;
	ListHead lv_list;
	ListHead list;
};

void init_symbol();
bool isID(SyntaxTreeType *);
SymbolType * getType(char *);
void addSymbol(char *, SymbolType *, int, int);
void addType(SymbolType *, int);
bool neqType(SymbolType *, SymbolType *);
SymbolType * FindSymbol(char *);
SymbolType * FindStructFiled(SymbolType *, char *, int);
#endif // COMPILER_SYMBOL_H_
