#pragma once
#include <vector>
#include "ServiceIf.h"


namespace Service
{

class HTTPSProxySrv : public ServiceIf
{
public:
	HTTPSProxySrv(const std::string& dbPath, const std::string& name);
	~HTTPSProxySrv();

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


	// --== HTTPS Access ==--
	// INCOME STATEMENT
	bool getFromIncomeStatement(const std::string& stockTicker,
		std::vector<double>& revenueVec, 
		std::vector<double>& grossProfitVec,
		std::vector<double>& netIncomeVec);

private:
std::string m_dbPath;
std::string m_dbPathWithName;
std::string m_name;

Common::FactoryIf* m_factoryImplSingleton;
};

}


