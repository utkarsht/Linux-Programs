#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Insufficient argument");
		exit(1);
	}

	while(1)
	{
		printf("msg from %s\n", argv[1]);
		int r = rand() % 5 + 1;
		sleep(r);
	}
}
