#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 500

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char **argv)
{
	struct sockaddr_in cli_addr, serv_addr;
	struct hostent *server;
	int sockfd, portno, n, clilen;
	char buffer[BUFFER_SIZE];		
	
	if(argc < 3)
	{
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
	
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
        error("ERROR opening socket ");	
	
	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		fprintf(stderr,"ERROR, no such host\n");
        exit(0);
	}
	
	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	bcopy((char*)server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
	//serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting ");
	
	clilen = sizeof(cli_addr);	
	
	getsockname(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	printf("Client socket has IP: %s\n", inet_ntoa(cli_addr.sin_addr));
	printf("Client socket has port: %hu\n", ntohs(cli_addr.sin_port));
	
	printf("Enter data: ");
	bzero(buffer, BUFFER_SIZE);
	fgets(buffer, BUFFER_SIZE, stdin);
	
	n = write(sockfd, buffer, BUFFER_SIZE);
	if(n < 0) 
         error("ERROR writing to socket ");
    
    bzero(buffer, BUFFER_SIZE);
    
	n = read(sockfd, buffer, BUFFER_SIZE);
	if(n < 0) 
         error("ERROR reading from socket ");
	
	printf("Received Data from Server: %s\n", buffer);
	close(sockfd);
	
	return 0;
}

















