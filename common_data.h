#include <iostream>

struct client_parameters
{
        int command_id;
        int delay;
        bool repeat;
};

enum commands
{
	GET_TIME,
	GET_OS,
	GET_HOST_FILE
};

void print_buffer(char* buff, int len)
{
        for (int i = 0; i < len; i++)
        {
                printf(" %08x", buff[i]);
        }

        printf("\n");
}

