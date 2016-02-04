#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#define M 128

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("Insufficient argument\n");
		exit(1);		
	}

	int i, cnt;
	FILE* fp;
	char buf[M];

	int fd = open("dummy.txt", O_CREAT | O_WRONLY);
/*
	fscanf(fd, "%s", buf);
//	read(fd, buf, M);
	printf("%s\n", buf);	
	exit(1);

	dup2(fd, 0);
*/
	freopen("dummy.txt", "r", stdin);
	
	for (i = 1; i < argc; i++)
	{
		fp = popen(argv[i], "r");
		fgets(buf, M, fp);	
 		pclose(fp);

		if(i == argc - 1)
			printf("%s\n", buf);
		else
			write(fd, buf, 1);
	}
	return 0;
}
