#include <stdio.h>
#include <unistd.h>

int main()
{
	int c;
	c = fork();
	int N;

	if (c == 0)		//	child process
	{
		sleep(1);
		printf("%d", N*N);
	}
	else			//	parent process
	{
		scanf("%d", &N);
	} 
	return 0;
}
