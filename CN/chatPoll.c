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
char buf[M];
int ctos, stoc;
struct pollfd clfile;

void err (char *str)
{
	perror(str);
	exit(1);
}

void* readT(void *arg)
{
	while(1)
	{
//		wait for server to write
//		printf("In chatpoll read \n");
		
		read(stoc, buf, M);
		if(strlen(buf) > 1)
			printf("%s\n", buf);
//		write(1, buf, M);
/*		poll(&clfile, 1, 1000);
		if (clfile.revents & POLLIN) 
		{		
			read(stoc, buf, 5);
			write(1, buf, 5);
		}*/
	}
}

void* writeT(void *arg)
{
	while(1)
	{
//		printf("In chatpoll Write \n");
		read(0, buf, M);
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

	char fname[8] = "stoc";
	strcat(fname, argv[1]);	
	
	if(mknod(fname, S_IFIFO | 0666, 0) == -1)
	;//	err("mknod err");
		
	ctos = open("ctos", O_WRONLY|O_NONBLOCK);
	stoc = open(fname, O_RDONLY|O_NONBLOCK);	
	
	clfile.fd = stoc;
	clfile.events = POLLIN;

	pthread_t r, w;
	pthread_create(&r, NULL, &readT, NULL);
	pthread_create(&w, NULL, &writeT, NULL);

	pthread_join(r, NULL);
	pthread_join(w, NULL);

	return 0;
}
