#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#define M 100

int main()
{	
	char buf[M];
	int count = 3;
	while(count--)
	{
		printf("Sending from popen\n");
		sleep(5);
	}
	return 0;
}
