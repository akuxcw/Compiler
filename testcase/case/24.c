int gcd(int m, int n)
{
	if(n == 0)
		return m;
	else
		return gcd(n, (m - (m / n)*n));
}
int main()
{
	int i = read(), j = read(), k;
	k = gcd(i, j);
	write(k);
	return 0;
}

