#include "ServiceX.h"


ServiceX::ServiceX(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
}


ServiceX::~ServiceX()
{
}


void ServiceX::preInit()
{
	// Get params from DB for this instance
	std::cout << "ServiceX preInit() called!" << '\n';
}


void ServiceX::postInit()
{

}