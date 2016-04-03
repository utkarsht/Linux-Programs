#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#define M 256

int main(int argc, char* argv[])
{
	int a, b;
	char buffer[M];

	while (1)
	{
		fgets(buffer, M, stdin);
		if (strncmp(buffer, "exit", 4) == 0)
		{
			break;
		}

		sscanf(buffer, "%d %d", &a, &b);
		sprintf(buffer, "Sum is %d\n", a + b);
		write(1, buffer, strlen(buffer));	
	}

	kill(getppid(), SIGUSR1);
	write(1, "Service exited", 15);
}
