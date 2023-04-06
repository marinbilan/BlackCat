#include "SrvLinuxSys.h"


Services::SrvLinuxSys::SrvLinuxSys(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::cout << "Services::SrvLinuxSys constructor called!" << '\n';
}


Services::SrvLinuxSys::~SrvLinuxSys()
{
	std::cout << "Services::SrvLinuxSys destructor called!" << '\n';
}


const std::string& Services::SrvLinuxSys::getName()
{
	// std::cout << "Services::SrvLinuxSys preInit() called!" << '\n';

	return m_name;
}


void Services::SrvLinuxSys::preInit()
{
	// Get params from DB for this instance
	// std::cout << "SrvLinuxSys preInit() called!" << '\n';
}


void Services::SrvLinuxSys::postInit()
{

}