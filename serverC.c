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


#define MYPORT "23482"
#define HOST "localhost"


int main(void){//18-53, set up UDP connection, refer to Beej's Guide
	int sockfd;
	int rv;
	struct addrinfo hints;  
	struct addrinfo *servinfo; 
	struct sockaddr_storage their_addr;
	struct addrinfo *p; 
	socklen_t addr_len;


	memset(&hints, 0, sizeof hints);  
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_DGRAM; 
	hints.ai_flags = AI_PASSIVE; 

	if ((rv = getaddrinfo(HOST, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 0;
	}
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol))== -1) {
			perror("serverC: socket");
			continue;
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("serverC: bind");
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "serverC: failed to bind socket\n");
		return 0;
	}
	freeaddrinfo(servinfo);
	printf( "The Server C is up and running using UDP on port %s.\n", MYPORT);

	while(1){
		addr_len=sizeof their_addr;
		char function[6];
		char input[100];

		recvfrom(sockfd, function, sizeof function, 0, (struct sockaddr *)&their_addr, &addr_len);
		recvfrom(sockfd, input, sizeof input, 0, (struct sockaddr *)&their_addr, &addr_len);

		char * func_name=function;
		char * word=input;
		printf("The Server C received input <%s> and operation <%s> \n", word, func_name);

		int count=0;
		char buffer[100];
		char *exp;

		if(function[0]=='s'&&function[1]=='e'){
			FILE *stream;//open and read the .txt file, refer to GeeksforGeeks
			if((stream=fopen("backendC.txt","r"))!=NULL){
				while(fgets(buffer,100,stream)!=NULL){
					char * front = strtok(buffer, " :: ") ;
					char * back = strtok(NULL, " :: ");
					if(strcmp(word,front)==0){
						count++;
						exp=back;
					}
				}
			}
			printf("The Server C has found < %d > matches\n",count);
			
		}
		else if(function[0]=='p'){
			FILE *stream;
			if((stream=fopen("backendC.txt","r"))!=NULL){
				while(fgets(buffer,100,stream)!=NULL){
					char *front = strtok(buffer, " :: ") ;
					char *back = strtok(NULL, " :: ");
					if(strstr(front,word)==front){
						count++;
						exp=back;
					}
				}
			}
			printf("The Server C has found < %d > matches\n",count);
			
		}
		else if(function[0]=='s'&&function[1]=='u'){
			FILE *stream;
			if((stream=fopen("backendC.txt","r"))!=NULL){
				while(fgets(buffer,100,stream)!=NULL){
					char *front = strtok(buffer, " :: ") ;
					char *back = strtok(NULL, " :: ");
					char *word1 = strstr(front,word);
					if(word1!=NULL){
						char s[100];
						strcpy(s,word1);
						if(strcmp(s,input)==0){
							count++;
							exp=back;
						}	
					}
					
				}
			}
			printf("The Server C has found < %d > matches\n",count);
			
		}

	}

}