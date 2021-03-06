#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define M 256

void error(char *msg)
{
    perror(msg);
    exit(1);
}

char *av[10]; 
void getargv(char* str)
{
	char *token;
	token = strtok(str, " ");
	int i = 0;
	while(token != NULL) 
	{
		av[i] = malloc(M);
		
		sprintf(av[i++], "%s", token);
		token = strtok(NULL, " ");
	}
	av[i] = NULL;
}

int main()
{
	int sfd, newsfd, portno, clntlen;
	char buffer[M];
	struct sockaddr_in serv_addr, clnt_addr;
	int n;

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0) 
		error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 6001;
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
			error("error in accept");
		
		int c = fork();
		if (c == 0)
		{
			close(sfd);
			while(1)
			{
				bzero(buffer, M);
				n = read(newsfd, buffer, M);
			  	if (n < 0) 
		 			error("ERROR reading from socket");
				
				printf("%s\n", buffer);
	
				getargv(buffer);
				int d = fork();
				if(d > 0)
				{
				//	wait();
//					read(stdin, buffer, M);
//					write(newsfd, buffer, M);
				//	read from service and send back the result
				}
				else
				{
				//	duplicating the stdout to newsfd, so output of service will go to client directly
					dup2(newsfd, 1);		
					execv(av[0], av);
				}
			}
		}
		else
		{
			close(newsfd);
		}
	}
}	
