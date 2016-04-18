#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#define SHM_SIZE 100
#define KEY 2015

int main()
{
	sem_t *s1, *s2;

	char p1[] = "px1";
	char p2[] = "px2";
	s1 = sem_open(p1, O_CREAT, 0666, 0);
	s2 = sem_open(p2, O_CREAT, 0666, 0);

	int shmid;
	char* shmptr;
	
	if((shmid = shmget(KEY, SHM_SIZE, IPC_CREAT|0666)) < 0)
		err("shmget error");	
	if((shmptr = shmat(shmid, 0, 0)) == (char *)-1)
		err("shmat error");
	char temp[10];

	while(1)
	{
		sem_wait(s2);
		strcpy(temp, shmptr);
		strcat(temp, "+");
		printf("Sending : %s\n", temp);
		strcpy(shmptr, temp);	
		sem_post(s1);	
		sleep(1);
	}

	return 0;
}
