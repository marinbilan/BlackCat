#pragma once
#include "ServiceIf.h"

//#include <iostream>
//#include <string>


namespace Services
{

class ServiceX : public ServiceIf
{
public:
	ServiceX(const std::string& dbPath, const std::string& name);

	~ServiceX();

	const std::string& getName();

	void preInit();

	void postInit();


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;
};

} // End of namespace Services