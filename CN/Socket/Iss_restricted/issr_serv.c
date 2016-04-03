#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define M 256

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int clnt[] = {0, 0};
int max_clnt[] = {2, 1};

int avail(char* servc)
{
	if (strncmp(servc, "add", 3) == 0)
	{
		if (clnt[0] == max_clnt[0])
			return 0;
		clnt[0]++;
	}	
	else
	{
		if (clnt[1] == max_clnt[1])
			return 0;
		clnt[1]++;		
	}
	return 1;
}

//	when got a signal, either read from shared memory
//	which service has send the signal and decrement that count
//  or for two service check from signal number itself <- done here

void sigH1()
{
	clnt[0]--;
}

void sigH2()
{
	clnt[1]--;
}

int main(int argc, char** argv)
{
	if (argc < 2)
		err("usage : ./obj portno");	

	signal(SIGUSR1, sigH1);
	signal(SIGUSR2, sigH2);
	
	int n, sfd, newsfd, portno, clntlen;
	char buffer[M];
	portno = atoi(argv[1]);

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
		
		read(newsfd, buffer, M);
		printf("%s", buffer);	

		if (avail(buffer))
		{
			int c = fork();
			if (c == 0)
			{
				close(sfd);
				printf("Connected now give input\n");

				dup2(newsfd, 0);
				dup2(newsfd, 1);

				if (strncmp(buffer, "add", 3) == 0)
					execl("addr", "addr", NULL);
				else
					execl("squarer", "squarer", NULL);		
			}
			else
			{
				close(newsfd);
			}
		}		
		else
			write(newsfd, "Service not available", 22);
	}
}
