#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define M 256

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int ar[2] = {0};

int already(char *buff)
{
	if (strncmp(buff, "add", 3) == 0)
	{
		if (ar[0])
			return 1;
		else
			ar[0] = 1;
	}
	if (strncmp(buff, "square", 6) == 0)
	{
		if (ar[1])
			return 1;
		else
			ar[1] = 1;
	}	
	return 0;
}

char* getPort(char *buff)
{
	if (strncmp(buff, "add", 3) == 0)
		return "6500";
	if (strncmp(buff, "square", 6) == 0)
		return "6501";	
}

int main(int argc, char** argv)
{
	if (argc < 2)
		err("usage : ./obj portno");	

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
			error("error in server accept");
			printf("%d", errno);
		}

		read(newsfd, buffer, M);
		if (!already(buffer))
		{
			int c = fork();
			if (c == 0)
			{
				close(sfd);
				printf("Connected now give input\n");

				if (strncmp(buffer, "add", 3) == 0)
					execl("add", "add", getPort(buffer), NULL);
				else
					execl("square", "square", getPort(buffer), NULL);		
			}
			else
			{
				close(newsfd);
			}
		}
	}
}
