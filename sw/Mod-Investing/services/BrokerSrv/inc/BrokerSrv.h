#pragma once
#include "BrokerSrvIf.h"

#include "MasterSrvIf.h"
#include "ClientServerSrvIf.h"

namespace Services
{

class BrokerSrv : public BrokerSrvIf
{
public:
	BrokerSrv(const std::string& dbPath, const std::string& name);

	~BrokerSrv();

	const std::string& getName();

	void preInit();

	void postInit();

	void runClientServer();


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;

// Master Service
std::shared_ptr<Services::MasterSrvIf> m_masterSrv;
std::shared_ptr<Services::ClientServerSrvIf> m_clientServerSrv;
};

} // End of namespace Services