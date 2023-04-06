#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <errno.h>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>


#define SOCKET_NAME "/tmp/DemoSocket"
// 128 bytes
#define BUFFER_SIZE 128



// Msg Queue
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)
#define QUEUE_PERMISSIONS 0660



int main()
{
	fd_set readfds;
	char buffer[MSG_BUFFER_SIZE];
	int msgq_fd = 0;

	// Provide recipient msgQ name : format </msgq-name>

	// To set msgQ attribs
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = MAX_MESSAGES;
	attr.mq_msgsize = MAX_MSG_SIZE;
	attr.mq_curmsgs = 0;

	const char* msgQ = "/msgql";

	if((msgq_fd = mq_open(msgQ, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1)
	{
		std::cout << "[ERROR]: MqOpen failed" << '\n';
	}

	while(1)
	{
		FD_ZERO(&readfds);
		FD_SET(msgq_fd, &readfds);
		std::cout << "Receiver blocked on select() system call ..." << '\n';
		
		select(msgq_fd + 1, &readfds, NULL, NULL, NULL);
		if(FD_ISSET(msgq_fd, &readfds))
		{
			std::cout << "Message received ... " << '\n';

			memset(buffer, 0, MSG_BUFFER_SIZE);
			if(mq_receive(msgq_fd, buffer, MSG_BUFFER_SIZE, NULL) == -1)
			{
				std::cout << "mq_receive error ... " << '\n';
			}

			printf("Msg received from Queue = %s\n", buffer);

		}
	}

	return 0;
}


/*
int main()
{
	
	struct sockaddr_un addr;
	int i;
	int ret;
	int data_socket;
	char buffer[BUFFER_SIZE];

	// Create Socket
	data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

	// Error handling
	if(data_socket == -1)
	{
		std::cout << "[ERROR] Socket Creation Failed" << '\n';
	}
	std::cout << "Socket Creation Success" << '\n';

	// Clear the whole structure for portability
	memset(&addr, 0, sizeof(struct sockaddr_un));

	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

	ret = connect(data_socket, (const struct sockaddr*) &addr, sizeof(struct sockaddr_un));

	// Error handling
	if(ret == -1)
	{
		std::cout << "[ERROR] Unable to connect. Server down" << '\n';
	}
	std::cout << "Connect Success" << '\n';

	// Send arguments
	do
	{
		std::cout << "Enter number to send to server: " << '\n';
		scanf("%d", &i);
		ret = write(data_socket, &i, sizeof(int));

		if(ret == -1)
		{
			std::cout << "[ERROR] Unable to write data" << '\n';
		}

		printf("Number of bytes sent = %d, data sent = %d\n", ret, i);

	} while(i);

	// Request result
	memset(buffer, 0, BUFFER_SIZE);
	ret = read(data_socket, buffer, BUFFER_SIZE);
	if(ret == -1)
	{
		std::cout << "[ERROR] Read error" << '\n';
	}

	printf("Recvd from Server : %s \n", buffer);

	// Close socket
	close(data_socket);

	return 0;
}
*/


