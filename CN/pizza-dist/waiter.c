#include "utils.h"
#define M 128

char* shmptr;
int usfd_boy;
char buffer[M];

void shm_init()
{
	int shmid = shmget(1234, M, IPC_CREAT | 0666);
	shmptr = shmat(shmid, 0, 0);
}

int main(int argc, char **argv)
{
	shm_init();

	//	unix connection
	char* path1 = "/tmp/btow";
	int usfd = init_sockconnect(path1);
	
	//	connection with D_boy
	char* path2 = "/tmp/wtoboy";
	usfd_boy = init_sockbind(path2);
	usfd_boy = unix_accept(usfd_boy);

	while (1)
	{
		int client = recv_fd(usfd);
		read(usfd, buffer, M);
		strcpy(shmptr, buffer);
		kill(getppid(), SIGUSR1);

		read(client, buffer, M);		
		sprintf(buffer, "from waiter %s", buffer);
		write(usfd_boy, buffer, M);
	}
}
