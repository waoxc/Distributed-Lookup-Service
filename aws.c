#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define UDPPORT "24482"
#define TCPCLIENT "25482"
#define TCPMONITOR "26482"
#define HOST "localhost"
#define PORTA "21482"
#define PORTB "22482"
#define PORTC "23482"
#define BACKLOG "100"

void sigchld_handler(int s){
    
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int sendOverUDP(char *func_name, char *word, char server){//37-72, set up UDP connection, refer to Beej's Guide
	int mysock;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char* backserver_port;

    if(server == 'A')
    	backserver_port = PORTA;
    else if (server == 'B')
    	backserver_port = PORTB;
    else if( server == 'C')
    	backserver_port = PORTC;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_DGRAM;


    if ((rv = getaddrinfo(HOST, backserver_port, &hints, &servinfo))
			!= 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((mysock = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("talker: socket");
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "talker: failed to bind socket\n");
		return 2;
	}


	sendto(mysock, func_name, sizeof func_name, 0, p->ai_addr,p->ai_addrlen);
	sendto(mysock, word, 100, 0, p->ai_addr,p->ai_addrlen);
	printf("The AWS sent <%s> and <%s> to Backend-Server %c \n", word, func_name, server);

}

int setUpMonitor(){//81-122, set up TCP connection, refer to Beej's Guide
	int sockfd, new_fd;  
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; 
    socklen_t sin_size;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 

    if ((rv = getaddrinfo(HOST, TCPMONITOR, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1) {
			perror("setsockopt");
			exit(1);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}
	freeaddrinfo(servinfo); 

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
}

int main(void){//130-172, set up TCP connection, refer to Beej's Guide

	int sockfd, new_fd;  
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; 
    socklen_t sin_size;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 

    if ((rv = getaddrinfo(HOST, TCPCLIENT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))
				== -1) {
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))
				== -1) {
			perror("setsockopt");
			exit(1);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}
	freeaddrinfo(servinfo); 

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	setUpMonitor();
	
	printf( "The AWS is up and running. \n");

	while(1){
		sin_size = sizeof their_addr;	
		new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");			
			exit(1);
		}

		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);
		struct sockaddr_in addrTheir;
		memset(&addrTheir, 0, sizeof(addrTheir));
		int len = sizeof(addrTheir);
		getpeername(new_fd, (struct sockaddr *) &addrTheir, (socklen_t *) &len);
		int client_port = addrTheir.sin_port;

		char func_name[7];
		char word[27];

		recv(new_fd, func_name, sizeof func_name, 0);	
		recv(new_fd, word, sizeof word, 0);
		
		printf("The AWS received input=%s and function=%s from the client using TCP over port %d. \n", word, func_name, client_port);

		sendOverUDP(func_name, word, 'A');
		sendOverUDP(func_name, word, 'B');
		sendOverUDP(func_name, word, 'C');
	}
}


