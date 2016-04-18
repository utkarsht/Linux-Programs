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
#define C 3

int stoc[C];
struct pollfd ctos[C];
int tC = 0;

void err (char *str)
{
	perror(str);
	exit(1);
}

void* readandwrite()
{
	int i, sender;
	char buf[M];

	while(1)
	{
		int p = poll(ctos, C, 1000);
		sender = -1;
		for (i = 0; i < C; i++)
		{	
			if(ctos[i].revents & POLLIN)
			{	
				printf("%d\n", i);
				read(ctos[i], buf, M);
				sender = i;
				break;
			}
		}
		if (sender != -1)
		for (i = 0; i < C; i++)
		{
			if (sender != i)
			{
				write(stoc[i], buf, M);}
			}
		}
}

char* add(char* str, int i)
{
	char n[3];
	sprintf(n, "%d", i);
	strcat(str, n);
	return str;
}

int main(int argc, char** argv)
{
	int i;	
	for (i = 0; i < C; i++)
	{
		char sname[8] = "stoc";
		char cname[8] = "ctos";
		mkfifo(add(sname, i), 0666);
		mkfifo(add(cname, i), 0666);
	}

	for (i = 0; i < C; i++)
	{
		char sname[8] = "stoc";
		char cname[8] = "ctos";
		ctos[i].fd = open(add(cname, i), O_RDONLY);
		stoc[i] = open(add(sname, i), O_WRONLY);
		ctos[i].events = POLLIN;
	}
	printf("Started\n");

	readandwrite();
}
