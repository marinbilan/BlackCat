#include "Service0.h"


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