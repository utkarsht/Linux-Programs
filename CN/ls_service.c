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

char temp[M];
char* addDtl(char* buf, int id)
{
	sprintf(temp, "Service %d returned: %s", id, buf);
	return temp;
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("Please provide Id too \n");
		exit(1);
	}
	int ctos = open("ls_ctos", O_RDONLY);
	int stoc = open("ls_stoc", O_WRONLY);
	int tols = open("ls_tols", O_WRONLY);

	char buf[M];
	int a, b;
	int id = atoi(argv[1]);
	
	while(1)
	{
		read(ctos, buf, M);
		sscanf(buf, "%d %d", &a, &b);
		sprintf(buf, "add is -> %d", a + b);
		
		write(tols, addDtl(buf, id), M);
		write(stoc, buf, M);
	}

	return 0;
}
