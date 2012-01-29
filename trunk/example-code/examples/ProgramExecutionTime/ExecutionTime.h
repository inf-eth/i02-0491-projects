// Function Prototype.
int sum (int, int);

// Function Definition.
int sum (int a, int b)
{
	int z;
	z = a+b;
	return z;
}

void PerformanceTest ()
{

	const unsigned int X  = 100;
	const unsigned int Y  = 100;
	const unsigned int T  = 100;

	double A[X][Y];
	double B[X][Y];
	double C[X][Y];

	int i, j, t;
	for (t=0; t<T; t++)
	{
		for (i=0; i<X; i++)
		{
			for (j=0; j<Y; j++)
			{
				C[i][j] = A[i][j] * B[i][j];
			}
		}
	}
}
