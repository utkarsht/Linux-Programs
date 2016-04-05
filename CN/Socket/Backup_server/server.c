#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <netinet/in.h>
#include "unix_sock.h"
#define M 256
#define MAX 2

int nsfd[MAX];

void* service(void* arg)
{
	int id = *(int *)arg;
	char buf[M];
	int a, b;

	while (1)
	{
		if (nsfd[id] != -1)
		{
			read(nsfd[id], buf, M);
			sscanf(buf, "%d %d", &a, &b);
			sprintf(buf, "Server, thread %d : Sum is %d\n", id, a + b);
			write(nsfd[id], buf, M);
		}
		else
		{
			sleep(1);
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2)
		err("usage : ./obj portno");

	char path[] = "/tmp/sockpath";
	unlink(path);
	int usfd = init_sockbind(path);
	int nusfd = unix_accept(usfd);

	write(nusfd, "Hello", 6);

	if (nusfd < 0)
		err("unix sfd bind ");	
	
	char buffer[M];
		
	int port = atoi(argv[1]);
	int sfd = tcpsocket_bind(port);
	int i, clntcnt = 0;

	while (1)
	{
		int tempfd = tcp_accept(sfd);
		printf("Server connected to client on %d\n", tempfd);
		if (clntcnt == MAX)
		{
			for (i = 0; i < MAX; i++)
			{
				printf("Sending fd %d via %d", nsfd[i], nusfd);
				if(send_fd(nusfd, nsfd[i]) < 0);
					perror("sending fd error ");			
			}
			
			clntcnt = 0;
			for (i = 0; i < MAX; i++)
				nsfd[i] = -1;	
			nsfd[0] = tempfd;
		}
		else
		{
			nsfd[clntcnt] = tempfd;
			printf("Creating new thread \n");
			pthread_t newThread;
			int *id = (int *)malloc(sizeof(int));
			*id = clntcnt;
			pthread_create(&newThread, NULL, service, (void *)id);

			clntcnt++;
		}
	}
}
