#pragma once
#include <iostream>
#include <string>


namespace Services
{

class TutorialIf
{
public:
	virtual ~TutorialIf()
	{
	}

	virtual const std::string& getName() = 0;

	virtual void preInit() = 0;
	
	virtual void postInit() = 0;

private:
};

} // End of namespace