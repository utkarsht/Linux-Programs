/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "arraysum.h"

int *
arraysum_1_svc(args *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	int i;
	result = 0;
	for (i = 0; i < argp->size; i++)
		result += argp->arr[i];

	return &result;
}
