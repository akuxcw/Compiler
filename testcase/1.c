#include <stdio.h>

struct Operands
{
	int o1;
	int o2;
};

int add(struct Operands temp)
{
	temp.o1 = 10;
	return (temp.o1 + temp.o2);
}

int main()
{
	int n;
	struct Operands op;
	op.o1 = 1;
	op.o2 = 2;
	n = add(op);
	printf("%d\n", op.o1);
	return 0;
}
