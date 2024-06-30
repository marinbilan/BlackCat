#pragma once
#include <iostream>
#include <string>



namespace Services
{

class MasterSrvIf
{
public:
	virtual ~MasterSrvIf()
	{
	}

	virtual const std::string& getName() = 0;

	virtual void preInit() = 0;
	
	virtual void postInit() = 0;

	virtual void cmdSecondWord(const std::string& cmdLine) = 0;

private:
};

} // End of Namespace Services