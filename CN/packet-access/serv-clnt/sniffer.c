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

void err(char* str)
{
	perror(str);
	exit(1);
}

int main(int argc, char** argv)
{
	/*
		Sniffer catches both incoming and outgoing packets for
		incoming packet only repalce ETH_P_ALL to ETH_P_IP.
	*/
	int rsfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (rsfd < 0)
		err("socket() ");
	
	struct sockaddr_in addr;
	char buf[M];
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

		printf("\n\n");	
	}
    return 0;
}
