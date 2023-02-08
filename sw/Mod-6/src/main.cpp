#include <iostream>
#include "ServiceIf.h"
#include "Service0.h"

int main()
{
	std::cout << "Hello..." <<  '\n';

	Service::Service0 service0_0("Test", "Test");
	service0_0.preInit();
}