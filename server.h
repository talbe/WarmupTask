#include <iostream>
#include "udp_handler.h"
#include "ThreadPool.h"
#include <string>
#include "common_data.h"

using namespace std;

struct client_info
{
        client_parameters params;
	udp_handler my_udp_handler;
	unsigned short port;
};

class server
{
	private:
		udp_handler udpHandler;
		ThreadPool<void (*)(client_info), client_info> thp;
		static string get_time();
		static string get_os();
		static string generate_response(int command_id);
	public:
		void run();
		static void client_req_callback(client_info info);
};
