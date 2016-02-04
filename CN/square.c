#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#define M 100

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("Insufficient argument\n");
		exit(1);
	}

	char buf[M];
	int a = atoi(argv[1]);
	int pid = atoi(argv[2]);

	int stoc = open("mss_stoc", O_WRONLY);	
	sprintf(buf, "The square of %d is -> %d\n", a, a * a);
	write(stoc, buf, M);
	kill(pid, SIGUSR1);
	return 0;
}
