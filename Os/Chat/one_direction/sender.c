#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#define KEY 1234
#define M 100

struct msg_buf
{
	long mtype;
	char data[M];
};

void err(char* s)
{
	perror(s);
	exit(1);
}

int msqid;

void* snd_msg(void *arg)
{
	struct msg_buf mbuf;
	mbuf.mtype = 1;
	while(fgets(mbuf.data, M, stdin))
	{
		if((msgsnd(msqid, (void *)&mbuf, sizeof(mbuf.data), 0)) == -1)
			err("msgsnd in snd ");
		if(strcmp(mbuf.data, "exit\n") == 0)
		{
			if((msgctl(msqid, IPC_RMID, NULL)) == -1)
				err("msgctl");
			exit(1);
		}
	}
}

int main()
{
	if((msqid = msgget(KEY, IPC_CREAT | 0666)) == -1)
		err("msgget ");

	snd_msg(NULL);
	return 0;
}
