#include "MasterSrv.h"


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
	// Get params from DB for this instance
	// std::cout << "MasterSrv preInit() called!" << '\n';
}


void Services::MasterSrv::postInit()
{

}