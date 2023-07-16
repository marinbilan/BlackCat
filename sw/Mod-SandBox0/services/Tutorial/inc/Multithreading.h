#pragma once
#include "TutorialIf.h"

//#include <iostream>
//#include <string>


namespace Services
{

class Multithreading : public TutorialIf
{
public:
	Multithreading(const std::string& dbPath, const std::string& name);

	~Multithreading();

	const std::string& getName();

	void preInit();

	void postInit();


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;
};

} // End of namespace Services