#include "utils.h"
#define M 128
#define PROTOCOL 150

int main(int argc, char** argv)
{
	int rsfd = socket(AF_INET, SOCK_RAW, PROTOCOL);
	if (rsfd < 0)
		err("socket() ");

	struct sockaddr_in addr;
	bzero((char *)&addr, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    char buf[M];
    while(1) 
	{
        printf("Enter Ad: ");
		read(0, buf, M);
        if(sendto(rsfd, buf, strlen(buf), 0, (struct sockaddr*)&addr, sizeof addr) < 0)
            err("sendto() ");
    }
    return 0;
}

