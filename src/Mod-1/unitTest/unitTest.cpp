#include <gmock/gmock.h>
#include <memory>
#include "ServiceIf.h"
#include "Service0.h"
#include "Service1.h"
#include "mockService0.h"


// using ::testing::AtLeast;


// Test class - remove this
class myClass
{
public:
	myClass(int value) : m_value(value) {};
	int m_value;
};


// TEST FIXTURE //
class myClassTest : public ::testing::Test
{
public:
myClass* m_myClass;
Service::ServiceIf* m_srvTestPtr;

	myClassTest()
	{
		m_myClass = new myClass(11);

		m_srvTestPtr = new Service::Service0("testDbPath", "instName");
	}

	~myClassTest()
	{
		delete m_myClass;
		delete m_srvTestPtr;
	}
};


TEST_F(myClassTest, firstmyClassTest)
{
	// myClass c(10);
	m_myClass->m_value;
	m_srvTestPtr->preInit();
	// EXPECT_EQ(11, myClass->m_value);
}


TEST_F(myClassTest, FactoryUnitTest)
{
	// Set FactoryImpl (not mock for now) (Dependency injection)
	m_srvTestPtr->setFactoryImplPtr(Common::FactoryIf::getInstance());

	// Call method which invoke FactoryImpl method
	m_srvTestPtr->testFactoryImpl();

}


TEST_F(myClassTest, mockUnitTest)
{
	// Create mock
	MockService0 mockService0_0;

	// Dependency inject mock object
	Service::ServiceIf* m_srvTestPtr1_0 = new Service::Service1("testDbPath", "instName");

	// Expect call on mock object in tested method
	EXPECT_CALL(mockService0_0, preInit()).Times(testing::AtLeast(1));

	// Tested method on actual object
	m_srvTestPtr1_0->setService(&mockService0_0);

}

/*
int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
*/