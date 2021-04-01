#include <stdio.h>

extern int SumSquares(int, int*);

int main(int argc, char **argv)
{
	int n;
	if (argc < 2)
	{
		printf("Array size: ");
	        scanf("%d", &n);

	}
	else
	{
		n = argc-1;
	}
	int array[n];
	if (argc < 2)
	{
	printf("Enter %d numbers\n", n);

	int i = 0;
	while(i < n)
	{
		scanf("%d", &array[i]);
		i++;
	}
	}
	else
	{
		int i = 0;
		while (i < n)
		{
			sscanf(argv[i+1], "%d", &array[i]);
			i++;
		}
	}
	int result = SumSquares(n, array);
	printf("result: %d\n", result);
	return 0;
}
