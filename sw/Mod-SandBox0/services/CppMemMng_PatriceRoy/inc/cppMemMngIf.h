#pragma once
#include <iostream>
#include <string>


namespace CppMemMgr
{

class CppMemMgrIf
{
public:
	virtual ~CppMemMgrIf()
	{
	}

	//virtual const std::string& getName() = 0;

	//virtual void preInit() = 0;	
	//virtual void postInit() = 0;

private:
};

} // End of namespace