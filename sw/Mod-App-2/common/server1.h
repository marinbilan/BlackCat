#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <errno.h>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>


#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)


void msgQ(int argc, char **argv)
{
	char buffer[MSG_BUFFER_SIZE];
	int recvr_msgq_fd = 0;

	memset(buffer, 0, MSG_BUFFER_SIZE);
	printf("Enter message to be sent to receiver: %s\n", argv[1]);
	scanf("%s", buffer);

	// Some message queue ID
	const char* srvMsg = "/msgql";

	if((recvr_msgq_fd = mq_open(srvMsg, O_WRONLY | O_CREAT, 0, 0)) == -1)
	{
		std::cout << "Client: mq_open failed" << '\n';
	}

	if(mq_send(recvr_msgq_fd, buffer, strlen(buffer) + 1, 0) == -1)
	{
		std::cout << "Client: Not able to send message to sever" << '\n';
	}

	// recvr_msgq_fd = mq_open(srvMsg, O_WRONLY);

	mq_close(recvr_msgq_fd);
}