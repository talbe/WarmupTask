#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>

using namespace std;

class udp_handler
{
	private:
		int sockfd;
		struct sockaddr_in my_addr;

	public:
		void init();
		void bind_address(unsigned short port);
		unsigned short listen(char* buffer, int buff_len);
		void send(unsigned short port, const char* buffer, const int bufferLen);
};

		
