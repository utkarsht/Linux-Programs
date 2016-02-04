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

void* rcv_msg(void *arg)
{
	struct msg_buf mbuf;
	mbuf.mtype = 1;
	while(1)
	{
		if(msgrcv(msqid, (void *)&mbuf, sizeof(mbuf.data), 0, 0) == -1)
			err("msgrcv ");
		if(strcmp(mbuf.data, "exit\n") == 0)
			exit(1);
		printf("%s", mbuf.data);
	}
}

int main()
{
	if((msqid = msgget(KEY, IPC_CREAT | 0666)) == -1)
		err("msgget ");

	rcv_msg(NULL);
	return 0;
}
