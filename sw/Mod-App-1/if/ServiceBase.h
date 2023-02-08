#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <map>
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

	/*
	template<typename FileCallback>
	void readAllFiles() {};
	*/

	virtual void initAllConfs() {};

	virtual std::optional<std::map<std::string,int>::iterator> getIterConfigOpt() {};

	virtual std::map<std::string,int>::iterator getIterConfigNoOpt(std::pair<std::string, int> const& desiredConfig) {};

	virtual std::optional<std::reference_wrapper<int>> getConfigFromMap(std::pair<std::string, int> const& desiredConfig) {};

private:
std::string returnErrorStr = "Error: ServiceIf::getName() called from ServiceIf!";
std::string m_dbPath;
std::string m_dbPathWithName;
std::string m_name;
};

} // End of namespace