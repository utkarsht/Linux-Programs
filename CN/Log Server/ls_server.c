#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#define M 128
#define MAX 10

int main()
{
	int tols = open("ls_tols", O_RDONLY);
	char buf[M];

	while(1)
	{
		read(tols, buf, M);
		printf("%s\n", buf);
	}

	return 0;
}
