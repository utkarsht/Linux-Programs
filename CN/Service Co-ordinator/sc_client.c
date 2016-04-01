#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#define M 128
#define MAX 10

int ctos, svtoc;
char buf[M];

void sigHandler_read()
{
	read(svtoc, buf, M);
}

int main()
{
	signal(SIGUSR1, sigHandler_read);
	ctos = open("sc_ctos", O_WRONLY);
	svtoc = open("sc_svtoc", O_RDONLY);

	char a[MAX];
	int pid = getpid();
	while(1)
	{
		scanf("%s", a);
		sprintf(buf, "%d %s", pid, a);
		printf("%d %s\n", pid, a);
		write(ctos, buf, M);
		if(strcmp(a, "exit") == 0)
			break;
		pause();
		printf("%s\n", buf);
	}
	return 0;
}
