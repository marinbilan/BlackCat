#include <algorithm>
#include "InitSrv.h"

#include "SrvLinuxSysIf.h"
#include "SrvLinuxSys.h"
#include "HTTPSProxySrvIf.h"
#include "HTTPSProxySrv.h"


Services::InitSrv::InitSrv(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::cout << "Services::InitSrv constructor called!" << '\n';
}


Services::InitSrv::~InitSrv()
{
	std::cout << "Services::InitSrv destructor called!" << '\n';
}


const std::string& Services::InitSrv::getName()
{
	

	return m_name;
}



void Services::InitSrv::preInit()
{
	std::cout << "Services::InitSrv preInit() called!" << '\n';

	SrvLinuxSys testSys("test", "test");
	testSys.preInit();

}


void Services::InitSrv::postInit()
{

}