#pragma once
#include "BrokerSrvIf.h"



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


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;
};

} // End of namespace Services