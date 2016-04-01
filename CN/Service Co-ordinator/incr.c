#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#define M 128
#define MAX 10

int a, pid;
int stosv, svtoc;

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("Insufficient argument\n");
		exit(1);
	}
	svtoc = open("sc_svtoc", O_WRONLY);

	char name[MAX] = "sc_stosv";
	strcat(name, argv[1]);
	mknod(name, S_IFIFO|0666, 0);
	system("pkill -SIGUSR1 o1");

	stosv = open(name, O_RDONLY);	

	char buf[M];
	while(1)
	{
		read(stosv, buf, M);
		sscanf(buf, "%d %d", &pid, &a);	
		printf("Servicing %d\n", pid);
		sprintf(buf, "Incremented value is : %d", a + 1);
		write(svtoc, buf, M);	
		kill(pid, SIGUSR1);		
	}

	return 0;
}
