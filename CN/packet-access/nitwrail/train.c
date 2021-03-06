#include "utils.h"
#define M 128

int main(int argc, char** argv)
{
	if (argc < 2)
		err("usage : ./obj portno");

	int port = atoi(argv[1]);
	int sfd = tcpsocket_connect(port);

	char buf[M];
	read(sfd, buf, M);
	if (strncmp(buf, "-1", 2) == 0)
	{
		printf("platform not available\n");
	}
	else
	{
		printf("train got %s platform\n", buf);
		port = atoi(buf);
		sfd = tcpsocket_connect(port);
		while (1)
		{
			read(0, buf, M);
			write(sfd, buf, M);
			sleep(0.5);
			if (strncmp(buf, "-1", 2) == 0)
				break;
		}
	}
}
