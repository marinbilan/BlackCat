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


	return 0;
}