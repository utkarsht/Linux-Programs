#include "utils.h"
#define P 3
#define M 128

int *aval;
int platpid[P];

void shared_init()
{
	int shmid = shmget(0x124, M, IPC_CREAT|0666);
	aval = (int *)shmat(shmid, NULL, 0);
}

void platform_init(int port)
{
	int i;
	char id[M], prt[M];
	for (i = 0; i < P; i++)
	{
		int c = fork();
		if (c == 0)
		{
			sprintf(prt, "%d", port + P + i);
			sprintf(id, "%d", i);
			execl("platform", "platform", id, prt, NULL);	
		}
		else		
		{
			platpid[i] = c;
			*(aval + i) = 0;
		}
	}
}

int available(int i)
{
	return *(aval + i) == 0;
}

int main(int argc, char** argv)
{
	if (argc < 2)
		err("usage : ./obj portno");

	int sfd[P];
	int i, nsfd, fd = -1;
	int port = atoi(argv[1]);
	char buf[M];

	fd_set master, test;
	FD_ZERO(&master);
	FD_ZERO(&test);

	shared_init();
	platform_init(port);

	for (i = 0; i < P; i++)
	{
		sfd[i] = tcpsocket_bind(port + i);
		printf("Station %d listening on port : %d\n", i, port + i);

		if (fd < sfd[i])
			fd = sfd[i];
		FD_SET(sfd[i], &master);
	}

	while (1)
	{
		test = master;
		select(fd + 1, &test, NULL, NULL, NULL);
		
		for (i = 0; i < P; i++)
		{
			if (FD_ISSET(sfd[i], &test))
			{
				nsfd = tcp_accept(sfd[i]);
				break;
			}
		}
		
		if (i != P)
		{
			printf("Train arrived on station - %d\n", i);
			for (i = 0; i < P; i++)
			{
				if (available(i))
				{
					printf("Platform %d is available\n", i + 1);
					sprintf(buf, "%d", port + P + i);
					write(nsfd, buf, M);
					sleep(1);
					kill(platpid[i], SIGUSR1);
					break;
				}
			}

			if (i == P)
			{
				write(nsfd, "-1", 2);
			}
			close(nsfd);
		}
	}
}
