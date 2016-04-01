#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <string.h>
#define M 100

int main()
{
	FILE* fd2 = popen("./p2 p2", "w");
	FILE* fd3 = popen("./p3 p3", "w");
	FILE* fd4 = popen("./p4 p4", "w");
	FILE* fd5 = popen("./p5 p5", "w");		//	all are "r" !

	FILE* fd6 = popen("./p6", "r");

	int fd[2];
	char buffer[M];
	pipe(fd);
	
	int c = fork();
	if (c == 0)
	{
		close(fd[0]);
//		dup2(fd[1], 1);		
		while (1)
		{
			printf("msg coming from p1\n");
			int r = rand() % 2 + 1;
			sleep(r);
		}
	}
	else
	{
		close(fd[1]);

		struct pollfd pfd[4];
		struct pollfd pfdi[1];
		pfd[0].fd = fileno(fd2);
		pfd[0].events = POLLIN;
		pfd[1].fd = fileno(fd3);
		pfd[1].events = POLLIN;
		pfd[2].fd = fileno(fd4);
		pfd[2].events = POLLIN;
		pfd[3].fd = fileno(fd5);
		pfd[3].events = POLLIN;

		pfdi[0].fd = fd[0];
		pfdi[0].events = POLLIN;

		int i, f;
		while (1)	
		{
			poll(pfd, 4, 1000);
			f = 0;
			for (i = 0; i < 4; i++)
			{
				if (pfd[i].revents & POLLIN)
				{
					read(pfd[i].fd, buffer, M);
					write(fd6, buffer, M);
					f = 1;
					break;
				}
			}	
			if (!f)
			{
				poll(pfdi, 1, 1000);
				if (pfdi[0].revents & POLLIN)
				{
					read(pfdi[0].fd, buffer, M);
					write(fd6, buffer, M);
				}
			}
		}  				
	}
}
