#ifndef COMPILER_INTERCODE_H_
#define COMPILER_INTERCODE_H_

#include "common.h"
#include "symbol.h"

enum {PARAM_CODE, FUNCTION_CODE, ASSIGN_CODE, PLUS_CODE, MINUS_CODE, STAR_CODE, DIV_CODE,
	  RETURN_CODE, LABEL_CODE, GOTO_CODE, READ_CODE, WRITE_CODE, ZEROMINUS_CODE, IF_CODE,
	  ARG_CODE, CALL_CODE};
enum {ID_OP, FUNCTION_OP, INT_OP};

typedef struct Operand Operand;
typedef struct InterCode InterCode;

struct Operand {
	char * str_val;
	int type;
	ListHead list;
};

struct InterCode {
	int type;
	ListHead list;
	ListHead op_list;
};
char * new_var();
char * new_label();
Operand * new_op(char *);
void new_code(int, int, ...);
char * neg_op(char *);
void translate_Program(SyntaxTreeType *);
void translate_ExtDefList(SyntaxTreeType *);
void translate_ExtDef(SyntaxTreeType *);
void translate_FunDec(SyntaxTreeType *);
void translate_VarList(SyntaxTreeType *);
void translate_ParamDec(SyntaxTreeType *);
void translate_CompSt(SyntaxTreeType *);
void translate_DefList(SyntaxTreeType *);
void translate_StmtList(SyntaxTreeType *);
void translate_Stmt(SyntaxTreeType *);
void translate_Args(SyntaxTreeType *);
Operand * translate_Exp(SyntaxTreeType *);
void translate_Cond(SyntaxTreeType *, Operand *, Operand *);

#endif // COMPILER_INTERCODE_H_
