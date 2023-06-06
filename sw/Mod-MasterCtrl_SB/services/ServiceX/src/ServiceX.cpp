#include "ServiceX.h"


Services::ServiceX::ServiceX(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::cout << "Services::ServiceX constructor called!" << '\n';
}


Services::ServiceX::~ServiceX()
{
	std::cout << "Services::ServiceX destructor called!" << '\n';
}


const std::string& Services::ServiceX::getName()
{
	// std::cout << "Services::ServiceX preInit() called!" << '\n';

	return m_name;
}


void Services::ServiceX::preInit()
{
	// Get params from DB for this instance
	// std::cout << "ServiceX preInit() called!" << '\n';
}


void Services::ServiceX::postInit()
{

}


void Services::foo(boVector v)
{

};


Services::boVector Services::createBoVector()
{
	Services::boVector boVec(10);

	return boVec;
}