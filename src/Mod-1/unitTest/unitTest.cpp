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


class Widget
{
public:
	Widget(int id) : m_id(id) {}

	int getId() const
	{
		return m_id;
	}

private:
int m_id;
};


TEST_F(myClassTest, TestVectorIterators)
{
	// ---- Basic iterator operations ----
	std::vector<int> vecOfInt = {0, 2, 16, 8, 11};

	std::vector<int>::iterator it0 = vecOfInt.begin();
	for(; it0 != vecOfInt.end(); it0++)
	{
		// Iterator address is always the same - only value is increased
		std::cout << "it0 address: " << &it0 << " vec elem address: " << &(*it0) << " vec elem value: " << *it0 << '\n';
	}

	// Return iterator on first element greather than 10
	// it1 will have own address, but underlying elem of vector will have the same address and value
	std::vector<int>::iterator it1 = std::find_if(vecOfInt.begin(), vecOfInt.end(), 
		[=](int elemInFoo){ return  (elemInFoo >= 10); });
	std::cout << '\n' << "it1 address: " << &it1 << " vec elem address: " << &(*it1) << " vec elem value: " << *it1 << '\n';

	for(const auto& s : vecOfInt) // Important: auto is int NOT iterator!
	{
		std::cout << "Address of elem in vec: " << &s << " value of elem in vec: " << s << '\n';
	}

	// ---- Basic filtering ----
	std::vector<int> vecOfFilteredElems;

    // copy only positive numbers:
    int upper = 11;
    int low = 2;
    std::copy_if(vecOfInt.begin(), vecOfInt.end(), std::back_inserter(vecOfFilteredElems), 
    	[=](int elemInFoo){ return  (elemInFoo >= low) && (elemInFoo <= upper); });

	for(const auto& s : vecOfFilteredElems) // Important: auto is int NOT iterator!
	{
		std::cout << "Filtered elem: " << s << '\n';
	}


	// ---- Test vector of shared pointers ----
	std::shared_ptr<Widget> p0 = std::make_shared<Widget>(17);
	std::shared_ptr<Widget> p1 = std::make_shared<Widget>(213);
	std::shared_ptr<Widget> p2 = std::make_shared<Widget>(19);
	std::shared_ptr<Widget> p3 = std::make_shared<Widget>(1);

	std::vector<std::shared_ptr<Widget>> vecOfWidgetPtrs {p0, p1, p2, p3};

	// By dereferencing iterator (*) - access underlying shared pointer in vector
	std::vector<std::shared_ptr<Widget>>::iterator it2 = vecOfWidgetPtrs.begin();
	for(; it2 != vecOfWidgetPtrs.end(); it2++)
	{
		std::cout << (*it2)->getId() << '\n';
	}

	// auto is type of std::shared_ptr<Widget> - NOT iterator! (Direct access)
	for(auto const& s : vecOfWidgetPtrs)
	{
		std::cout << "Direct access: " << s->getId() << '\n';
	}

	// ---- Sort ----
    std::sort(vecOfWidgetPtrs.begin(), vecOfWidgetPtrs.end(), 
    	[](const std::shared_ptr<Widget>& a, const std::shared_ptr<Widget>& b) // -> int
    {
        return a->getId() < b->getId();
    });

	std::cout << '\n';
	// auto is type of std::shared_ptr<Widget> - NOT iterator! (Direct access)
	for(auto const& s : vecOfWidgetPtrs)
	{
		std::cout << "Sorted: " << s->getId() << '\n';
	}

	// Filter - do something with filtered shared ptr Widgets
	int param = 15;
	//
	std::cout << '\n';
    for_each(vecOfWidgetPtrs.begin(),vecOfWidgetPtrs.end(),[=](std::shared_ptr<Widget> const& p)
    {
    	// Here p is shared_ptr Widget - NOT iterator!
        if( p->getId() > param ) std::cout << "Filter print:" << p->getId() << '\n';
    });

}


/*
int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
*/