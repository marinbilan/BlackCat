#pragma once
#include "MasterSrvIf.h"

//#include <iostream>
//#include <string>


namespace Services
{

class MasterSrv : public MasterSrvIf
{
public:
	MasterSrv(const std::string& dbPath, const std::string& name);

	~MasterSrv();

	const std::string& getName();

	void preInit();

	void postInit();


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;
};

} // End of namespace Services