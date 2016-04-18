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
    char buffer[1024], buf[M];

    int rsfd = socket(AF_INET, SOCK_RAW, PROTOCOL);
    if(rsfd < 0) 
        err("socket() ");
     
	int n = 0, val = 1;   
    if(setsockopt(rsfd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val)) < 0) 
        err("setsockopt() ");
    
    struct sockaddr_in addr;
    bzero((char *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    struct iphdr *ipheader;
    while(1) 
    {
        printf("Enter Message to Send: ");
        read(0, buf, M);        
        memset(buffer, 0 , M);

        ipheader = (struct iphdr *)buffer;
        ipheader->version = 4;
        ipheader->ihl = 5;
        ipheader->tos = 0;
        ipheader->frag_off = 0;
        ipheader->ttl = 8;
        ipheader->protocol = PROTOCOL;
        ipheader->saddr = inet_addr("127.0.1.1");
        ipheader->daddr = inet_addr("127.0.0.1");
        strcpy(buffer + sizeof(struct iphdr), buf);
        ipheader->tot_len = htons(sizeof(struct iphdr) + strlen(buf));

        if(sendto(rsfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, sizeof addr) < 0) 
            perror("sendto() ");
		printf("Packet Sent %d \n", ++n);
    }
    return 0;
}
