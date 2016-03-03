#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Not sufficient argument");
		exit(1);
	}

	int a = atoi(argv[1]);

	printf("Square is %d\n", a*a);	
}
