#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define M 128

void err(char* str)
{
	perror(str);
	exit(1);
}

int main(int argc, char** argv)
{
	if (argc < 3)
		err("usage : /.obj portno filename.txt");

	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
		err("socket ");

	struct sockaddr_in s_addr, c_addr;
	memset((char *)&s_addr, 0, sizeof s_addr);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = atoi(argv[1]);
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (connect(sfd, (struct sockaddr*)&s_addr, sizeof(s_addr)) < 0)
		err("connection error ");

	char fname[M];
	strcpy(fname, argv[2]);
	write(sfd, fname, strlen(fname));

	fname[strlen(fname) - 4] = '\0';

	strcat(fname, "_copy.txt");
	FILE* fp = fopen(fname, "w");

	char buf[M];	
	while (1)
	{
		read(sfd, buf, M);
		if (strncmp(buf, "-1", 2) == 0)
			break;
//		printf("%s", buf);
		fprintf(fp, "%s", buf);
	}
	fclose(fp);
	close(sfd);
}
