/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "square.h"

bool_t
xdr_args (XDR *xdrs, args *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->num))
		 return FALSE;
	return TRUE;
}
