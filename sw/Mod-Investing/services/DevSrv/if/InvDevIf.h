#pragma once
#include <string>



namespace Services
{

class InvDevIf
{
public:
	virtual ~InvDevIf()
	{
	}

	virtual const std::string& getName() = 0;

	virtual void preInit() = 0;
	
	virtual void postInit() = 0;

	//
	virtual void collectData() = 0;

	virtual void calculateData() = 0;

	virtual void storeData() = 0;

private:
};

} // End of namespace