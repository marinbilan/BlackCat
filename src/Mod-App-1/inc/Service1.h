#pragma once
#include "ServiceIf.h"


namespace Service
{

class Service1 : public ServiceIf
{
public:
	Service1(const std::string& dbPath, const std::string& name);
	~Service1();

	const std::string& getName()
	{
		return m_name;
	}

	void preInit();
	void postInit();

	// Call Factory singleton here
	void testFactory();

	// Unit Test setter
	void setFactoryImplPtr(Common::FactoryIf& factoryPtr);

	// Call FactoryImpl singleton here
	void testFactoryImpl();

	void setService(ServiceIf* srvIfPtr);

private:
std::string m_dbPath;
std::string m_dbPathWithName;
std::string m_name;

Common::FactoryIf* m_factoryImplSingleton;
};

}