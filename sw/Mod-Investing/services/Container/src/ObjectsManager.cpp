#include "ObjectsManager.h"


Services::ObjectsManager::ObjectsManager(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::cout << "Services::ObjectsManager constructor called!" << '\n';
}


Services::ObjectsManager::~ObjectsManager()
{
	std::cout << "Services::ObjectsManager destructor called!" << '\n';
}


const std::string& Services::ObjectsManager::getName()
{
	// std::cout << "Services::ObjectsManager preInit() called!" << '\n';

	return m_name;
}


void Services::ObjectsManager::preInit()
{
	// Get params from DB for this instance
	// std::cout << "ObjectsManager preInit() called!" << '\n';
}


void Services::ObjectsManager::postInit()
{

}


// Devs
void Services::ObjectsManager::setInvDevIf(const std::shared_ptr<Services::InvDevIf>& invDevIf)
{

	m_invDevIfVec.push_back(invDevIf);
}


std::vector<std::shared_ptr<Services::InvDevIf>>& Services::ObjectsManager::getInvDevIfVec()
{

	return m_invDevIfVec;
}



void Services::ObjectsManager::setServiceIf(const std::shared_ptr<Services::ServiceIf>& serviceIf)
{

	m_serviceIfVec.push_back(serviceIf);
}


std::shared_ptr<Services::ServiceIf> Services::ObjectsManager::getServiceIf(const std::string& instName)
{

	for (auto it = m_serviceIfVec.begin(); it != m_serviceIfVec.end(); ++it)
	{	
		if (!(*it)->getName().compare(instName))
		{
			return *it;
		}
		
	}

	return nullptr;
}