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


#include "ClientServerSrv.h"
#include "Factory.h"

#define SOCKET_NAME "/tmp/DemoSocket"
// 128 bytes
#define BUFFER_SIZE 128


Services::ClientServerSrv::ClientServerSrv(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::cout << "Services::ClientServerSrv constructor called!" << '\n';
}


Services::ClientServerSrv::~ClientServerSrv()
{
	std::cout << "Services::ClientServerSrv destructor called!" << '\n';
}


const std::string& Services::ClientServerSrv::getName()
{
	// std::cout << "Services::ClientServerSrv preInit() called!" << '\n';

	return m_name;
}


void Services::ClientServerSrv::preInit()
{
	// Get params from DB for this instance
	// std::cout << "ClientServerSrv preInit() called!" << '\n';

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


	
	Common::Factory::Factory::getInstance().getMasterSrv()->preInit();



	// ---- 
	/*
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
	*/

	// Close socket
	close(dataSocket);
}


void Services::ClientServerSrv::postInit()
{

}