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
	sprintf(temp, "Client %d sent: %s", id, buf);
	return temp;
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("Please provide Id too \n");
		exit(1);
	}

	mkfifo("ls_ctos", 0666);
	mkfifo("ls_stoc", 0666);
	mkfifo("ls_tols", 0666);

	int ctos = open("ls_ctos", O_WRONLY);
	int stoc = open("ls_stoc", O_RDONLY);

	int tols = open("ls_tols", O_WRONLY);
	char buf[M];

	int id = atoi(argv[1]);
	int a, b;
	printf("files opened\n");
	while(1)
	{
		fgets(buf, M, stdin);	//	enter two numbers
		write(tols, addDtl(buf, id), M);
		write(ctos, buf, M);
		read(stoc, buf, M);
		printf("%s\n", buf);	
	}

	return 0;
}
