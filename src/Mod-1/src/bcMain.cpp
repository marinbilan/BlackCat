#include <iostream>

#include "ServiceIf.h"
#include "Service0.h"

int main() 
{

	Service::Service0 srv0_0("dbPath", "instName");
	std::cout << "Srv_0 name: " << srv0_0.getName() << '\n';

	return 0;
}