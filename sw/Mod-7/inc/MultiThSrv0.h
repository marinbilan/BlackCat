#pragma once
#include "ServiceIf.h"


// General Multithreaded class
namespace Service
{

class MultiThSrv0 : public ServiceIf
{
public:
	MultiThSrv0(const std::string& dbPath, const std::string& name);
	~MultiThSrv0();

	const std::string& getName()
	{
		return m_name;
	}

	void preInit();
	void postInit();

private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;
};

} // End of namespace Service

