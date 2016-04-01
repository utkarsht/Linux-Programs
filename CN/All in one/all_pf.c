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
	int pftos = open("pftos", O_WRONLY);
	
	char buf[M];
	while(1)
	{
		scanf("%s", buf);
		write(pftos, buf, M);
	}
	return 0;
}
