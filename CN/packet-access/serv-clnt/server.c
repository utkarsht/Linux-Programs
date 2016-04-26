#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <netinet/if_ether.h>

#define M 1024
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
    
	if (bind(rsfd, (struct sockaddr*)&addr, sizeof addr) < 0)
		err("bind() ");

	char msg[M], buf[M];
	int len, n = 0;
	struct sockaddr_in src_addr, dest_addr;
	struct iphdr* ipheader;

    while(1) 
	{
		len = sizeof(addr);
        if(recvfrom(rsfd, buf, M, 0, (struct sockaddr *)&addr, &len) < 0) 
            err("recvfrom() ");
		printf("Packet Received %d \n", ++n);

		ipheader = (struct iphdr*)buf;
	
		bzero((char *) &src_addr, sizeof(src_addr));
		bzero((char *) &dest_addr, sizeof(dest_addr));
		src_addr.sin_addr.s_addr = ipheader->saddr;
		dest_addr.sin_addr.s_addr = ipheader->daddr;

		printf("Version : %d\n", ipheader->version);
		printf("Type of service : %d\n", ipheader->tos);
		printf("Total length : %d\n", ipheader->tot_len);
		printf("ID : %d\n", ipheader->id);
		printf("Fragment off : %d\n", ipheader->frag_off);
		printf("Time to leave : %d\n", ipheader->ttl);
		printf("Protocol : %d\n", ipheader->protocol);
		printf("CheckSum : %d\n", ipheader->check);
		printf("Source address : %s\n", inet_ntoa(src_addr.sin_addr));
		printf("Destination address : %s\n", inet_ntoa(dest_addr.sin_addr));

		int ipheaderlen = ipheader->ihl * 4;    
		strcpy(msg, buf + ipheaderlen);
		msg[strlen(msg)] = '\0';
		printf("Message sent : %s\n\n", msg);		
	}
    return 0;
}
