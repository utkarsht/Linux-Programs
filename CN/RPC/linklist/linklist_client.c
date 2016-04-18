/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "linklist.h"

void
linklist_1(char *host)
{
	CLIENT *clnt;
	void  *result_1;
	int  addnode_1_arg;
	struct nodeptr  *result_2;
	char *print_1_arg;
	void  *result_3;
	char *addone_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, LINKLIST, LINKLIST_1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	while (1)
	{
		scanf("%d", &addnode_1_arg);
		result_1 = addnode_1(&addnode_1_arg, clnt);
		if (result_1 == (void *) NULL) {
			clnt_perror (clnt, "call failed");
		}

		result_2 = print_1((void*)&print_1_arg, clnt);
		if (result_2 == (struct nodeptr *) NULL) {
			clnt_perror (clnt, "call failed");
		}
		
		node* temp = result_2->ptr;
		while (temp != NULL)
		{
			printf("%d -> ", temp->data);
			temp = temp->next;
		}
	}

	result_3 = addone_1((void*)&addone_1_arg, clnt);
	if (result_3 == (void *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	linklist_1 (host);
exit (0);
}
