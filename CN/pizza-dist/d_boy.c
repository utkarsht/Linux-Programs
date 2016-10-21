#include "utils.h"
#define M 128
#define W 3

int usfd_boy;
struct pollfd pfd[W];
char buffer[M];

int main(int argc, char **argv)
{	
	//	connection with D_boy
	char* path1 = "/tmp/btoboy";
	usfd_boy = init_sockconnect(path1);

	//	connection with waiter
	char* path2 = "/tmp/wtoboy";
	int i, w, client;
	for (i = 0; i < W; ++i)
	{
		w = init_sockconnect(path2);
		pfd[i].fd = w;
		pfd[i].events = POLLIN;		
	}

	while (1)
	{
		poll(pfd, W, 1000);
		for (i = 0; i < W; ++i)
		{
			if (pfd[i].revents & POLLIN)
			{
				read(pfd[i].fd, buffer, M);
				client = recv_fd(usfd_boy);

				sprintf(buffer, "from boy %s", buffer);
				write(client, buffer, M);
				break;
			}
		}
	}
}
