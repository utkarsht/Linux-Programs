#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <poll.h>
#include <unistd.h>
#include <pthread.h>
#define GP 3
#define M 128
#define MX 10

void err(char *str)
{
	perror(str);
	exit(1);
}

struct pollfd pfd[GP][MX];
int k[GP];

void* readandsend(void *arg)
{
	int gp_num = *(int *)arg;
	int i, sender;
	char buff[M];

	while (1)
	{
		int K = k[gp_num];
		poll(pfd[gp_num], K, 1000);
		sender = -1;
		
		for (i = 0; i < K; i++)
		{
			if (pfd[gp_num][i].revents & POLLIN)
			{
				read(pfd[gp_num][i].fd, buff, M);
				sender = i;
				break;
			}
		}
		if (sender != -1)
		{
			for (i = 0; i < K; i++)
			{
				if (i != sender)
					write(pfd[gp_num][i].fd, buff, M);
			}
		}
	}
}

int main(int argc, char** argv)
{
	if (argc < 2)
		err("Usage : ./obj portno");

	struct sockaddr_in serv_addr, clnt_addr;
	int nsfd, clientlen, maxfd = 0;
	int i, sfd[GP], already[GP] = {0};
	fd_set master, test;
	FD_ZERO(&master);
	FD_ZERO(&test);	

	pthread_t t[GP];
	int port = atoi(argv[1]);

	for (i = 0; i < GP; i++)
	{
		already[i] = 0;
		sfd[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (sfd[i] < 0)
			err("Socket error");
	
		serv_addr.sin_family = AF_INET;		
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(port + i);
		
		if (bind(sfd[i], (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
			err("Bind error");	
		FD_SET(sfd[i], &master);

		listen(sfd[i], 3);

		if (maxfd < sfd[i])
			maxfd = sfd[i];
	}
	
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	while (1)
	{		
		test = master;
		select(maxfd + 1, &test, NULL, NULL, &tv);
		for (i = 0; i < GP; i++)
		{
			if (FD_ISSET(sfd[i], &test))
			{
				clientlen = sizeof(clnt_addr);
				if ((nsfd = accept(sfd[i], (struct sockaddr*) &clnt_addr, &clientlen)) < 0)
				{
					err("accept error");
				}				
				break;
			}
		}	
	
		if (i != GP)
		{
			if (already[i])
			{
				int K = k[i];
				pfd[i][K].fd = nsfd;
				pfd[i][K].events = POLLIN;
				k[i]++;
			}
			else
			{
				already[i] = 1;
				k[i] = 0;
				int K = k[i];
				pfd[i][K].fd = nsfd;
				pfd[i][K].events = POLLIN;
				k[i]++;

				int *x = (int *)malloc(sizeof(int));
				*x = i;

				pthread_create(&t[i], NULL, readandsend, (void *)x);
			}
		}
	}
}
