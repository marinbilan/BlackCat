#include "Multithreading.h"


Services::Multithreading::Multithreading(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::cout << "Services::Multithreading constructor called!" << '\n';
}


Services::Multithreading::~Multithreading()
{
	std::cout << "Services::Multithreading destructor called!" << '\n';
}


const std::string& Services::Multithreading::getName()
{
	// std::cout << "Services::Multithreading preInit() called!" << '\n';

	return m_name;
}


void Services::Multithreading::preInit()
{
	// Get params from DB for this instance
	std::cout << "----> Multithreading preInit() called!" << '\n';
}


void Services::Multithreading::postInit()
{

}