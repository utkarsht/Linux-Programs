#include "utils.h"
#define M 128

int sfd, nsfd, id, port;
int *aval;

void shared_init()
{
	int shmid = shmget(0x124, M, IPC_CREAT|0666);
	aval = (int *)shmat(shmid, NULL, 0);
}

void sigHandle()
{
	*(aval + id) = 1;
	char buf[M];

	nsfd = tcp_accept(sfd);
	printf("train arrived at platform %d\n", id + 1);
	while (1)
	{
		read(nsfd, buf, M);
		if(strncmp(buf, "-1", 2) == 0)
			break;
		printf("%s\n", buf);
	}
	printf("train left from platform %d\n", id + 1);
	*(aval + id) = 0;
}

int main(int argc, char** argv)
{
	signal(SIGUSR1, sigHandle);

	id = atoi(argv[1]);
	port = atoi(argv[2]);

	sfd = tcpsocket_bind(port);
	shared_init();

	printf("Platform %d started at port %d\n", id + 1, port);
	while (1)
	{
		sleep(1);
	}
	return 0;
}
