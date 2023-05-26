
#include "BrokerSrvIf.h"
#include "BrokerSrv.h"


int main()
{

	Services::BrokerSrv initSrvInst("Test", "Test");
	initSrvInst.preInit();


	return 0;
}