#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <linux/stat.h>
#include <signal.h>
#define M 100

char buf[M];
int ctos, stoc;
	
void sigHandler()
{
	read(stoc, buf, M);
}

int main()
{
	ctos = open("mss_ctos", O_WRONLY);
	stoc = open("mss_stoc", O_RDONLY);

	signal(SIGUSR1, sigHandler);
	int pid = getpid();

	while(1)
	{
		fgets(buf, M, stdin);
		buf[strlen(buf) - 1] = '\0';

		sprintf(buf, "%s %d", buf, pid);
		write(ctos, buf, M);	
		pause();
		printf("%s\n", buf);
	}
	return 0;
}
