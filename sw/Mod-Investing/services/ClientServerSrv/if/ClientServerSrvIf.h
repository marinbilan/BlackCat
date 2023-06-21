#pragma once
#include <iostream>
#include <string>



namespace Services
{

class ClientServerSrvIf
{
public:
	virtual ~ClientServerSrvIf()
	{
	}

	virtual const std::string& getName() = 0;

	virtual void preInit() = 0;
	
	virtual void postInit() = 0;

	virtual void TRACE(const std::string& msg) = 0;

	virtual void sendMsg(const std::string& msg) = 0;

private:
};

} // End of namespace