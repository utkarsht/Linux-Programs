/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "linklist.h"

bool_t
xdr_node (XDR *xdrs, node *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->data))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->next, sizeof (node), (xdrproc_t) xdr_node))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_nodeptr (XDR *xdrs, nodeptr *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->ptr, sizeof (node), (xdrproc_t) xdr_node))
		 return FALSE;
	return TRUE;
}
