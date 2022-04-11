#include <gmock/gmock.h>
#include <memory>
#include "ServiceIf.h"
#include "Service0.h"


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


/*
int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
*/