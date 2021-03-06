#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define M 128

void err(char* str)
{
	perror(str);
	exit(1);
}

int main(int argc, char** argv)
{
	if (argc < 2)
		err("usage : /.obj portno");

	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
		err("socket ");

	struct sockaddr_in s_addr, c_addr;
	memset((char *)&s_addr, 0, sizeof s_addr);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = atoi(argv[1]);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sfd, (struct sockaddr*)&s_addr, sizeof(s_addr)) < 0)
		err("bind error ");

	listen(sfd, 5);

	int clntlen = sizeof(c_addr);
	int nsfd = accept(sfd, (struct sockaddr*)&c_addr, &clntlen);
	if (nsfd < 0)
		printf("accept error %d", errno);

	char buf[M];
	read(nsfd, buf, M);
	printf("File name received : %s\n", buf);
	int i = 0;

	//	ignoring end garbage part
	for (i = 0; i < strlen(buf); i++)
	{
		if (buf[i] == '.')
		{
			buf[i + 4] = '\0';
			break;
		}
	}

	FILE* fd = fopen(buf, "r");	
	if (fd == NULL)
		printf("%d\n", errno);

	i = 1;
	while (!feof(fd))
	{
		fgets(buf, M, fd);
		write(nsfd, buf, M);	
		printf("Sent %dth part\n", i++);
	}
	write(nsfd, "-1", 2);

	fclose(fd);
	close(sfd);
}
