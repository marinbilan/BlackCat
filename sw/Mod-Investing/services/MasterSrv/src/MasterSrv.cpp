#include "MasterSrv.h"
#include "Factory.h"



Services::MasterSrv::MasterSrv(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "\\")
{
	std::cout << "Services::MasterSrv constructor called!" << '\n';
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

	m_objectsManager = Common::Factory::Factory::getInstance().getObjectsManager();

	m_invDevIfVec = m_objectsManager->getInvDevIfVec();

	// std::cout << "MasterSrv dev vec size: " << m_invDevIfVec.size() << '\n';

	for(auto s : m_invDevIfVec)
	{
		std::cout << "Dev: " << s->getName() << '\n';
	}
}


void Services::MasterSrv::postInit()
{

}