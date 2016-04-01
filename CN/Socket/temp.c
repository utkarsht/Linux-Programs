//	prev iss_serv.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define M 256

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char** argv)
{
	if (argc < 2)
		err("usage : ./obj portno");	

	int n, sfd, newsfd, portno, clntlen;
	char buffer[M];
	struct sockaddr_in serv_addr, clnt_addr;

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0) 
		error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portno);

	if (bind(sfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");

	listen(sfd, 5);
	fd_set test;
	FD_ZERO(&test);
	FD_SET(sfd, &test);

	while(1)
	{
		select(sfd + 1, &test, NULL, NULL, NULL);

		if (FD_ISSET(sfd, &test))
		{
			clntlen = sizeof(clnt_addr);
			newsfd = accept(sfd, (struct sockaddr *) &clnt_addr, &clntlen);
			if (newsfd < 0)
				error("error in accept");
		
			int c = fork();
			if (c == 0)
			{
				close(sfd);
				bzero(buffer, M);
				n = read(newsfd, buffer, M);
			  	if (n < 0) 
		 			error("ERROR reading from socket");
				printf("%s\n", buffer);	
				printf("Connected now give input\n");
			
				dup2(newsfd, 0);
				dup2(newsfd, 1);
				execl(buffer, buffer, NULL);
			}
			else
			{
				close(newsfd);
			}
		}
	}
}	
