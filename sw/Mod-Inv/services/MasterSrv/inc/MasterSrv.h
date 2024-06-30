#pragma once
#include "MasterSrvIf.h"

#include "ContainerIf.h"
#include "InvDevIf.h"



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

	void cmdSecondWord(const std::string& cmdLine);


private:

std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;

// Objects Manager
std::shared_ptr<Services::ContainerIf> m_objectsManager;
// Devs
std::vector<std::shared_ptr<Services::InvDevIf>> m_invDevIfVec;
};

} // End of namespace Services