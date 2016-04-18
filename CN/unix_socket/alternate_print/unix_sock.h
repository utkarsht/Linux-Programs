#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <error.h>

int init_sockbind(const char * path);
int init_sockconnect(const char * path);
int send_fd(int sock_fd, int fd);
int recv_fd(int sock_fd);

/**
 * @param  PATH unique temprory path for passing fds
 * @return socket fd on success or return -1 on error !
 */

void err(char *str)
{
	perror(str);
	exit(1);
}

int init_sockconnect(const char * PATH) 
{
	struct sockaddr_un addr;
	int len, usfd;
	if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		err("socket() ");
		
	addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, PATH);
    len = strlen(addr.sun_path) + sizeof(addr.sun_family);
    if (connect(usfd, (struct sockaddr *)&addr, len) == -1)
        err("connect() ");
        
    return usfd;
}

int init_sockbind(const char * PATH) 
{
    struct sockaddr_un addr;
    int len, usfd;
    if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        err("socket() ");
        
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, PATH);
    len = strlen(addr.sun_path) + sizeof(addr.sun_family);
    if (bind(usfd, (struct sockaddr *)&addr, len) == -1) 
        err("bind() ");
        
    if(listen(usfd, 5) < 0) 
        err("listen() ");
    return usfd;
}

int send_fd(int sock_fd, int fd) 
{
	struct msghdr message; 
	struct iovec iov[1];
	struct cmsghdr *ctrl_msg = NULL;

	char ctrl_buf[CMSG_SPACE(sizeof(int))];
	char data[1];

	memset(&message, 0, sizeof(struct msghdr));
	memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

	/* We are passing at least one byte of data so that recvmsg() will not return 0 */
	data[0] = '*';
	iov[0].iov_base = data;
	iov[0].iov_len = sizeof(data);

	message.msg_name = NULL;
	message.msg_namelen = 0;
	message.msg_iov = iov;
	message.msg_iovlen = 1;
	message.msg_controllen =  CMSG_SPACE(sizeof(int));
	message.msg_control = ctrl_buf;

	ctrl_msg = CMSG_FIRSTHDR(&message);
	ctrl_msg->cmsg_level = SOL_SOCKET;
	ctrl_msg->cmsg_type = SCM_RIGHTS;
	ctrl_msg->cmsg_len = CMSG_LEN(sizeof(int));

	int * fdptr = (int *) CMSG_DATA(ctrl_msg);
    memcpy((void *)fdptr, (void *)&fd, sizeof(int));

	return sendmsg(sock_fd, &message, 0);
}

int recv_fd(int sock_fd) 
{
 	struct msghdr message;
 	struct iovec iov[1];
 	struct cmsghdr *ctrl_msg = NULL;
 	char ctrl_buf[CMSG_SPACE(sizeof(int))];
 	char data[1]; int fd;
 	
    memset(&message, 0, sizeof(struct msghdr));
    memset(ctrl_buf, 0, CMSG_SPACE(sizeof(int)));

    /* For the dummy data */
    iov[0].iov_base = data;
    iov[0].iov_len = sizeof(data);
	
	message.msg_name = NULL;
    message.msg_namelen = 0;
    message.msg_control = ctrl_buf;
    message.msg_controllen = CMSG_SPACE(sizeof(int));
    message.msg_iov = iov;
    message.msg_iovlen = 1;

    if(recvmsg(sock_fd, &message, 0) < 0)
    	return -1;
  	ctrl_msg = CMSG_FIRSTHDR(&message);
  	if((ctrl_msg->cmsg_level == SOL_SOCKET) && (ctrl_msg->cmsg_type == SCM_RIGHTS)) 
	{
  		int * _fd = (int *) CMSG_DATA(ctrl_msg);
  		fd = * _fd;
  	}
  	return fd;
}
