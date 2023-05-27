#pragma once
#include "ClientServerSrvIf.h"



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


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;
};

} // End of namespace Services