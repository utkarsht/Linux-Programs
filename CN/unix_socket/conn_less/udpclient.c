#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define M 128

void err(char* str)
{
	perror(str);
	exit(1);
}

int main(int argc, char** argv)
{
	if (argc < 2)
		err("usage : /.obj portno");

	int sfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sfd < 0)
		err("socket ");

	struct sockaddr_in c_addr;
	memset((char *)&c_addr, 0, sizeof c_addr);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = atoi(argv[1]);
	c_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	printf("Now give input alternatively\n");

	char buf[M];
	while (1)
	{
		read(0, buf, M);
		if(sendto(sfd, buf, M, 0, (struct sockaddr*)&c_addr, sizeof c_addr) < 0) 
			err("sendto() error");	

		int clntlen = sizeof c_addr;
		if(recvfrom(sfd, buf, M, 0, (struct sockaddr*)&c_addr, &clntlen) < 0) 
			err("recvfrom() error");
		printf("%s\n", buf);	
	}
	close(sfd);
}
