#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#define KEY 1234
#define SHM_SIZE 100

void err (char *str)
{
	perror(str);
	exit(1);
}

sem_t* sem;
char* shmptr;
char my[SHM_SIZE];

void* read(void *arg)
{
	while(1)
	{
		sem_wait(sem);
		printf("%s", shmptr);
	}
}

void* write(void *arg)
{
	char msg[SHM_SIZE];
	char clienName[10];
	int i;

	while(1)
	{
		fgets(msg, SHM_SIZE, stdin);
		for (i = 0; msg[i] != ':'; i++)
			clienName[i] = msg[i];

		clienName[i + 1] != '\0';
				
		strcpy(msg, msg + i + 1);
		strcat(my, msg);
		strcpy(shmptr, my);
		sem_t *t = sem_open(clienName, O_CREAT);
		sem_post(t);
	}
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("Please provide client number.\n");
		exit(1);
	}
	printf("To send client X write X:\n");

	int shmid;
	strcpy(my, argv[1]);
	strcat(my, " -> ");
	if((shmid = shmget(KEY, SHM_SIZE, IPC_CREAT | 0666)) == -1)
		err("shmget");

	if((shmptr = shmat(shmid, 0, 0)) == (char*)-1)			
		err("shmat");

	sem = sem_open(argv[1], O_CREAT, 0666, 0);
	pthread_t r, w;
	
	pthread_create(&r, NULL, &read, NULL);
	pthread_create(&w, NULL, &write, NULL);

	pthread_join(r, NULL);
	pthread_join(w, NULL);

	return 0;
}
