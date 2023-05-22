#pragma once
#include "SrvLinuxSysIf.h"

#include <iostream>
#include <map>
#include <regex>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#define SOCKET_NAME "/tmp/DemoSocket"
// 128 bytes
#define BUFFER_SIZE 128



namespace Services
{

class SrvLinuxSys : public SrvLinuxSysIf
{
public:
	SrvLinuxSys(const std::string& dbPath, const std::string& name);

	~SrvLinuxSys();

	const std::string& getName();

	void preInit();

	void postInit();

	// RECEIVE AND PROCESS INCOMMING MESSAGE FROM CLIENT
	void receiveMessage(int commSocketFdId, const std::string& msg);

	void printFDSetMap()
	{
		for(auto it : m_MonitoredFdSetMap)
		{
			std::cout << "clientFDId: " << it.first << " Client info: " << it.second << '\n';
		}
	}


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;

/*
An map of active file descriptors which the server process is maintaining in order
to talk with the connected clients. Master FD is also member of this array.
Master FD plus all client's file descriptors
*/
std::map<int, std::string> m_MonitoredFdSetMap;
};

} // End of namespace Services