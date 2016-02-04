#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define FIFO_FILE       "MYFIFO"

int main(int argc, char *argv[])
{
       int fp;
        char buf[128];

	fp = open(FIFO_FILE, O_RDONLY);
	if(fp == -1)
		perror("s");
        while(1)
        {
		printf("uper\n");
	//		fseek(fp, 0, SEEK_SET);
		
                read(fp,buf, 80);
		if(strcmp(buf, "exit\n") == 0)	
			break;
                printf("Received string: %s\n", buf);
	//	printf("beech\n");
	//	fseek(fp, 0, SEEK_SET);
	//	printf("Niche\n");
		
//		system("cat /dev/null > MYFIFO");   
        }
	close(fp);
     
        return(0);
}

//	fifo is one to one so, no two client can read at a time.. 
