/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "nfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
nfs_rpc_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		file_args nfs_create_1_arg;
		file_args nfs_rm_1_arg;
		file_args nfs_cd_1_arg;
		file_args read_1_arg;
		file_write write_1_arg;
		file_args mkdir_1_arg;
		file_args rmdir_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case nfs_ls:
		_xdr_argument = (xdrproc_t) xdr_void;
		_xdr_result = (xdrproc_t) xdr_buf;
		local = (char *(*)(char *, struct svc_req *)) nfs_ls_1_svc;
		break;

	case nfs_create:
		_xdr_argument = (xdrproc_t) xdr_file_args;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) nfs_create_1_svc;
		break;

	case nfs_rm:
		_xdr_argument = (xdrproc_t) xdr_file_args;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) nfs_rm_1_svc;
		break;

	case nfs_cd:
		_xdr_argument = (xdrproc_t) xdr_file_args;
		_xdr_result = (xdrproc_t) xdr_buf;
		local = (char *(*)(char *, struct svc_req *)) nfs_cd_1_svc;
		break;

	case read:
		_xdr_argument = (xdrproc_t) xdr_file_args;
		_xdr_result = (xdrproc_t) xdr_buf;
		local = (char *(*)(char *, struct svc_req *)) read_1_svc;
		break;

	case write:
		_xdr_argument = (xdrproc_t) xdr_file_write;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) write_1_svc;
		break;

	case mkdir:
		_xdr_argument = (xdrproc_t) xdr_file_args;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) mkdir_1_svc;
		break;

	case rmdir:
		_xdr_argument = (xdrproc_t) xdr_file_args;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) rmdir_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (nfs_rpc, nfs);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, nfs_rpc, nfs, nfs_rpc_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (nfs_rpc, nfs, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, nfs_rpc, nfs, nfs_rpc_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (nfs_rpc, nfs, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
