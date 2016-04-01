#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <pthread.h>
#define M 100
#define KEY 1111

int main()
{	
	int shmid = shmget(KEY, M, IPC_CREAT | 0666);
	char* shmptr = shmat(shmid, 0, 0);

	while(1)
	{
		scanf("%s", shmptr);
		system("pkill -SIGUSR1 o1");
	}
	return 0;
}
