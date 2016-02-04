#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <pthread.h>
#define M 100
#define N 4					//	Total client should be defined before execution i.e. limitation

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

int msqid[N];
int client_num;

void* rcv_msg(void *arg)
{
	struct msg_buf mbuf;
	mbuf.mtype = 1;

	while(1)
	{
		if(msgrcv(msqid[client_num], (void *)&mbuf, sizeof(mbuf.data), 0, 0) == -1)
			err("msgrcv ");
		if(strcmp(mbuf.data, "exit\n") == 0)
			exit(1);
		printf("%s", mbuf.data);
	}
}

void* snd_msg(void *arg)
{
	struct msg_buf mbuf;
	mbuf.mtype = 1;
	int i;

	while(fgets(mbuf.data, M, stdin))
	{
		for(i = 0; i < N; i++)				//	sending msg to all clients except itself		
		{
			if(i != client_num)
				if((msgsnd(msqid[i], (void *)&mbuf, sizeof(mbuf.data), 0)) == -1)
					err("msgsnd in snd ");
		}
		if(strcmp(mbuf.data, "exit\n") == 0)
		{
			if((msgctl(msqid[client_num], IPC_RMID, NULL)) == -1)
				err("");
			exit(1);
		}
	}
}
void init_msg_queue()
{
	int i;
	for(i = 0; i < N; i++)
	{
		if((msqid[i] = msgget(i + 100, IPC_CREAT | 0666)) == -1)
			err("msgget ");
	}
}

int main(int argc, char* argv[])
{
	init_msg_queue();
	if(argc < 2)
	{
		printf("Please provide client number.\n");
		exit(1);
	}

	client_num = atoi(argv[1]);
	pthread_t snd, rcv;
	
	pthread_create(&snd, NULL, snd_msg, NULL);
	pthread_create(&rcv, NULL, rcv_msg, NULL);

	pthread_join(snd, NULL);
	pthread_join(rcv, NULL);

	return 0;
}
