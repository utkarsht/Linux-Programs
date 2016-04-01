#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void sighandler()
{
	printf("Ctrl C is pressed");
	exit(1);
}

void sighandler1()
{
	printf("Ctrl D is pressed");
	exit(1);
}

int main()
{
	signal(SIGINT, sighandler);
	signal(SIGINT, sighandler1);
	
	for (;;)
	{}
	return 0;
}
