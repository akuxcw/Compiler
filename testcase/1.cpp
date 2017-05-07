int a;
struct a{
	int s;
} b;

int fun() {
	float a = 1.1;
	{
		int a = 0;
	}
	a = 1.1;
	return 0;
}

int main() {
	int a = 1;
	int fun = 1;
}
