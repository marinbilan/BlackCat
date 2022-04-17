#pragma once
#include <iostream>
#include <string>
// #include "FactoryIf.h"


namespace Service
{

class ServiceBase
{
public:
	ServiceBase(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
	{
	}

	virtual ~ServiceBase()
	{
	}

	virtual const std::string& getName()
	{
		return m_name;
	}

	virtual void preInit() {};
	virtual void postInit() {};

	// Call Factory singleton here
	// If this was pure virtual method, ServiceBase would not be 
	// able to create instance
	virtual void testVirtualMethod() {};

private:
std::string returnErrorStr = "Error: ServiceIf::getName() called from ServiceIf!";
std::string m_dbPath;
std::string m_dbPathWithName;
std::string m_name;
};

} // End of namespace