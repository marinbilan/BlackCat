#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <vector>


namespace Service
{

class HTTPSProxySrvIf
{
public:
	~HTTPSProxySrvIf()
	{
	}

	virtual const std::string& getName()
	{
		return returnErrorStr;
	}

	virtual void preInit() {};
	virtual void postInit() {};

private:
	std::string returnErrorStr = "Error: HTTPSProxySrvIf::getName() called from HTTPSProxySrvIf!";
};

} // End of namespace