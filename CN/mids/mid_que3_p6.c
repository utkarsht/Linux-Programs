#include <stdio.h>
#define M 128

int main()
{
	char buffer[M];
	while(1)
	{
		fgets(buffer, M, stdin);	
		printf("%s\n", buffer);
	}
}
