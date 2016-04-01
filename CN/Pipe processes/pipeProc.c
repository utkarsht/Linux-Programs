#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#define M 100

int main(int argc, char** argv)
{
	int i;
	char buf[M];
	FILE* f = popen(argv[1], "r");
	dup2(fileno(f), 0);

	if(argc == 2)		//	in case only one arg
	{
		fgets(buf, M, f);
		printf("at the end : %s\n", buf);
	}
	
	for (i = 2; i < argc; i++)
	{
		f = popen(argv[i], "r");
		
		if(i == argc - 1)
		{		
			fgets(buf, M, f);
			printf("at the end : %s\n", buf);
		}
	}

	return 0;
}
