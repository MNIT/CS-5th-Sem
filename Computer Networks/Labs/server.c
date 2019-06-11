/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 500

void dostuff(int, struct sockaddr_in);

/* This function is called when a system call fails.
   It displays a message about the error on stderr and then aborts the program. */
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char **argv)
{
	struct sockaddr_in cli_addr, serv_addr;
	
	int sockfd, newsockfd, clilen, portno;
	char buffer[BUFFER_SIZE];
	int n, pid;
	
	if(argc < 2)
	{
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
    }
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) 
        error("ERROR opening socket ");
       
    portno = atoi(argv[1]);
    
    bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding ");
	
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	
	while(1)
	{		
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if(newsockfd < 0) 
          error("ERROR on accept ");
       
        pid = fork();
        if(pid < 0)
            error("ERROR on fork ");
        
        if(pid == 0)
        {
            close(sockfd);
            dostuff(newsockfd, cli_addr);
            exit(0);
        }
        else
        	close(newsockfd);
    }

	return 0;
}


/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff(int newsockfd, struct sockaddr_in cli_addr)
{
   	int n;
   	char buffer[BUFFER_SIZE];
      
   	printf("Client IP: %s\n", inet_ntoa(cli_addr.sin_addr));
	printf("Client Port: %hu\n", ntohs(cli_addr.sin_port));
		
	bzero(buffer, BUFFER_SIZE);
		
	n = read(newsockfd, buffer, BUFFER_SIZE);
	if(n < 0)
		error("ERROR reading from socket ");
			
	printf("Received: %s", buffer);
		
	n = write(newsockfd, "I got your message.", 19);
	if(n < 0)
		error("ERROR writing to socket ");
			
	printf("==============\n\n");
}














