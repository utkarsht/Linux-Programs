#include "utils.h"
#define M 128

char buffer[M];

int main(int argc, char** argv)
{
	if (argc != 2)
		err("usage: ./a.out portno");

	int portno = atoi(argv[1]);
	int sfd = tcp_accept(portno);
	read(0, buffer, M);
	write(sfd, buffer, M);

	sleep(1);
	read(sfd, buffer, M);
	write(1, buffer, M);
}
