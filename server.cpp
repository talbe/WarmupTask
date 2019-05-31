#include "server.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <chrono>
#include <sys/utsname.h>
#include <ctime>
#include <iomanip>

void server::run()
{
	const int PORT = 1056;

	// Create and bind udp handler
	this->udpHandler.init();
        this->udpHandler.bind_address(PORT);
	
	// Wait and handle client requests	
        while (true)
        {
		client_info info;

		// Wait for clients request
                unsigned short clientPort = this->udpHandler.listen((char*)&info.params, sizeof(info.params));

                info.port = clientPort;
		info.my_udp_handler = this->udpHandler;

		// Submit task for the thread pool
                cout << "Task submit " << endl;
                this->thp.submit(client_req_callback, info);
        }
}

static void server::client_req_callback(client_info info)
{
	// Generate response
        string res = generate_response(info.params.command_id);

	// Send the response to the client
	info.my_udp_handler.send(info.port, res.c_str(), 50);
	
	// Keep do that if repeat is true
        while (info.params.repeat)
        {
		res = generate_response(info.params.command_id);
                info.my_udp_handler.send(info.port, res.c_str(), 50);
                sleep(info.params.delay);
        }
}

static string server::get_time()
{
	time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];

        time (&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);

	return buffer;
}

static string server::get_os()
{
	utsname os_info;
        uname(&os_info);
	
	return ((string)os_info.sysname + " -- " + (string)os_info.release);
}

static string server::generate_response(int command_id)
{
	string res;

	switch (command_id)
        {
                case(GET_TIME):
                {
                        res = get_time();
                        break;
                }
                case(GET_OS):
                {
                        res = get_os();
                        break;
                }
                case(GET_HOST_FILE):
                {
                        break;
                }
        }

	return res;
}
