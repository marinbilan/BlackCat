#pragma once
#include <iostream>
#include <string>
#include <thread>
#include "FactoryIf.h"


namespace Service
{

class ServiceIf
{
public:
	~ServiceIf()
	{
	}

	virtual const std::string& getName()
	{
		return returnErrorStr;
	}

	virtual void preInit() {};
	virtual void postInit() {};

	// Call Factory singleton here
	virtual void testFactory() = 0;

	// Unit Test Factory setter
	virtual void setFactoryImplPtr(Common::FactoryIf& factoryPtr) = 0;

	// Call FactoryImpl singleton here
	virtual void testFactoryImpl() = 0;

	virtual void setService(ServiceIf* srvIfPtr) = 0;
	// virtual void postInit() {};

private:
	std::string returnErrorStr = "Error: ServiceIf::getName() called from ServiceIf!";
};

} // End of namespace