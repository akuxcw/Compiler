struct Node
{

	int id;
	int p0;
	int p1;
};

int distance (struct Node n1, struct Node n2)
{

	int dis;
	dis = (n1.p0 - n2.p0) * (n1.p0 - n2.p0)
		+ (n1.p1 - n2.p1) * (n1.p1 - n2.p1);
	return dis;

}
int main()
{

	struct Node n[4];
	struct Node obj;
	int i = 0, j = 1, minDis, minId, cntDis;
	while (i < 4)
	{

		n[i].id = i + 1;
		n[i].p0 = i * 3;
		n[i].p1 = i * 3;
		i = i + 1;

	}
	obj.id = 5;
	obj.p0 = 4;
	obj.p1 = 5;
	minDis = distance(obj, n0);
	minId = 0;
	while(j < 4)
	{

		cntDis = distance(obj, n[j]);
		if (cntDis < minDis)
		{

			minDis = cntDis;
			minId = n[j].id;

		}
		j = j + 1;

	}
	write(minId);
	return 0;
}
