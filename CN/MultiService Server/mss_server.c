#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <linux/stat.h>
#define M 100

char *av[10]; 
void getargv(char* str)
{
	char *token;
	token = strtok(str, " ");
	int i = 0;
	while(token != NULL) 
	{
		av[i] = malloc(M);
		
		sprintf(av[i++], "%s", token);
		token = strtok(NULL, " ");
	}
	av[i] = NULL;
}

int main()
{
	mknod("mss_ctos", S_IFIFO|0666, 0);
	mknod("mss_stoc", S_IFIFO|0666, 0);
	
	int ctos = open("mss_ctos", O_RDONLY);
	open("mss_stoc", O_WRONLY);

	char buf[M];	
	int pid;

	while(1)
	{
		pid = fork();
		if(pid > 0)		//	parent
		{
			wait();
		}
		else			//	child
		{
			read(ctos, buf, M);
			getargv(buf);
			printf("%s : %s..\n", av[0], av[1]);
				
			execv(av[0], av);
		}
	}
	
	return 0;
}
