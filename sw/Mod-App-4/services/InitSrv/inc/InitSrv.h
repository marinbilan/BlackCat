#pragma once
#include "InitSrvIf.h"



namespace Services
{

class InitSrv : public InitSrvIf
{
public:
	InitSrv(const std::string& dbPath, const std::string& name);

	~InitSrv();

	const std::string& getName();

	void preInit();

	void postInit();


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;
};

} // End of namespace Services