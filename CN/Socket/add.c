#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Not sufficient argument");
		exit(1);
	}

	int a = atoi(argv[1]);
	int b = atoi(argv[2]);

	printf("Sum is %d\n", a + b);	
}
