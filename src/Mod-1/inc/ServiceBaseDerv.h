#pragma once
#include "ServiceBase.h"


namespace Service
{

class ServiceBaseDerv : public ServiceBase
{
public:
	ServiceBaseDerv(const std::string& dbPath, const std::string& name);
	~ServiceBaseDerv();

	const std::string& getName()
	{
		return m_name;
	}

	void preInit();
	void postInit();

	// Call Factory singleton here
	void testVirtualMethod();

private:
// std::string m_dbPath;
// std::string m_dbPathWithName;
std::string m_name;

// Common::FactoryIf* m_factoryImplSingleton;
};

}