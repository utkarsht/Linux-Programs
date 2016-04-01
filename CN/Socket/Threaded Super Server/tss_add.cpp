#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/poll.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <bits/stdc++.h>
#define M 128
#define MAX 10
using namespace std;

int stosv, svtos;
char buffer[M];
pthread_cond_t con;
int f = 0;

void* solve(void* arg)
{
	int id = *(int *)arg;
	int a, b, pid;

	while(1)
	{
		sscanf(buffer, "%d %s %d %d", &pid, buffer, &a, &b);
		if(pid == id)
		{
			sprintf(buffer, "Thread %d : add is %d\n", id, a + b);
			write(svtos, buffer, M);
		}
		pthread_cond_wait(&con, NULL);	
	}
}

int main()
{
	pthread_t pt[MAX];
	stosv = open("stosv1", O_RDONLY);
	svtos = open("svtos", O_WRONLY);
	pthread_cond_init(&con, NULL);
	
	set<int> st;
	int pid, k = 0;
	char dum[M];

	while(1)
	{
		read(stosv, buffer, M);
		sscanf(buffer, "%d %s", &pid, dum);
		
		if(!st.count(pid))
		{
			int* id = (int *)malloc(sizeof(int));
			*id = pid;			
			pthread_create(&pt[k++], NULL, solve, (void *)id);
			st.insert(pid);
		}
		pthread_cond_broadcast(&con);
	}
}
