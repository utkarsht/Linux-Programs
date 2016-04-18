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
#define MAX 10

int ctos, stoc[MAX];
struct pollfd serfile;
int tC = 0;
char buf[M];

void err (char *str)
{
	perror(str);
	exit(1);
}

void* readandsend(void* arg)
{
	int i;
	while(1)
	{
//		wait for client to write
//		printf("Waiting Here\n");
		read(ctos, buf, M);
		for(i = 0; i < tC; i++)				
			write(stoc[i], buf, M);
		
		/*poll(&serfile, 1, 1000);
		if(serfile.revents & POLLIN)
		{
			read(ctos, buf, M);
			for(i = 0; i < tC; i++)				
				write(stoc[i], buf, M);
		}*/
	}
}

void* addConnection(void* arg)
{
	char name[MAX];
	char fname[MAX] = "stoc";
	while(1)
	{
		printf("Enter client number to add :");
		scanf("%s", name);
		printf("\n%d\n", tC);
		strcat(fname, name);
		stoc[tC] = open(fname, O_WRONLY|O_NONBLOCK);	
		write(stoc[tC], "Hello", M);
		tC++;	
	}
}

int main(int argc, char** argv)
{
	if(mknod("ctos", S_IFIFO | 0666, 0) == -1)
	;//	err("ctos err");
	ctos = open("ctos", O_RDONLY|O_NONBLOCK);

	serfile.fd = ctos;
	serfile.revents = POLLIN;

	pthread_t r, w;
	pthread_create(&r, NULL, &readandsend, NULL);
	pthread_create(&w, NULL, &addConnection, NULL);

	pthread_join(r, NULL);
	pthread_join(w, NULL);
}
