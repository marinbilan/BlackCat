#include <gmock/gmock.h>
#include <iostream>
#include <vector>
#include <string>

#include "InheritanceAndPoly.h"


// TEST FIXTURE //
class InheritanceAndPolyTest : public ::testing::Test
{
public:

	InheritanceAndPolyTest()
	{
	}

	~InheritanceAndPolyTest()
	{
	}
};



TEST_F(InheritanceAndPolyTest, Test_InheritAndPoly_0)
{
// Create derived object on stack
    ClientImpl wI0(11);
    ClientImpl wI1(112);
    ClientImpl wI2(111);
    ClientImpl wI3(1123);
    // std::cout << wI0.getBaseId() << '\n';
    // std::cout << wI0.getBaseIdVirtual() << '\n';
   
    // Also possible creation on stack: ClientImpl wI0(11); // = ClientImpl(11);
    std::vector<ClientImpl> vecClinetImpl = {wI0, wI1, wI2, wI3};

    for(auto const& s : vecClinetImpl)
    {
        s.getBaseIdVirtual();
    }
   
    // Sort
    std::sort(vecClinetImpl.begin(), vecClinetImpl.end(), [](const ClientImpl& a, const ClientImpl& b) -> int
    {
        return a.getBaseIdVirtual() < b.getBaseIdVirtual();
    });

    // Sorted
    for(auto const& s : vecClinetImpl)
    {
        s.getBaseIdVirtual();
    }
   
   
    std::cout << " ---- CREATE BASE object ---- " << '\n';
    // ClientBase b0(1111);
    // ff(b0);  // Unable to call this method with base object
   
    ClientImpl cI(1112);
    // f(cI); // Object slicing
    ff(cI);
   
    std::cout << " ---- CREATE SHARED ptr objects ---- " << '\n';
    std::shared_ptr<ClientBase> p0 = std::make_shared<ClientImpl>(17);
    std::shared_ptr<ClientBase> p1 = std::make_shared<ClientImpl>(15);
    std::shared_ptr<ClientBase> p2 = std::make_shared<ClientImpl>(13);
    std::shared_ptr<ClientBase> p3 = std::make_shared<ClientImpl>(16);
    std::shared_ptr<ClientBase> p4 = std::make_shared<ClientImpl>(28);

    std::vector<std::shared_ptr<ClientBase>> vecSharedPtrs = {p0, p1, p2, p3, p4};
    std::cout << " ---- SORT SHARED ptr objects ---- " << '\n';
   
    // Sort
    std::sort(vecSharedPtrs.begin(), vecSharedPtrs.end(), [](const std::shared_ptr<ClientBase>& a, const std::shared_ptr<ClientBase>& b) // -> int
    {
        return a->getBaseIdVirtual() < b->getBaseIdVirtual();
    });  

    int param = 15;
    //
    for_each(vecSharedPtrs.begin(),vecSharedPtrs.end(),[=](const std::shared_ptr<ClientBase>& p)
    {
        if( p->getBaseIdVirtual() > param ) std::cout << "Filter print:" << p->getBaseIdVirtual() << '\n';
    });
   

    // Sorted Shared ptr objects
    for(auto const& s : vecSharedPtrs)
    {
        s->getBaseIdVirtual();
    }

    std::cout << " ---- TEST MOCK stuff ---- " << '\n';
    ClientImpl clientImplMock(22);
    fff(&clientImplMock);
   
   
    // Filtering
    std::vector<int> foo = {25,15,5,-5,-15};
    std::vector<int> bar;

    // copy only positive numbers:
    int upper = 20;
    int low = 0;
    std::copy_if (foo.begin(), foo.end(), std::back_inserter(bar), [=](int elemInFoo){ return  (elemInFoo >= low) && (elemInFoo <= upper); });

    for(auto const& s : bar)
    {
        std::cout << s << '\n';
    }


    std::cout << " ---- Send object from shared ptr ---- " << '\n';
    std::shared_ptr<ClientImpl> objFromSharedPtr = std::make_shared<ClientImpl>(17);  
    ff(*objFromSharedPtr);

    std::vector<int> vec{ 10, 25, 40, 55 };
    std::vector<int>::iterator it = std::find_if(vec.begin(), vec.end(), [=](int elemInFoo){ return  (elemInFoo >= 0); });
    // int itTemp = it; // Error
    // std::cout << " ---- it: " << *it << '\n';
}