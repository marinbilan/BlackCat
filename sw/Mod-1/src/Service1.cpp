#include "Service1.h"
#include "Factory.h"
// #include "FactoryImpl.h"


Service::Service1::Service1(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
}


Service::Service1::~Service1()
{
}


void Service::Service1::preInit()
{
	// Get params from DB for this instance
	std::cout << "Service1 preInit() called!" << '\n';
}


void Service::Service1::postInit()
{

}


void Service::Service1::testFactory()
{
	Common::Factory::getInstance().testFactoryPrint();
}


void Service::Service1::setFactoryImplPtr(Common::FactoryIf& factoryPtr)
{
	m_factoryImplSingleton = &factoryPtr;
}


void Service::Service1::testFactoryImpl()
{
	m_factoryImplSingleton->doSomething();
}


void Service::Service1::setService(ServiceIf* srvIfPtr)
{
	// Call some method via ServiceIf
	srvIfPtr->preInit();
}