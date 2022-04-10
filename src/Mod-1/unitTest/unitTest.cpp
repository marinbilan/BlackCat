#include <gmock/gmock.h>


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

	myClassTest()
	{
		m_myClass = new myClass(11);
	}

	~myClassTest()
	{
		delete m_myClass;
	}
};


TEST_F(myClassTest, firstmyClassTest)
{
	// myClass c(10);
	m_myClass->m_value;
	// EXPECT_EQ(11, myClass->m_value);
}


/*
int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
*/