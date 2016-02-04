#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define M 100

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("Insufficient argument\n");
		exit(1);		
	}
	int i;
	FILE* fp1, fp2;
	char buf[M];
	fp2 = 0;

	for (i = 1; i < argc; i++)
	{
		fp1 = popen(argv[i], "r");
		fgets(buf, M, fp1);		//	currently handling one line output only	otherwise read in a while loop
		pclose(fp);

		if (i == argc - 1)		//	If last argument then just output on screen
		{
			printf("%s\n", buf);
			exit(1);
		}
		fp2 = popen(argv[i + 1], "w");
		fputs(buf, fp2);
		pclose(fp2);
	}
	return 0;
}
