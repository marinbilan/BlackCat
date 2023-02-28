#include <iostream>

#include "Factory.h"
#include "Database.h"

//#include "NetworkingIf.h"
//#include "HTTPSProxySrv.h"
//#include "HTTPSClient.h"

//#include "ServiceIf.h"
//#include "ServiceX.h"
#include "ContainerIf.h"
#include "ObjectsManager.h"

#include "InitSrvIf.h"
#include "InitSrv.h"

#include <memory>


int main()
{

	Services::InitSrv initSrvInst("Test", "Test");
	initSrvInst.preInit();



	// GET THIS INFO FROM CONFIG FILE (Xml)



	// 1] DATABASE - Create DB and store it in Factory
	//std::unique_ptr<Common::Database> database = std::make_unique<Common::Database>("Database/database_0.txt");

	// 2] OBJECTs MANAGER - Create Object Manager and store it in Factory
	//std::shared_ptr<Services::ContainerIf> objContainerIf = std::make_shared<Services::ObjectsManager>("Test", "Test");


	// Set Database
	//Common::Factory::getInstance().setDatabase(database);
	// Set Objects Manager
	//Common::Factory::getInstance().setObjectsManager(objContainerIf);


	// Move this to Factory init method
	//Common::Factory::getInstance().registerClassConstructor();
	//Common::Factory::getInstance().getInterfacesAndCreateInstances();


	// GET SOME INSTANCEs FROM OBJECTs MANGER
	//std::string instanceStr("serviceX_0");
	//std::shared_ptr<Services::ServiceIf> serviceIfPtr = objContainerIf->getServiceIf(instanceStr);

	/*
	if(serviceIfPtr == nullptr) 
	{
		std::cout << "Unable to get instance " << instanceStr << '\n';
	}

	std::cout << "Found instance name: " << serviceIfPtr->getName() << '\n';
	*/

	// objContainerIf->getServiceIf()->getName();
	// std::cout << "Main test name: " << serviceIfPtr->getName() << '\n';

	/*
	std::cout << "This is main(). Mod App 2" << '\n';

	Services::ServiceX srvX("Test", "Test");
	std::cout << "Srv name: " << srvX.getName() << '\n';
	*/

	// ServiceX sX("test", "test");
	// sX.preInit();

	/*
	std::unique_ptr<Services::ServiceX> srvIf = std::make_unique<Services::ServiceX>("Test", "Test");
	srvIf->getName();

	Service::HTTPSProxySrv httpsProxySrvTemp("Test", "Test");
	*/

	return 0;
}