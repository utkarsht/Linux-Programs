#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 256

int main(int argc, char* argv[])
{
	int a, b;
	char buffer[M];

	while (1)
	{
		fgets(buffer, M, stdin);
		sscanf(buffer, "%d %d", &a, &b);
		sprintf(buffer, "sum is %d\n", a + b);
		write(1, buffer, strlen(buffer));	
	}
}
