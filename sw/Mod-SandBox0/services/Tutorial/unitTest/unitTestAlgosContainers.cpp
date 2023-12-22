#include <gmock/gmock.h>
#include <iostream>
#include <vector>
#include <string>

#include "AlgosContainers.h"


// TEST FIXTURE //
class AlgosContainersTest : public ::testing::Test
{
public:

	AlgosContainersTest()
	{
	}

	~AlgosContainersTest()
	{
	}
};


/*
Learn Advanced Modern C++ - Algorithms, Containers
*/
// [65] Algorithms overview
TEST_F(AlgosContainersTest, DISABLED_Test_AlgosOverview)
{

    // testF();
    std::string str{"Hello World"};
    std::cout << "String: ";

    for(auto c : str) {
        std::cout << c << ", ";
    }
    std::cout << "\n";

    auto res = std::find(std::cbegin(str), std::cend(str), 'l');

    std::cout << "res: " << *res << '\n';

    if(res != std::cend(str)) {
        std::cout << "Found a matching element at index: " << res - str.begin() << '\n';
    }
}


// [66] Algorithms with predicates
TEST_F(AlgosContainersTest, DISABLED_Test_AlgosWithPredicates)
{
    Element el1(11);
    Element el2(13);
    Element el3(12);

    std::vector<Element> vecOfElems = {el1, el2, el3};

    // Sort using std::sort
    // std::sort(std::begin(vecOfElems), std::end(vecOfElems));

    for(auto s : vecOfElems) {
        std::cout << s.getId() << '\n';
    }

    // Sorting using std::sort with predicate
    std::cout << "Sort using predicate" << '\n';
    std::sort(std::begin(vecOfElems), std::end(vecOfElems), isBiggerMethod);

    // Sorting using std::sort using functor
    std::cout << "Sort using functor" << '\n';
    std::sort(std::begin(vecOfElems), std::end(vecOfElems), IsBiggerFunctor());
}


// [67] Algorithms with _if versions
TEST_F(AlgosContainersTest, Test_AlgosWith_ifVersions)
{

}