#include "InvDev0.h"


Services::InvDev::InvDev(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::cout << "Services::InvDev constructor called!" << '\n';
}


Services::InvDev::~InvDev()
{
	std::cout << "Services::InvDev destructor called!" << '\n';
}


const std::string& Services::InvDev::getName()
{
	// std::cout << "Services::InvDev preInit() called!" << '\n';

	return m_name;
}


void Services::InvDev::preInit()
{
	// Get params from DB for this instance
	// std::cout << "InvDev preInit() called!" << '\n';
}


void Services::InvDev::postInit()
{

}