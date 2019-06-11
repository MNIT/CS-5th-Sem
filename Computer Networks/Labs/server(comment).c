/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h> //The header file socket.h includes a number of definitions of structures needed for sockets. 
#include <netinet/in.h> //The header file netinet/in.h contains constants and structures needed for internet domain addresses. 
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
	/*  A sockaddr_in is a structure containing an internet address. This structure is defined in <netinet/in.h>.
	    Here is the definition:

		struct sockaddr_in {
				short   sin_family;
				u_short sin_port;
				struct  in_addr sin_addr;
				char    sin_zero[8];
		};

	    An in_addr structure, defined in the same header file, contains only one field, a unsigned long called s_addr.
	    The variable serv_addr will contain the address of the server, 
	    and cli_addr will contain the address of the client which connects to the server. */
	
	struct sockaddr_in cli_addr, serv_addr;
	
	
	/*  sockfd and newsockfd are file descriptors, i.e. array subscripts into the file descriptor table .
	    These two variables store the values returned by the socket system call and the accept system call.
	    clilen stores the size of the address of the client. This is needed for the accept system call.
	    n is the return value for the read() and write() calls; i.e. it contains the number of characters read or written. */
	
	int sockfd, newsockfd, clilen, portno;
	
	//  The server reads characters from the socket connection into this buffer. 
	char buffer[BUFFER_SIZE];
	int n, pid;
	
	if(argc < 2)
	{
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
    }
	
	
	/*  The socket() system call creates a new socket. It takes three arguments. The first is the address domain of the socket.
		Recall that there are two possible address domains, the unix domain for two processes which share a common file system, 
		and the Internet domain for any two hosts on the Internet. The symbol constant AF_UNIX is used for the former, 
		and AF_INET for the latter (there are actually many other options which can be used here for specialized purposes).

		The second argument is the type of socket. Recall that there are two choices here, 
		a stream socket in which characters are read in a continuous stream as if from a file or pipe, 
		and a datagram socket, in which messages are read in chunks. The two symbolic constants are SOCK_STREAM and SOCK_DGRAM. 
	
		The third argument is the protocol. If this argument is zero (and it always should be except for unusual circumstances), 
		the operating system will choose the most appropriate protocol. It will choose TCP for stream sockets and UDP for datagram sockets.

		The socket system call returns an entry into the file descriptor table (i.e. a small integer). 
		This value is used for all subsequent references to this socket. If the socket call fails, it returns -1. 
		In this case the program displays and error message and exits. However, this system call is unlikely to fail.  */
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) 
        error("ERROR opening socket ");
       
    portno = atoi(argv[1]);
    
    
    /*  The function bzero() sets all values in a buffer to zero. It takes two arguments, 
		the first is a pointer to the buffer and the second is the size of the buffer. 
		Thus, this line initializes serv_addr to zeros.  */
    
    bzero((char*)&serv_addr, sizeof(serv_addr));
    
    /*  The variable serv_addr is a structure of type struct sockaddr_in. This structure has four fields. 
		The first field is short sin_family, which contains a code for the address family. 
		It should always be set to the symbolic constant AF_INET. */
	
	serv_addr.sin_family = AF_INET;
	
	/*  The second field of serv_addr is unsigned short sin_port , which contain the port number. 
		However, instead of simply copying the port number to this field, 
		it is necessary to convert this to network byte order using the function htons() 
		which converts a port number in host byte order to a port number in network byte order.  */
	
	serv_addr.sin_port = htons(portno);
	
	/*  The third field of sockaddr_in is a structure of type struct in_addr which contains only a single field unsigned long s_addr. 
		This field contains the IP address of the host. 
		For server code, this will always be the IP address of the machine on which the server is running, 
		and there is a symbolic constant INADDR_ANY which gets this address.  */
	
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	
	/*  The bind() system call binds a socket to an address, in this case the address of the current host and port number 
		on which the server will run. It takes three arguments, the socket file descriptor, the address to which is bound, 
		and the size of the address to which it is bound. The second argument is a pointer to a structure of type sockaddr, 
		but what is passed in is a structure of type sockaddr_in, and so this must be cast to the correct type. 
		This can fail for a number of reasons, the most obvious being that this socket is already in use on this machine.  */
	
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding ");
	
	
	/*  The listen system call allows the process to listen on the socket for connections. 
		The first argument is the socket file descriptor, and the second is the size of the backlog queue, i.e., 
		the number of connections that can be waiting while the process is handling a particular connection. 
		This should be set to 5, the maximum size permitted by most systems. 
		If the first argument is a valid socket, this call cannot fail, and so the code doesn't check for errors.  */
	
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	
	while(1)
	{	
		/*  The accept() system call causes the process to block until a client connects to the server. 
			Thus, it wakes up the process when a connection from a client has been successfully established. 
			It returns a new file descriptor, and all communication on this connection should be done using the new file descriptor. 
			The second argument is a reference pointer to the address of the client on the other end of the connection, 
			and the third argument is the size of this structure.  */
		
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














