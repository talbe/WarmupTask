#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "ThreadPool.h"
#include "common_data.h"
#include <chrono>
#include <sys/utsname.h>
#include <ctime>
#include <iomanip>

#define PORT     1056 
#define MAXLINE 1024 
using namespace std;

struct client_info
{
	int sockfd;
	struct sockaddr_in cliaddr;
	int addressLen;
	client_parameters params;
};

void print(client_info info)
{
	string res;

	switch (info.params.command_id)
	{
		case(GET_TIME):
		{
			time_t rawtime;
  			struct tm * timeinfo;
  			char buffer[80];

  			time (&rawtime);
  			timeinfo = localtime(&rawtime);

  			strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
  			res = buffer;

  			std::cout << "now date is :: " << res << endl;

			break;
		}
		case(GET_OS):
		{
			utsname os_info;
			uname(&os_info);
			
			res = (string)os_info.sysname + " -- " + (string)os_info.release;

			cout << "os is:: " << res << endl;

			break;
		}
		case(GET_HOST_FILE):
		{
		}
	}

	cout << "about to send " << res << endl;
    	sendto(info.sockfd, (const char *)res.c_str(), 50,  
        	MSG_CONFIRM, (const struct sockaddr *) &info.cliaddr, info.addressLen); 

	while (info.params.repeat)
	{
    		sendto(info.sockfd, (const char *)res.c_str(), 50,  
        		MSG_CONFIRM, (const struct sockaddr *) &info.cliaddr, info.addressLen); 
		sleep(info.params.delay);
	}
}

int main(int argc, char* argv[])
{
	ThreadPool<void (*)(client_info), client_info> th;
	client_info info;
	cout <<"Server start" << endl;
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from server"; 
	struct sockaddr_in servaddr, cliaddr; 
     
	cout << "creating socket file discriptor" << endl;
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
       		perror("socket creation failed"); 
       		exit(EXIT_FAILURE); 
 	} 
      
    	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
      
 	// Filling server information 
   	servaddr.sin_family    = AF_INET; // IPv4 
    	servaddr.sin_addr.s_addr = INADDR_ANY; 
    	servaddr.sin_port = htons(PORT); 
      
	cout << "Binding address" << endl;
    	// Bind the socket with the server address 
    	if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
           	sizeof(servaddr)) < 0 ) 
    	{ 
        	perror("bind failed"); 
        	exit(EXIT_FAILURE); 
    	} 
      
	while (true)
	{	
		cout << "Waiting for clients" << endl;
    		int len, n; 
    		n = recvfrom(sockfd, (void *)&info.params, sizeof(info.params),  
                		MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
		
		print_buffer((char*)&info.params, sizeof(info.params));
		info.sockfd = sockfd;
		info.cliaddr = cliaddr;
		info.addressLen = len;
		cout << "Task submit " << endl;
		th.submit(print, info);
	}

	return 1;
}
