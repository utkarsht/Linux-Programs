#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 256

int main(int argc, char* argv[])
{
	int a;
	char buffer[M];

	while (1)
	{
		fgets(buffer, M, stdin);
		sscanf(buffer, "%d", &a);
		sprintf(buffer, "Square is %d\n", a*a);	
		write(1, buffer, strlen(buffer));
	}
}
