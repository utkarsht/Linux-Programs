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
	int fd = open("file.txt", O_RDONLY);
	if (fd < 0)
		err("file open");

	struct sockaddr_un p_addr;
	int n, len;
	char buff[M];
	const char path[] = "/tmp/socket";
	unlink(path);

	int usfd = init_sockbind(path);
	if (usfd < 0)
		err("unix sfd");

	len = sizeof(p_addr);
	int nusfd = accept(usfd, (struct sockaddr *)&p_addr, &len);
	if (nusfd < 0)
		err("accept");

	while (1)
	{
		printf("Enter no of bytes to read \n");
		scanf("%d", &n);

		memset(buff, '\0', n);
		read(fd, buff, n);
		buff[n] = '\0';
		printf("%s\n", buff);		

		if (send_fd(nusfd, fd) < 0)
			err("send_fd");
		close(fd);

		if ((fd = recv_fd(nusfd)) < 0)
			err("recv_fd");				
	}	 		
	return 0;
}
