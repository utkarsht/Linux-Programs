#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <poll.h>
#include <signal.h>
#include <netinet/in.h>
#include <errno.h>
#include "unix_sock.h"
#define M 256
#define MAX 2

int nsfd[MAX];
struct pollfd pfd[MAX];
pthread_mutex_t mtx;
int clntcnt = 0;

void* service(void* arg)
{
	char buf[M];
	int a, b, i;

	while (1)
	{
		pthread_mutex_lock(&mtx);
		poll(pfd, clntcnt, 1000);
		
		for (i = 0; i < clntcnt; i++)
		{
			if (pfd[i].revents & POLLIN)
			{
				read(pfd[i].fd, buf, M);
				sscanf(buf, "%d %d", &a, &b);
				sprintf(buf, "Server, Sum is %d\n", a + b);
				write(pfd[i].fd, buf, M);
			}
		}
		pthread_mutex_unlock(&mtx);
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
	int i, created = 0;

	pthread_mutex_init(&mtx, NULL);
	pthread_t thread;
	pthread_create(&thread, NULL, service, NULL);

	while (1)
	{
		int tempfd = tcp_accept(sfd);
		printf("Server connected to client on %d\n", tempfd);
		if (clntcnt == MAX)
		{
			pthread_mutex_lock(&mtx);
			for (i = 0; i < MAX; i++)
			{
				printf("Sending fd %d via %d\n", pfd[i].fd, nusfd);
				if(send_fd(nusfd, pfd[i].fd) < 0)
					printf("sending fd error %d\n", errno);
				
				close(pfd[i].fd);
			}
						
			clntcnt = 0;			
			pfd[clntcnt].fd = tempfd;
			pfd[clntcnt].events = POLLIN;

			clntcnt++;
			pthread_mutex_unlock(&mtx);
		}
		else
		{			
			pthread_mutex_lock(&mtx);
			pfd[clntcnt].fd = tempfd;
			pfd[clntcnt].events = POLLIN;
			clntcnt++;
			pthread_mutex_unlock(&mtx);
		}
	}
}
