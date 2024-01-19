#include "MasterSrv.h"
#include "Factory.h"



Services::MasterSrv::MasterSrv(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "\\")
{
	std::string trace = "[MB][MasterSrv][InvDev] Services::MasterSrv constructor";
	Common::Factory::Factory::getInstance().getClientServerSrv()->TRACE(trace);

	trace = "[MB][MasterSrv][InvDev] Services::MasterSrv name: " + m_name;
	Common::Factory::Factory::getInstance().getClientServerSrv()->TRACE(trace);
}


Services::MasterSrv::~MasterSrv()
{
	std::cout << "Services::MasterSrv destructor called!" << '\n';
}


const std::string& Services::MasterSrv::getName()
{
	// std::cout << "Services::MasterSrv preInit() called!" << '\n';

	return m_name;
}


void Services::MasterSrv::preInit()
{
	std::cout << "[MB][MasterSrv][InvDev] Services::MasterSrv preInit()" << '\n';

	m_objectsManager = Common::Factory::Factory::getInstance().getObjectsManager();
	m_invDevIfVec = m_objectsManager->getInvDevIfVec();

	for(auto s : m_invDevIfVec)
	{
		// PROCESS DATA FOR EACH INSTANCE (DEVICE)
		s->collectData();
		// std::cout << "xxx before" << '\n';
		s->calculateData();
		// std::cout << "xxx after 1" << '\n';
		s->storeData();  // In DB
		// std::cout << "xxx after 2" << '\n';

		// POSTPROCESS DATA FOR EACH INSTANCE (DEVICE)
		s->sortStocksByYearsToReturnDebt();
		s->printStocksByYearsToReturnDebt();
		
	}
}


void Services::MasterSrv::postInit()
{

}