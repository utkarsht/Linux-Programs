#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#define M 100

int main()
{
	int stop5 = open("stop5", O_RDONLY);
	
	char buf[M];
	while(1)
	{
		read(stop5, buf, M);
		printf("%s\n", buf);
	}
	return 0;
}
