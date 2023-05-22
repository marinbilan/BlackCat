#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <math.h>


namespace Services
{

class InitSrvIf
{
public:
	virtual ~InitSrvIf()
	{
	}

	virtual const std::string& getName() = 0;

	virtual void preInit() = 0;
	
	virtual void postInit() = 0;

private:
};

} // End of namespace