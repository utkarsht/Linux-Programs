#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#define M 128

int clientNum;
int ctos, stoc;

void* readT(void *arg)
{
	char buf[M];
	while(1)
	{
		read(stoc, buf, M);
		if(strlen(buf) > 1)
			printf("%s\n", buf);
		
	}
}

void* writeT(void *arg)
{
	char buf[M];
	while(1)
	{
		scanf("%s", buf);
		write(ctos, buf, M);
	}
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("Please provide client number.\n");
		exit(1);
	}
	clientNum = atoi(argv[1]);

	char sname[8] = "stoc";
	strcat(sname, argv[1]);	
	
	char cname[8] = "ctos";
	strcat(cname, argv[1]);	

	ctos = open(cname, O_WRONLY);
	stoc = open(sname, O_RDONLY);	
	
	printf("Started Client\n");

	pthread_t r, w;
	pthread_create(&r, NULL, &readT, NULL);
	pthread_create(&w, NULL, &writeT, NULL);

	pthread_join(r, NULL);
	pthread_join(w, NULL);

	return 0;
}
