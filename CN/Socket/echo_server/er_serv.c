//	http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define M 256

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[M];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = 6001;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
     	error("ERROR on binding");

     listen(sockfd, 5);

     while(1)
	 {
	     clilen = sizeof(cli_addr);
  		 newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     	 if (newsockfd < 0) 
        	error("ERROR on accept");

		 int c = fork();
         if (c == 0)
         {
             close(sockfd);
			 while(1)
			 {
	 	    	 bzero(buffer, M);
			 	 n = read(newsockfd, buffer, M);
			  	 if (n < 0) 
		 			error("ERROR reading from socket");
			 
			 	 printf("Here is the message: %s\n", buffer);
			 	 n = write(newsockfd, "I got your message", 18);
			 	 if (n < 0) 
					error("ERROR writing to socket");
			 }
         }
		 else
		 {
			 close(newsockfd);		
		 }		
	 } 
     return 0; 
}
