#pragma once
#include "ContainerIf.h"



namespace Services
{

class ObjectsManager : public ContainerIf
{
public:
	ObjectsManager(const std::string& dbPath, const std::string& name);

	~ObjectsManager();

	const std::string& getName();

	void preInit();

	void postInit();

	// Dev
	void setInvDevIf(const std::shared_ptr<Services::InvDevIf>& invDevIf);

	std::vector<std::shared_ptr<Services::InvDevIf>>& getInvDevIfVec();

	// 
	void setServiceIf(const std::shared_ptr<Services::ServiceIf>& serviceIf);

	std::shared_ptr<Services::ServiceIf> getServiceIf(const std::string& instName);


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;

std::vector<std::shared_ptr<Services::InvDevIf>> m_invDevIfVec;
std::vector<std::shared_ptr<Services::ServiceIf>> m_serviceIfVec;
};

} // End of namespace Services