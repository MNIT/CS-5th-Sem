#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 500

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
	
	/*  The variable server is a pointer to a structure of type hostent. 
		This structure is defined in the header file netdb.h as follows:

		struct  hostent {
				char    *h_name;        // official name of host 
				char    **h_aliases;    // alias list 
				int     h_addrtype;     // host address type 
				int     h_length;       // length of address 
				char    **h_addr_list;  // list of addresses from name server 
		#define h_addr  h_addr_list[0]  // address, for backward compatiblity 
		};

		It defines a host computer on the Internet. The members of this structure are:

		h_name       Official name of the host.

		h_aliases    A zero  terminated  array  of  alternate
				     names for the host.

		h_addrtype   The  type  of  address  being  returned;
				     currently always AF_INET.

		h_length     The length, in bytes, of the address.

		h_addr_list  A pointer to a list of network addresses
				     for the named host.  Host addresses are
				     returned in network byte order.

		Note that h_addr is an alias for the first address in the array of network addresses. */
	
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
	
	
	/*  argv[1] contains the name of a host on the Internet, e.g. mnit.ac.in. The function:

     	struct hostent *gethostbyname(char *name)

		Takes such a name as an argument and returns a pointer to a hostent containing information about that host. 
		The field char *h_addr contains the IP address. 
		If this structure is NULL, the system could not locate a host with this name.

		The mechanism by which this function works is complex, often involves querying large databases all around the country.  */
	
	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		fprintf(stderr,"ERROR, no such host\n");
        exit(0);
	}
	
	
	/*  This code sets the fields in serv_addr. 
		Much of it is the same as in the server. 
		However, because the field server->h_addr is a character string, we use the function:

		void bcopy(char *s1, char *s2, int length)

		which copies length bytes from s1 to s2.  */
	
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

















