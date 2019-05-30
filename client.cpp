#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "common_data.h"
#include <string.h>
#include <iostream>

using namespace std;

#define PORT     1056
#define MAXLINE 1024 
  
// Driver code 
int main(int argc, char* argv[]) { 
    int command_id = atoi(argv[1]);
    int delay = atoi(argv[2]);
    bool repeat = !(strcmp(argv[3], "true"));

    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello from client"; 
    struct sockaddr_in     servaddr; 
    client_parameters params;

    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    int n, len;
    
    params.command_id = command_id;
    params.delay = delay;
    params.repeat = repeat;

    sendto(sockfd, (void *)&params, sizeof(params), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)); 
    printf("Hello message sent.\n"); 
          
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
    buffer[n] = '\0'; 
    printf("Server : %s\n", buffer); 
  
    while (repeat)
    {
    	n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
        	        MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
	buffer[n] = 0;
    	printf("Server : %s\n", buffer); 
    }

    close(sockfd); 
    return 0; 
} 
