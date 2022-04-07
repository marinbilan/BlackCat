// #pragma once
#include "Service0.h"


Service::Service0::Service0(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
}


Service::Service0::~Service0()
{
}


void Service::Service0::preInit()
{
	// Get params from DB for this instance
}


void Service::Service0::postInit()
{

}
