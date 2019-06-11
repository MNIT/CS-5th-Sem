/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 500

/* This function is called when a system call fails.
   It displays a message about the error on stderr and then aborts the program. */
void error(char *msg)
{
    perror(msg);
    exit(1);
}

typedef struct product
{
	int pcode;
	int pprice;
	char pname[26];
} product;

void dostuff(int, struct sockaddr_in, product []);

int t;

int main(int argc, char **argv)
{
	FILE *fp = fopen("product.txt", "r");
	if(fp == NULL)
	{
		printf("Database doesn't exist.\n");
		exit(0);
	}
	
	int i;
	fscanf(fp, "%d", &t);
	product p[t];
	
	for(i=0; i<t; i++)
		fscanf(fp, "%d %s %d", &p[i].pcode, p[i].pname, &p[i].pprice);
	
	printf("\n                        ##  Product Database ##\n");
	printf("     --------------------------------------------------------------\n");
	printf("     |   Product Code   |    Product Price    |    Product Name    \n");
	printf("     --------------------------------------------------------------\n");
	
	for(i=0; i<t; i++)
		printf("     |      %d      |         %4d        |     %s\n", p[i].pcode, p[i].pprice, p[i].pname);
	printf("     --------------------------------------------------------------\n");
	
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
            dostuff(newsockfd, cli_addr, p);
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
void dostuff(int newsockfd, struct sockaddr_in cli_addr, product p[t])
{
   	int n, i, k, totalPrice=0;
   	char buffer[BUFFER_SIZE];
      
   	printf("Connected: %s\n", inet_ntoa(cli_addr.sin_addr));
	//printf("Client Port: %hu\n", ntohs(cli_addr.sin_port));
	
	while(1)
	{	
		bzero(buffer, BUFFER_SIZE);
		
		n = read(newsockfd, buffer, BUFFER_SIZE);
		if(n < 0)
			error("ERROR reading from socket ");			
		
		printf("\nReceived: %s", buffer);
		printf("Client: %s\n", inet_ntoa(cli_addr.sin_addr));
		
		//int token = atoi(strtok(buffer, " "));
		int code, no, r, flag = 1;
		char temp[BUFFER_SIZE];
		
		for(i=0; i<strlen(buffer); i++)
		{
			if(!isdigit(buffer[i]) && !isspace(buffer[i]))
			{				
				flag = 0;
				break;
			}
		}
		
		if(strlen(buffer) < 10 || flag == 0)
		{
			n = write(newsockfd, "1 -- Protocol error", 20);
			if(n < 0)
				error("ERROR writing to socket ");
			continue;
		}
		
		sscanf(buffer, "%d", &r);
		
		if(r == 0)
		{
			sscanf(buffer, "%d %d %d", &r, &code, &no);
			//code = atoi(strtok(NULL, " "));
			//no = atoi(strtok(NULL, " "));
			
			for(k=0; k<t; k++)
			{
				if(p[k].pcode == code)
					break;
			}
			
			if(k==t)
				sprintf(temp, "1 -- %s", "Product not in the database.");
			
			else
			{
				totalPrice += p[k].pprice * no;			
				sprintf(temp, "0 -- %d %s", p[k].pprice, p[k].pname);
			}
						
			n = write(newsockfd, temp, BUFFER_SIZE);
			if(n < 0)
				error("ERROR writing to socket ");
		}
		
		else if(r == 1)
		{
			sprintf(temp, "0 -- Total cost = %d", totalPrice);
			
			n = write(newsockfd, temp, BUFFER_SIZE);
			if(n < 0)
				error("ERROR writing to socket ");
			
			printf("\nDisconnected: %s\n", inet_ntoa(cli_addr.sin_addr));
			close(newsockfd);
			break;
		}
		
		else
		{
			sprintf(temp, "1 -- %s", "Invalid");
			
			n = write(newsockfd, temp, BUFFER_SIZE);
			if(n < 0)
				error("ERROR writing to socket ");
		}
			
	}		
	printf("==============\n\n");
}














