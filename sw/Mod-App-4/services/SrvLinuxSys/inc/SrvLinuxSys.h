#pragma once
#include "SrvLinuxSysIf.h"

#include <iostream>

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


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;
};

} // End of namespace Services