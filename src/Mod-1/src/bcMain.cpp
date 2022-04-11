#include <iostream>

#include "FactoryIf.h"
#include "Factory.h"
// #include "FactoryImpl.h"

//#include "ServiceIf.h"
//#include "Service0.h"


int main()
{
	// Create Factory
	Common::Factory::getInstance();

	// Move to Unit Test
	Service::Service0 srv0_0("dbPath", "instName");
	
	// Test Factroy
	srv0_0.testFactory();

	// Set FactroyImpl
	srv0_0.setFactoryImplPtr(Common::FactoryIf::getInstance());

	// Test FactoryImpl
	srv0_0.testFactoryImpl();

	return 0;
}