#pragma once
#include "NetworkingIf.h"


namespace Networking
{

class HTTPClient : public ClientIf
{
public:

	~HTTPClient();

	const std::string& getName()
	{
		return m_name;
	}

	void preInit();
	void postInit();

	// Dummy test method
	void doSomething();

private:

};

}