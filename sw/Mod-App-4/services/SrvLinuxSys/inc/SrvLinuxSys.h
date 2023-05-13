#pragma once
#include "SrvLinuxSysIf.h"

#include <iostream>
#include <map>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


#define SOCKET_NAME "/tmp/DemoSocket"
// 128 bytes
#define BUFFER_SIZE 128
// 31 Clients (descriptors) and one Master descriptor
#define MAX_CLIENT_SUPPORTED 32


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

	//

	// initialize monitor fd_set

	void addToMonitoredFdSetMap(int sktFd)
	{
		// Insert in map
		m_MonitoredFdSetMap.insert({sktFd, "TestClient"});
	}

	void removeFromMonitoredFdSetMap(int sktFd)
	{
		// Insert in map
		m_MonitoredFdSetMap.erase(sktFd);
	}

	/*
	Clone all FDs in m_MonitoredFdSet array into fd_set data structure
	fd_set - standard C structure
	Remove all fd's from fd_set and copy the FD's to m_MonitoredFdSet structure
	Refresh the standard fd_set structure
	*/
	void refreshFdSet(fd_set* fd_set_ptr)
	{
		FD_ZERO(fd_set_ptr);

		for(auto const& [key, value] : m_MonitoredFdSetMap)
		{
			std::cout << "Key: " << key << " Value: " << value << '\n';
			FD_SET(key, fd_set_ptr);
		}
	}

	int getMaxFd()
	{
		// Get last element from map
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