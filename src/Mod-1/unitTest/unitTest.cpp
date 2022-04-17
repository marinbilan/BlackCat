#include <gmock/gmock.h>
#include <memory>
#include <map>

#include "ServiceIf.h"
#include "ServiceBase.h"
#include "Service0.h"
#include "Service1.h"
#include "ServiceBaseDerv.h"

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


// Test Widget
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
	// ---- Basic iterator operations on vector----
	// Ex: 1
	std::vector<int> vecOfInt = {0, 2, 16, 8, 11};

	std::vector<int>::iterator it0 = vecOfInt.begin();
	int vecIndex = 0;
	for(; it0 != vecOfInt.end(); it0++)
	{
		// Iterator address is always the same - only value (of iterator) is increased
		// std::cout << "Iterator it0 address: " << &it0 << '\n';

		// Address of elem in iterator vs address of elem in vector
		EXPECT_EQ(&(*it0), &vecOfInt.at(vecIndex));
		// Elem value in iterator vs elem value in vector
		EXPECT_EQ(*it0, vecOfInt.at(vecIndex));

		vecIndex++;
	}

	// ---- Return iterator on first element greather than value ----
	// Ex: 2
	// it1 have own address, but underlying elem of vector has the same address and value
	int lowLimit = 10;

	// Find and return first iterator of elem greather than lowLimit (in vector)
	std::vector<int>::iterator it1 = std::find_if(vecOfInt.begin(), vecOfInt.end(), 
		[=](int elemInFoo){ return  (elemInFoo >= lowLimit); });

	// First elem greather than 10
	int wantedElemIndex = 2;
	EXPECT_EQ(*it1, vecOfInt.at(wantedElemIndex));

	for(const auto& s : vecOfInt) // Important: auto is int - NOT iterator!
	{
		// std::cout << "Address of elem in vec: " << &s << " value of elem in vec: " << s << '\n';
	}

	// ---- Basic filtering copy_if----
	// Ex: 3
	std::vector<int> vecOfFilteredElems;

    // Copy only numbers inside interval:
    int up = 11;
    int low = 2;
    std::copy_if(vecOfInt.begin(), vecOfInt.end(), std::back_inserter(vecOfFilteredElems), 
    	[=](int elemInvecOfInt){ return  (elemInvecOfInt >= low) && (elemInvecOfInt <= up); });

    // Expected values
    std::vector<int> vecOfExpectedVals = {2, 8, 11};
    EXPECT_EQ(vecOfFilteredElems, vecOfExpectedVals);
	for(const auto& s : vecOfFilteredElems) // Important: auto is int NOT iterator!
	{
		std::cout << "Filtered elem: " << s << '\n';
	}


	// ---- Test vector of shared pointers ----
	// Ex: 4
	std::shared_ptr<Widget> p0 = std::make_shared<Widget>(17);
	std::shared_ptr<Widget> p1 = std::make_shared<Widget>(213);
	std::shared_ptr<Widget> p2 = std::make_shared<Widget>(19);
	std::shared_ptr<Widget> p3 = std::make_shared<Widget>(1);

	std::vector<std::shared_ptr<Widget>> vecOfWidgetPtrs {p0, p1, p2, p3};

	// By dereferencing iterator (*) - access underlying shared pointer in vector
	std::vector<std::shared_ptr<Widget>>::iterator it2 = vecOfWidgetPtrs.begin();
	for(; it2 != vecOfWidgetPtrs.end(); it2++)
	{
		// std::cout << (*it2)->getId() << '\n';
	}

	// auto is type of std::shared_ptr<Widget> - NOT iterator! (Direct access)
	for(auto const& s : vecOfWidgetPtrs)
	{
		// std::cout << "Direct access: " << s->getId() << '\n';
	}

	// ---- Sort ----
	// Ex: 5
    std::sort(vecOfWidgetPtrs.begin(), vecOfWidgetPtrs.end(), 
    	[](const std::shared_ptr<Widget>& a, const std::shared_ptr<Widget>& b) // -> int
    {
        return a->getId() < b->getId();
    });

	std::cout << '\n';
	// auto is type of std::shared_ptr<Widget> - NOT iterator! (Direct access)
	for(auto const& s : vecOfWidgetPtrs)
	{
		// std::cout << "Sorted: " << s->getId() << '\n';
	}

	// Filter - do something with filtered shared ptr Widgets
	// Ex: 6
	int param = 15;
	//
	std::cout << '\n';
    for_each(vecOfWidgetPtrs.begin(),vecOfWidgetPtrs.end(),[=](std::shared_ptr<Widget>& p)
    {
    	// Here p is shared_ptr Widget - NOT iterator!
        if( p->getId() > param ) 
        {
        	// std::cout << "Filter print:" << p->getId() << '\n';
        }
    });
}


TEST_F(myClassTest, TestMapIterators)
{
    // ---- Basic iterator operations ----
    // Ex: 1
    std::map<std::string, int> myMap;
 
    // insert elements in random order
    myMap.insert(std::pair<std::string, int>("First", 40));
    myMap.insert(std::pair<std::string, int>("Second", 30));
    myMap.insert(std::pair<std::string, int>("Third", 60));
    myMap.insert(std::pair<std::string, int>("Forth", 20));
   
   
	std::map<std::string, int>::iterator it0 = myMap.begin();
	for(; it0 != myMap.end(); it0++)
	{
		// Iterator address is always the same - only value is increased
        // Access underlying value in vector: *it
        // Access underlying value in map: it->first, it->second
		// std::cout << "it0 address: " << &it0 << '\n';
		// std::cout << "First vec elem address: " << &(it0->first) << " Second vec elem address: " << &(it0->second) << '\n';
		// std::cout << "First map elem value: " << it0->first << " Second map elem value: " << it0->second << '\n';
	}    

    // std::cout << " ---- " << '\n';
	for(const auto& [s0, s1] : myMap) // Important: auto is int NOT iterator!
	{
		// std::cout << " first map elem value: " << s0 << " second map elem value: " << s1 << '\n';
	}

	// ---- Return iterator on first element greather than lowLimit ----
	// Ex: 2
	// it1 will have own address, but underlying elem of map will have the same address and value
	// Iterator is returned but as argument we have pair (not pointer type) - so we have access elemInMap.second
	int lowLimit = 10;
	std::map<std::string, int>::iterator it1 = std::find_if(myMap.begin(), myMap.end(),
	[=](const auto& elemInMap){ return  elemInMap.second >= lowLimit; });

	// std::cout << '\n' << "it1 address: " << &it1 << " map elem address: " << &(it1->first) << " map elem value: " << it1->second << '\n';

	// ---- Basic filtering copy_if----
	// Ex: 3
	std::map<std::string, int> myMapFilteredElems;
	std::copy_if(myMap.begin(),myMap.end(), std::inserter(myMapFilteredElems,myMapFilteredElems.end()),
    	[](auto const& x)
    	{
            return x.second > 30;    
        });

	// ---- Test vector of shared pointers ----
	// Ex: 4
	std::shared_ptr<Widget> p0 = std::make_shared<Widget>(17);
	std::shared_ptr<Widget> p1 = std::make_shared<Widget>(213);
	std::shared_ptr<Widget> p2 = std::make_shared<Widget>(19);
	std::shared_ptr<Widget> p3 = std::make_shared<Widget>(1);

	std::map<std::string, std::shared_ptr<Widget>> mySharedMap; //  {p0, p1, p2, p3};

	mySharedMap.insert(std::pair<std::string, std::shared_ptr<Widget>>("First",  p0));
    mySharedMap.insert(std::pair<std::string, std::shared_ptr<Widget>>("Second", p1));
    mySharedMap.insert(std::pair<std::string, std::shared_ptr<Widget>>("Third",  p2));
    mySharedMap.insert(std::pair<std::string, std::shared_ptr<Widget>>("Fourth", p3));

	// By dereferencing iterator (*) - access underlying shared pointer in vector
	std::map<std::string, std::shared_ptr<Widget>>::iterator it2 = mySharedMap.begin();
	for(; it2 != mySharedMap.end(); it2++)
	{
		std::cout << "Map of shared ptrs: " << it2->first << " " << it2->second->getId() << '\n';
	}

	// Filter something
	// Filter - do something with filtered shared ptr Widgets
	int param = 15;
	//
	std::cout << '\n';
	for_each(mySharedMap.begin(),mySharedMap.end(),[=](std::pair<std::string, std::shared_ptr<Widget>> const& p)
	{
		// Here p is shared_ptr Widget - NOT iterator - std::pair! - Access using .
		if( p.second->getId() > param ) std::cout << "Filter print:" << p.first << " " << " id: " << p.second->getId() << '\n';
	});
}


void f(Service::ServiceBase srvBase)
{
    srvBase.getName();
}

void ff(Service::ServiceBaseDerv srvBase)
{
    srvBase.testVirtualMethod();  
}

void fff(Service::ServiceBase* srvBasePtr)
{
    srvBasePtr->testVirtualMethod();
}


TEST(myTest, TestInheritance)
{
	// ---- Create derived instance on stack ----
	// Base would not be able to instanciate if only one method was pure virtual
	Service::ServiceBaseDerv derivedInst("dbPath", "instName");
	// It is posible to call method with srvBase class, but object sliceing
	f(derivedInst);
	ff(derivedInst);
	fff(&derivedInst);  // Same as ff()

	// Service::ServiceBase baseInst("dbPath", "instName");

    std::shared_ptr<Service::ServiceBaseDerv> objFromSharedPtr = std::make_shared<Service::ServiceBaseDerv>("dbPath", "instName");  
    ff(*objFromSharedPtr);
}


/*
int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
*/