#include <iostream>


#include "NetworkingIf.h"
#include "HTTPSProxySrv.h"
#include "HTTPSClient.h"

#include "ServiceIf.h"
#include "ServiceX.h"

#include <memory>

int main()
{
	std::cout << "This is main(). Mod App 2" << '\n';

	Services::ServiceX srvX("Test", "Test");
	std::cout << "Srv name: " << srvX.getName() << '\n';

	// ServiceX sX("test", "test");
	// sX.preInit();

	std::unique_ptr<Services::ServiceX> srvIf = std::make_unique<Services::ServiceX>("Test", "Test");
	srvIf->getName();

	Service::HTTPSProxySrv httpsProxySrvTemp("Test", "Test");

	return 0;
}