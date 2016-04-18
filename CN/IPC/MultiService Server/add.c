#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#define M 100

int main(int argc, char** argv)
{
	if(argc < 3)
	{
		printf("Insufficient argument\n");
		exit(1);
	}

	char buf[M];	
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	int pid = atoi(argv[3]);
	
	int stoc = open("mss_stoc", O_WRONLY);	
	sprintf(buf, "Sum %d and %d is -> %d\n", a, b, a + b);
	write(stoc, buf, M);
	kill(pid, SIGUSR1);
	return 0;
}
