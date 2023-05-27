
#include "BrokerSrvIf.h"
#include "BrokerSrv.h"


int main()
{

	std::cout << "[MasterSrv][InvDev] Services::BrokerSrv constructor called!" << '\n';

	Services::BrokerSrv brokerSrv("BrokerSrv", "BrokerSrv");
	brokerSrv.preInit();


	return 0;
}