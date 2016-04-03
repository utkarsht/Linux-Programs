#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#define M 256

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void* solve(void* arg)
{
	int fd = *(int *)arg;
	int a, b;
	char buffer[M];

	while (1)
	{
		read(fd, buffer, M);
		if (strncmp(buffer, "exit", 4) == 0)
			break;

		sscanf(buffer, "%d %d", &a, &b);
		sprintf(buffer, "Thread %d : Sum is %d\n", fd, a + b);
		write(fd, buffer, strlen(buffer));	
	}

	write(1, "Service exited", 15);
}

int main(int argc, char* argv[])
{
	if (argc < 2)
		err("usage : ./obj portno");	

	int sfd, newsfd, clntlen;
	int portno = atoi(argv[1]);
	printf("opened add service on port %d\n", portno);

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0) 
		error("ERROR opening socket");

	struct sockaddr_in serv_addr, clnt_addr;
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portno);

	if (bind(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");

	listen(sfd, 5);
	
	while(1)
	{
		clntlen = sizeof(clnt_addr);
		newsfd = accept(sfd, (struct sockaddr *) &clnt_addr, &clntlen);
		if (newsfd < 0)
		{
			error("error in accept");
			printf("%d", errno);
		}

		pthread_t newThread;
		int* fd = (int *)malloc(sizeof(int));
		*fd = newsfd;
		pthread_create(&newThread, NULL, solve, (void*)fd);		
	}	
}
