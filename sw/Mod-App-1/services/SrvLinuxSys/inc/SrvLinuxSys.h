#pragma once
#include "SrvLinuxSysIf.h"



namespace Services
{

class SrvLinuxSys : public SrvLinuxSysIf
{
public:
	SrvLinuxSys(const std::string& dbPath, const std::string& name);

	~SrvLinuxSys();

	const std::string& getName();

	void preInit();

	void postInit();


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;
};

} // End of namespace Services