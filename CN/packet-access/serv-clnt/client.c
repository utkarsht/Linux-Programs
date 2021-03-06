#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <netinet/if_ether.h>

#define M 128
#define PROTOCOL 150

void err(char* str)
{
	perror(str);
	exit(1);
}

int main(int argc, char** argv)
{
	if (argc < 2)
		err("usage: ./obj portno");

	int rsfd = socket(AF_INET, SOCK_RAW, PROTOCOL);
	if (rsfd < 0)
		err("socket() ");

	struct sockaddr_in addr;
	bzero((char *) &addr, sizeof(addr));
	addr.sin_family = AF_INET;
    addr.sin_port = atoi(argv[1]);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
	char buf[M];
	int n = 0;

    while(1) 
	{
		read(0, buf, M);
        if(sendto(rsfd, buf, strlen(buf), 0, (struct sockaddr *)&addr, sizeof addr) < 0) 
            err("sendto() ");
        
		printf("Packet Sent %d \n", ++n);
    }
    return 0;
}
