#include "asmcode.h"
extern ListHead intercodes;

int cur_add = 0;
int address[MAX_SYMBOL];

void TranslateAsm() {
	ListHead * ptr;
	Operand * op[4];
	list_foreach(ptr, &intercodes) {
		InterCode * code = list_entry(ptr, InterCode, list);
		int i = 0;
		ListHead * ptr_;
		list_foreach(ptr_, &code->op_list) {
			op[i ++]= list_entry(ptr_, Operand, list);
		}
		char *r1, *r2, *r3, *opt;
		switch(code->type) {
			case(LABEL_CODE):
//				printf("LABEL %s :\n", op[0]->str_val);
				printf("%s:\n", op[0]->str_val);
				break;
			case(FUNCTION_CODE) :
//				printf("FUNCTION %s :\n", op[0]->str_val);
				printf("%s:\n", op[0]->str_val);
				printf("addi $sp, $sp, -4\n");
				printf("sw $fp, 0($sp)\n");
				printf("move $fp, $sp\n");
				break;
			case(ASSIGN_CODE) :
//				printf("%s := %s\n", op[0]->str_val, op[1]->str_val);
				r1 = GetReg(op[0]->str_val);
				r2 = GetReg(op[1]->str_val);
				printf("move %s, %s\n", r1, r2);
				PutInMemory(r1, op[0]->str_val);
				break;
			case(PLUS_CODE) :
//				printf("%s := %s + %s\n", op[0]->str_val, op[1]->str_val, op[2]->str_val);
				r1 = GetReg(op[0]->str_val);
				r2 = GetReg(op[1]->str_val);
				r3 = GetReg(op[2]->str_val);
				printf("add %s, %s, %s\n", r1, r2, r3);
				PutInMemory(r1, op[0]->str_val);
				break;
			case(MINUS_CODE) :
//				printf("%s := %s - %s\n", op[0]->str_val, op[1]->str_val, op[2]->str_val);
				r1 = GetReg(op[0]->str_val);
				r2 = GetReg(op[1]->str_val);
				r3 = GetReg(op[2]->str_val);
				printf("sub %s, %s, %s\n", r1, r2, r3);
				PutInMemory(r1, op[0]->str_val);
				break;
			case(STAR_CODE) :
//				printf("%s := %s * %s\n", op[0]->str_val, op[1]->str_val, op[2]->str_val);
				r1 = GetReg(op[0]->str_val);
				r2 = GetReg(op[1]->str_val);
				r3 = GetReg(op[2]->str_val);
				printf("mul %s, %s, %s\n", r1, r2, r3);
				PutInMemory(r1, op[0]->str_val);
				break;
			case(DIV_CODE) :
//				printf("%s := %s / %s\n", op[0]->str_val, op[1]->str_val, op[2]->str_val);
				r1 = GetReg(op[0]->str_val);
				r2 = GetReg(op[1]->str_val);
				r3 = GetReg(op[2]->str_val);
				printf("div %s, %s\n", r2, r3);
				printf("mflo %s\n", r1);
				PutInMemory(r1, op[0]->str_val);
				break;
			case(ZEROMINUS_CODE):
//				printf("%s := #0 - %s\n", op[0]->str_val, op[1]->str_val);
				r1 = GetReg(op[0]->str_val);
				r2 = GetReg(op[1]->str_val);
				printf("sub %s, $0, %s\n", r1, r2);
				PutInMemory(r1, op[0]->str_val);
				break;
			case(GOTO_CODE):
//				printf("GOTO %s\n", op[0]->str_val);
				printf("j %s\n", op[0]->str_val);
				break;
			case(IF_CODE):
//				printf("IF %s %s %s GOTO %s\n", op[0]->str_val, op[1]->str_val, op[2]->str_val, op[3]->str_val);
				r1 = GetReg(op[0]->str_val);
				r2 = GetReg(op[2]->str_val);
				opt = op[1]->str_val;
				if(strlen(opt) == 1) {
					if(opt[0] == '>') printf("bgt ");
					else printf("blt ");
				} else {
					if(opt[0] == '=') printf("beq ");
					else if(opt[0] == '!') printf("bne ");
					else if(opt[0] == '>') printf("bge ");
					else printf("ble ");
				}
				printf("%s, %s, %s\n", r1, r2, op[3]->str_val);
				break;
			case(RETURN_CODE):
				printf("RETURN %s\n", op[0]->str_val);
				break;
			case(DEC_CODE):
				printf("DEC %s %s\n", op[0]->str_val, op[1]->str_val);
				break;
			case(ARG_CODE):
				printf("ARG %s\n", op[0]->str_val);
				break;
			case(CALL_CODE):
				printf("%s := CALL %s\n", op[0]->str_val, op[1]->str_val);
				cur_add = 0;
				break;
			case(PARAM_CODE):
				printf("PARAM %s\n", op[0]->str_val);
				break;
			case(READ_CODE):
//				printf("READ %s\n", op[0]->str_val);
				r1 = GetReg(op[0]->str_val);
				printf("add $sp, $sp, -4\nsw $ra, 0($sp)\njal read\nlw $ra, 0($sp)\naddi $sp, $sp, 4\nmove %s, $v0\n"
					, r1);
				PutInMemory(r1, op[0]->str_val);
				break;
			case(WRITE_CODE):
//				printf("WRITE %s\n", op[0]->str_val);
				r1 = GetReg(op[0]->str_val);
				printf("move $a0, %s\naddi $sp, $sp, -4\nsw $ra, 0($sp)\njal write\nlw $ra, 0($sp)\naddi $sp, $sp, 4\n"
					, r1);
				break;
		}
	}
}

int cur_reg = 0;
char *GetReg(char *id) {
	char * s = malloc(32);
	sprintf(s, "$t%d", cur_reg);
	cur_reg = (cur_reg + 1) % 10;
}

void PutInMemory(char * reg, char * id){
	printf("sw %s, -%d($fp)", reg, GetAddress(id));
};

int GetAddress(char * id) {
	if(address[hash(id)] != 0) return address[hash(id)];
	cur_add += 4;
	address[hash(id)] = cur_add;
	return cur_add;
}
