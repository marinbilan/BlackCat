#pragma once
#include <iostream>
#include <string>


class ServiceX
{
public:
	ServiceX(const std::string& dbPath, const std::string& name);
	~ServiceX();

	void preInit();
	void postInit();

private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;
};