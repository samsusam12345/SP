#include <stdio.h>
int main(void)
{
	int array[12] = {35, 12, 64, 23, 101, 68, 89, 53, 27, 95, 3, 16};
	int count = 0;
	int mask = 17;
	for (int i = 0; i < 12; i++)
	{
		array[i] = array[i] ^ mask;
		array[i] = array[i] & mask;
		if (array[i] == mask)
			count++;
	}
	printf("%i\n", count);
	return 0;
}
