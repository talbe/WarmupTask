#include "udp_handler.h"

void udp_handler::init()
{
	cout << "creating socket file discriptor" << endl;
	
	// Creating socket file descriptor
        if ( (this->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
        }

	// Zero my_addr
        memset(&(this->my_addr), 0, sizeof(this->my_addr));
}

void udp_handler::bind_address(unsigned short port)
{
        // Filling server information
        this->my_addr.sin_family      = AF_INET; // IPv4
        this->my_addr.sin_addr.s_addr = INADDR_ANY; // Any available nic 
        this->my_addr.sin_port        = htons(port); // given port

        cout << "Binding address" << endl;
        // Bind the socket with the server address
        if (bind(this->sockfd, 
		 (const struct sockaddr *)&this->my_addr, 
		 sizeof(this->my_addr)) < 0)
        {
                perror("bind failed");
                exit(EXIT_FAILURE);
        }
}

unsigned short udp_handler::listen(char* buffer, int buff_len)
{
	struct sockaddr_in other_addr;
	int addr_len;
	
	cout << "udp handler is about to wait for udp packet " << endl;
	recvfrom(this->sockfd, (void *)buffer, buff_len,
                 	MSG_WAITALL, ( struct sockaddr *) &other_addr, &addr_len);

	return ntohs(other_addr.sin_port);
}

void udp_handler::send(unsigned short port, const char* buffer, const int buffer_len)
{
	struct sockaddr_in other_addr;

        // Filling destenation information
        other_addr.sin_family      = AF_INET; // IPv4
        other_addr.sin_addr.s_addr = INADDR_ANY; // Any available nic 
        other_addr.sin_port        = htons(port); // given port

	sendto(this->sockfd, (const char *)buffer, buffer_len,
                MSG_CONFIRM, (const struct sockaddr *) &other_addr, sizeof(other_addr));
}
