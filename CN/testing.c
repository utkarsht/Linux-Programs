#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#define M 100

int main(int argc, char** argv)
{
	FILE* f = popen("./p1", "r");
	
	char buf[M];
//	fgets(buf, M, f);

	int N;
	dup2(fileno(f), 0);
	scanf("%d", &N);
	printf("%d\n", N);
}
