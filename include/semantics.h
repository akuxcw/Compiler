#ifndef _COMPLIER_SEMANTICS_H_
#define _COMPILER_SEMANTICS_H_

#include "syntax_tree.h"
#include "symbol.h"

void serror(int, int, const char *);
void Program(SyntaxTreeType *);
void ExtDefList(SyntaxTreeType *);
void ExtDef(SyntaxTreeType *);
SymbolType * Specifier(SyntaxTreeType *);
SymbolType * StructSpecifier(SyntaxTreeType *);
void ExtDecList(SyntaxTreeType *, SymbolType *);
void VarDec(SyntaxTreeType *, SymbolType *);
void FunDec(SyntaxTreeType *, SymbolType *);
void CompSt(SyntaxTreeType *);
void DefList(SyntaxTreeType *);
void Def(SyntaxTreeType *);
void DecList(SyntaxTreeType *, SymbolType *);

#endif // _COMPILER_SEMANTICS_H_
