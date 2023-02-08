#include "MultiThSrv0.h"


Service::MultiThSrv0::MultiThSrv0(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
}


Service::MultiThSrv0::~MultiThSrv0()
{
}


void Service::MultiThSrv0::preInit()
{
	// Get params from DB for this instance
	std::cout << "MultiThSrv0 preInit() called!" << '\n';
}


void Service::MultiThSrv0::postInit()
{

}