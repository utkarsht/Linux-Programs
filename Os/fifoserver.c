#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <linux/stat.h>

#define FIFO_FILE "MYFIFO"

int main(void)
{
        int fp;
        char buf[128];

        /* Create the FIFO if it does not exist */
        umask(0);
//	unlink("MYFIFO");		//  delete if exist

        if(mknod(FIFO_FILE, S_IFIFO|0666, 0) == -1)
	{
		perror("mknod err :");
//		exit(1);
	}
	
	fp = open(FIFO_FILE, O_WRONLY);
	while(1)
	{
		read(0,buf, 128);
	        write(fp,buf,80);	
		if(strcmp(buf, "exit\n") == 0)	
			break;
	//	fseek(fp, 0, SEEK_SET);			
	}
	close(fp);   

        return(0);
}
