#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <fcntl.h>
#include <pthread.h>
#include <error.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "unix_sock.h"

#define M 128

int main(int argc, char **argv)
{
	int n, fd;
	char buff[M];
	const char path[] = "/tmp/socket";

	int usfd = init_sockconnect(path);
	if (usfd < 0)
		err("unix sfd");

	while (1)
	{
		if ((fd = recv_fd(usfd)) < 0)
			err("recv_fd");				
		
		printf("Enter no of bytes to read \n");
		scanf("%d", &n);
		memset(buff, '\0', n);
		read(fd, buff, n);
		buff[n] = '\0';
		printf("%s\n", buff);		

		if (send_fd(usfd, fd) < 0)
			err("send_fd");
		close(fd);
	}	 		
	return 0;
}
