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


/*
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
*/


int main()
{
	
	struct sockaddr_un addr;
	int i;
	int ret;
	int dataSocket;
	char buffer[BUFFER_SIZE];

	// [1st STEP] Create Socket
	dataSocket = socket(AF_UNIX, SOCK_STREAM, 0);

	// Error handling
	if(dataSocket == -1)
	{
		std::cout << "[ERROR] Socket creation failed" << '\n';
	}
	std::cout << "[INFO] [1st STEP] Socket creation OK" << '\n';

	// Define connection (server) socket name - Same as Server side
	memset(&addr, 0, sizeof(struct sockaddr_un));
	// Specify the socket credentials
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

	// Send CIR to server - from client (data socket)
	ret = connect(dataSocket, (const struct sockaddr*) &addr, sizeof(struct sockaddr_un));

	// Error handling
	if(ret == -1)
	{
		std::cout << "[ERROR] Unable to connect. Server down" << '\n';
	}
	std::cout << "[INFO] [2nd STEP] Connect OK" << '\n';

	// After connection - Server is waiting for info
	// SEND CLIENT INFO TO SERVER - Get Data
	std::string clientInfo("[Master] [Dev1]");
	send(dataSocket, clientInfo.c_str(), strlen(clientInfo.c_str()), 0);
	std::cout << "[INFO] [3th STEP] Client info sent OK" << '\n';


	// ---- 
	std::string commandLineString;
	do
	{
		std::cout << "$ ";
		std::getline(std::cin, commandLineString);

		// SEND DATA TO SERVER
		send(dataSocket, commandLineString.c_str(), strlen(commandLineString.c_str()), 0);


		// ... RECEIVE DATA FROM SERVER ...
		// Blocking call

		// Prepare Buffer
		memset(buffer, 0, BUFFER_SIZE);
		ret = read(dataSocket, buffer, BUFFER_SIZE);

		std::string receivedMessageStr(buffer);
		std::cout << "[EVENT] Message receive size: " << receivedMessageStr.length() << " ";
		std::cout << "Received msg from client: " << receivedMessageStr << '\n';

	} while (commandLineString != "exit");


	// Close socket
	close(dataSocket);

	return 0;
}



