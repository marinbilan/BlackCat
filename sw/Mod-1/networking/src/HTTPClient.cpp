#include "HTTPClient.h"


Networking::HTTPClient::~HTTPClient()
{
}


void Networking::HTTPClient::preInit()
{
	// Get params from DB for this instance
	std::cout << "HTTPClient preInit() called!" << '\n';
}


void Networking::HTTPClient::postInit()
{

}