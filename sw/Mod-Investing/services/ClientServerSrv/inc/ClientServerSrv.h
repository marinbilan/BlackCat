#pragma once
#include "ClientServerSrvIf.h"

/*
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
*/


// #define SOCKET_NAME "/tmp/DemoSocket"
// 128 bytes
// #define BUFFER_SIZE 128


namespace Services
{

class ClientServerSrv : public ClientServerSrvIf
{
public:
	ClientServerSrv(const std::string& dbPath, const std::string& name);

	~ClientServerSrv();

	const std::string& getName();

	void preInit();

	void postInit();

	void TRACE(const std::string& msg);

	void sendMsg(const std::string& msg);


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;

struct sockaddr_un m_addr;

int m_dataSocket;
char m_buffer[BUFFER_SIZE];  // Not used for now
};

} // End of namespace Services