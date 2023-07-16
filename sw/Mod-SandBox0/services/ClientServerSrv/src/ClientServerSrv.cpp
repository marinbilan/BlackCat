#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ClientServerSrv.h"
#include "Factory.h"



Services::ClientServerSrv::ClientServerSrv(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "/")
{
	std::cout << "[MasterSrv][InvDev] Services::ClientServerSrv constructor" << '\n';
}


Services::ClientServerSrv::~ClientServerSrv()
{
	std::cout << "[MasterSrv][InvDev] Services::ClientServerSrv destructor" << '\n';

	// Close socket
	close(m_dataSocket);
}


const std::string& Services::ClientServerSrv::getName()
{
	return m_name;
}


void Services::ClientServerSrv::preInit()
{
	// Get params from DB for this instance
	std::cout << "[MasterSrv][InvDev] Services::ClientServerSrv preInit()" << '\n';

	int ret;

	// [1st STEP] Create Socket
	m_dataSocket = socket(AF_UNIX, SOCK_STREAM, 0);

	// Error handling
	if(m_dataSocket == -1)
	{
		std::cout << "[ERROR] Socket creation failed" << '\n';
	}
	std::cout << "[INFO] [1st STEP] Socket creation OK" << '\n';

	// Define connection (server) socket name - Same as Server side
	memset(&m_addr, 0, sizeof(struct sockaddr_un));
	// Specify the socket credentials
	m_addr.sun_family = AF_UNIX;
	strncpy(m_addr.sun_path, SOCKET_NAME, sizeof(m_addr.sun_path) - 1);

	// Send CIR to server - from client (data socket)
	ret = connect(m_dataSocket, (const struct sockaddr*) &m_addr, sizeof(struct sockaddr_un));

	// Error handling
	if(ret == -1)
	{
		std::cout << "[ERROR] Unable to connect. Server down" << '\n';
	}
	std::cout << "[INFO] [2nd STEP] Connect OK" << '\n';




	// After connection - Server is waiting for info
	// SEND CLIENT INFO TO SERVER - Get Data
	std::string clientInfo("xxx [Master] [Dev1]");
	sendMsg(clientInfo);
	std::cout << "[INFO] [3th STEP] Client info sent OK" << '\n';


	
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

}


void Services::ClientServerSrv::postInit()
{

}


void Services::ClientServerSrv::TRACE(const std::string& msg)
{
	send(m_dataSocket, msg.c_str(), strlen(msg.c_str()), 0);
}


void Services::ClientServerSrv::sendMsg(const std::string& msg)
{
	send(m_dataSocket, msg.c_str(), strlen(msg.c_str()), 0);
}
