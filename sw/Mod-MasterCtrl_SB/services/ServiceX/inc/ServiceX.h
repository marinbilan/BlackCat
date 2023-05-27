#pragma once
#include "ServiceIf.h"



namespace Services
{

class ServiceX : public ServiceIf
{
public:
	ServiceX(const std::string& dbPath, const std::string& name);

	~ServiceX();

	const std::string& getName();

	void preInit();

	void postInit();


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;
};


// Modern C++ 11 (Bo Qian) - YouTube
class Widget
{
public:
	Widget(int id) : m_id(id)
	{
		std::cout << "Widget created!" << '\n';
	}

private:
int m_id;
};


} // End of namespace Services