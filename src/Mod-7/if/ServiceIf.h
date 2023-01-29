#pragma once
#include <iostream>
#include <string>
#include <memory>


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

private:
	std::string returnErrorStr = "Error: ServiceIf::getName() called from ServiceIf!";
};

} // End of namespace


