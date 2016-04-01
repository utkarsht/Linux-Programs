#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#define KEY 2015
#define SHM_SIZE 100

void err(char *s)
{
	perror(s);
	exit(1);
}

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

	char num[5] = "1";
	while(1)
	{	
		strcpy(shmptr, num);
		printf("%s\n", shmptr);
		sem_post(s2);
		sem_wait(s1);
		strcpy(num, shmptr);
		printf("Receive : %s\n", num);
		sleep(1);
	}	

	if(shmdt(shmptr) == -1)
		err("shmdt error");
	if((shmctl(shmid, IPC_RMID, NULL)) == -1)
		err("shmctl error");

	return 0;
}
