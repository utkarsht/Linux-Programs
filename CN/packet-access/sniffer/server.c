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

int main(int argc, char const *argv[]) 
{
    struct sockaddr_in addr; 
	int n, len; 
	int total = 0;
	char buffer[M], buf[M];

    int rsfd = socket(AF_INET, SOCK_RAW, PROTOCOL);
    if(rsfd < 0) 
        err("socket() ");

	int val = 1;
    if(setsockopt(rsfd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val)) < 0) 
        err("setsockopt() ");
    
    struct iphdr * ipheader; 
	unsigned int iphdrlen;
    struct sockaddr_in src_addr, dest_addr;

    while(1) 
	{
        len = sizeof(addr);
        if(recvfrom(rsfd, buffer, M, 0, (struct sockaddr *)&addr, &len) < 0) 
            err("sendto() ");
        
    	printf("Packet Count %d \n", ++total);
        ipheader = (struct iphdr *) buffer; 
    	iphdrlen = ipheader->ihl * 4;
    	
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

    	strcpy(buf, buffer + iphdrlen);
		buf[strlen(buf)] = '\0';
    	printf("Message: %s\n", buf);
		printf("\n\n");       
    }
    return 0;
}
