
#include "BrokerSrvIf.h"
#include "BrokerSrv.h"


int main()
{

	Services::BrokerSrv brokerSrv("BrokerSrv", "BrokerSrv");
	brokerSrv.preInit();


	return 0;
}