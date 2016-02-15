#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <semaphore.h>
#define C 3
#define M 100
#define KEY 1111

int pftos, stop5;
char* shmptr;

int max(int a, int b, int c)
{
	if(a > b && a > c)	return a;
	if(b > a && b > c)	return b;
	if(c > a && c > b)	return c;
}

void readandsend()
{
	char buf[M];
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	fd_set serv, test;
	FD_ZERO(&serv);
	FD_ZERO(&test);

	int fd[2];

	pipe(fd);
	int p = fork();

	if(p == 0)
	{
		close(fd[0]);
		while(1)
		{
			scanf("%s", buf);
			write(fd[1], buf, M);
		}
	}
	else
	{
		close(fd[1]);
	
		FILE* pf = popen("./all_popen", "r");
		int fd_popen = fileno(pf);
		FD_SET(fd_popen, &serv);     //  popen descriptor
		FD_SET(fd[0], &serv);        //  pipe descriptor
		FD_SET(pftos, &serv);        //  fifo descriptor

		int nfds = max(fd_popen, fd[0], pftos) + 1;

		while(1)
		{
			test = serv;
			select(nfds, &test, NULL, NULL, &tv);

			if(FD_ISSET(fd[0], &test))
			{
				read(fd[0], buf, M);
				write(stop5, buf, M);
			}
			if(FD_ISSET(pftos, &test))
			{
				read(pftos, buf, M);
				write(stop5, buf, M);
			}
			if(FD_ISSET(fd_popen, &test))
			{
				read(fd_popen, buf, M);
			//	write(stop5, buf, M);
			//	some problem when using popen too
			}			
		}
	}
}

void sigfun()
{
	write(stop5, shmptr, M);
	sleep(1);	
}

int main()
{
	signal(SIGUSR1, sigfun);
	mkfifo("pftos", 0666);
	mkfifo("stop5", 0666);

	pftos = open("pftos", O_RDONLY);		
	stop5 = open("stop5", O_WRONLY);

	int shmid = shmget(KEY, M, IPC_CREAT | 0666);
	shmptr = shmat(shmid, 0, 0);
	
	readandsend();
	return 0;
}
