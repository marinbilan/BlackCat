#include "ServiceBaseDerv.h"
// #include "Factory.h"
// #include "FactoryImpl.h"


Service::ServiceBaseDerv::ServiceBaseDerv(const std::string& dbPath, const std::string& name) : 
m_name(name),
ServiceBase(dbPath, name)
{
}


Service::ServiceBaseDerv::~ServiceBaseDerv()
{
}


void Service::ServiceBaseDerv::preInit()
{
	// Get params from DB for this instance
	std::cout << "ServiceBaseDerv preInit() called!" << '\n';
}


void Service::ServiceBaseDerv::postInit()
{

}


void Service::ServiceBaseDerv::testVirtualMethod()
{
	std::cout << "Call from ServiceBaseDerv" << '\n';
}