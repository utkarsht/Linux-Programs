#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#define M 128
#define MAX 10
#define KEY 2233

int stosv[MAX];
int i, tS, shmid;
char* shmptr;

struct conn
{
	int snum;
	int pid;
};
struct conn co[MAX];

void err(char *s)
{
	perror(s);
	exit(1);
}

void sigHandler_add_serv()
{
	char name[MAX];
	sprintf(name, "sc_stosv%d", tS + 1);
	printf("%s\n", name);
	co[tS].snum = tS;
	co[tS].pid = -1;
	stosv[tS++] = open(name, O_WRONLY);
}

void set_shm()
{
	if((shmid = shmget(KEY, M, IPC_CREAT|0666)) < 0)
		err("shmget error");	
	if((shmptr = shmat(shmid, 0, 0)) == (char *)-1)
		err("shmat error");
}

int getSvno(int pid)
{
	int i;
	for (i = 0; i < tS; i++)
	{
		if(co[i].pid == pid)
			return co[i].snum;
	}
	for (i = 0; i < tS; i++)
	{
		printf("%d \n", i);
		if(co[i].pid == -1)
		{
			co[i].pid = pid;
			return co[i].snum;
		}
	}
	return -1;
}

void removeSvno(int pid)
{
	int i;
	for (i = 0; i < tS; i++)
	{
		if(co[i].pid == pid)
		{
			co[i].pid = -1;
			return;
		}
	}
}

int main()
{
	mknod("sc_ctos", S_IFIFO|0666, 0);
	mknod("sc_svtoc", S_IFIFO|0666, 0);

	signal(SIGUSR1, sigHandler_add_serv);
	
	int ctos = open("sc_ctos", O_RDONLY);
	int svtoc = open("sc_svtoc", O_WRONLY);

	char buf[M];
	char* avail;
	set_shm();
	tS = 0;
	int pid, a;
	char e[MAX];

	while(1)
	{
		read(ctos, buf, M);
		sscanf(buf, "%d %s", &pid, e);
		if(strcmp(e, "exit") == 0)
		{
			removeSvno(pid);
			continue;
		}

		sscanf(buf, "%d %d", &pid, &a);	
		printf("%d %d\n", pid, a);	
		avail = shmptr;		
		int sn = getSvno(pid);
		if(sn >= 0)
		{
			write(stosv[sn], buf, M);
		}
		else
		{
			strcpy(buf, "No service available");
			write(svtoc, buf, M);
			kill(pid, SIGUSR1);
		}
	}
	return 0;
}
