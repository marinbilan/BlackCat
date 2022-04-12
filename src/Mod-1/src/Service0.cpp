#include "Service0.h"
#include "Factory.h"
#include "FactoryImpl.h"


Service::Service0::Service0(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
}


Service::Service0::~Service0()
{
}


void Service::Service0::preInit()
{
	// Get params from DB for this instance
	std::cout << "Service0 preInit() called!" << '\n';
}


void Service::Service0::postInit()
{

}


void Service::Service0::testFactory()
{
	Common::Factory::getInstance().testFactoryPrint();
}


void Service::Service0::setFactoryImplPtr(Common::FactoryIf& factoryPtr)
{
	m_factoryImplSingleton = &factoryPtr;
}


void Service::Service0::testFactoryImpl()
{
	m_factoryImplSingleton->doSomething();
}


void Service::Service0::setService(ServiceIf* srvIfPtr)
{
	// Call some method via ServiceIf
}