#include "utils.h"
#define M 128
#define PROTOCOL 150

int sfd, nsfd, id, port;
int *aval;

void shared_init()
{
	int shmid = shmget(0x124, M, IPC_CREAT|0666);
	aval = (int *)shmat(shmid, NULL, 0);
}

void sigHandle()
{
	printf("Signal Caught\n");
	*(aval + id) = 1;
	char buf[M];

	nsfd = tcp_accept(sfd);
	printf("train arrived at platform %d\n", id + 1);
	while (1)
	{
		read(nsfd, buf, M);
		if(strncmp(buf, "-1", 2) == 0)
			break;
		printf("%s\n", buf);
	}
	printf("train left from platform %d\n", id + 1);
	*(aval + id) = 0;
}

void* announcement(void *arg)
{
	int rsfd = socket(AF_INET, SOCK_RAW, PROTOCOL);
	if (rsfd < 0)	
		err("raw socket ");

	char buf[1024]; 
	struct sockaddr_in addr; 
	struct iphdr * ip;
	while(1) 
	{
		int len = sizeof addr;	
		if(recvfrom(rsfd, buf, 1024, 0, (struct sockaddr*)&addr, &len) < 0) 
			err("recvfrom");
		
		ip = (struct iphdr *)buf;
		int iphdrlen = ip->ihl * 4;
		char *msg = buf + iphdrlen;
		printf("%s\n", msg);
	}
}

int main(int argc, char** argv)
{
	if (argc < 3)
		err("usage : ./obj id port");

	signal(SIGUSR1, sigHandle);
	
	pthread_t thread;
	pthread_create(&thread, NULL, announcement, NULL);

	id = atoi(argv[1]);
	port = atoi(argv[2]);

	sfd = tcpsocket_bind(port);
	shared_init();

	printf("Platform %d started at port %d\n", id + 1, port);
	while (1)
	{
		sleep(1);
	}
	pthread_join(thread);
	return 0;
}
