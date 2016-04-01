#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <pthread.h>
#define M 128

void err(char *str)
{
	perror(str);
	exit(1);
}

void* readT(void *arg)
{
	char buf[M];
	int nsfd = *(int *)arg;
	while(1)
	{
		read(nsfd, buf, M);
		if(strlen(buf) > 1)
			printf("%s\n", buf);
		
	}
}

void* writeT(void *arg)
{
	char buf[M];
	int nsfd = *(int *)arg;
	while(1)
	{
		read(0, buf, M);
		write(nsfd, buf, M);
	}
}

int main(int argc, char** argv)
{
	if (argc < 2)
		err("Usage : ./obj portno");

	struct sockaddr_in clnt_addr;
	int gp_num;
	printf("Enter the group number :\n");
	scanf("%d\n", &gp_num);

	int port = atoi(argv[1]) + gp_num - 1;				//	6000 + 3 - 1
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
		err("Socket error");

	clnt_addr.sin_family = AF_INET;		
	clnt_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	clnt_addr.sin_port = htons(port);
	
	if (connect(sfd,(struct sockaddr *)&clnt_addr,sizeof(clnt_addr)) < 0) 
        error("connect error");

	pthread_t r, w;
	pthread_create(&r, NULL, &readT, (void *)&sfd);
	pthread_create(&w, NULL, &writeT, (void *)&sfd);

	pthread_join(r, NULL);
	pthread_join(w, NULL);

	return 0;
}
