#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <netinet/in.h>
#include "unix_sock.h"
#define M 256
#define MAX 2

void* service(void* arg)
{
	int fd = *(int *)arg;
	char buf[M];
	int a, b;
	printf("Start servicing with %d\n", fd);

	while (1)
	{
		read(fd, buf, M);
		sscanf(buf, "%d %d", &a, &b);
		sprintf(buf, "Backup Server, thread %d : Sum is %d\n", fd, a + b);
		write(fd, buf, M);
	}
}

int main(int argc, char* argv[])
{
	char path[] = "/tmp/sockpath";
	int usfd = init_sockconnect(path);
	if (usfd < 0)
		err("unix sfd connect ");	
	
	char buf[10];
	read(usfd, buf, 10);
	printf("%s", buf);

	int i;
	printf("Waiting for receiving fds\n");
	pthread_t newThread[MAX];
	for (i = 0; i < MAX; i++)
	{
		int fd = recv_fd(usfd);
		if (fd < 0)
			err("recv fd error ");

		pthread_create(&newThread[i], NULL, service, (void *)&fd);
	}
	pthread_join(newThread[0]);
	pthread_join(newThread[1]);
}
