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

int main()
{
	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char buffer[M];

    portno = 6001;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

	while(1)
	{
		printf("Please enter the message: ");
		fgets(buffer, M, stdin);
		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0) 
		     error("ERROR writing to socket");

		bzero(buffer, M);
		n = read(sockfd, buffer, M);
		if (n < 0) 
		     error("ERROR reading from socket");
		printf("%s\n",buffer);
	}

    return 0;
}	
