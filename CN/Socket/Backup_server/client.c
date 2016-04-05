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

int main(int argc, char* argv[])
{
	if (argc < 2)
		err("usage : ./obj portno");

	char buffer[M];
	int sfd, a, b;
	int port = atoi(argv[1]);
	sfd = tcpsocket_connect(port);
	printf("Connected on %d, now give input :\n", sfd);	

	while (1)
	{
		scanf("%d %d", &a, &b);
		sprintf(buffer, "%d %d", a, b);
		write(sfd, buffer, M);
	
		read(sfd, buffer, M);
		printf("%s", buffer);		
	}
}


