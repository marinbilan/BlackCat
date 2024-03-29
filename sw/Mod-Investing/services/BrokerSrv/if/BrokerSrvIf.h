#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>



namespace Services
{

class BrokerSrvIf
{
public:
	virtual ~BrokerSrvIf()
	{
	}

	virtual const std::string& getName() = 0;

	virtual void preInit() = 0;
	
	virtual void postInit() = 0;

	virtual void runClientServer() = 0;

private:
};

} // End of namespace