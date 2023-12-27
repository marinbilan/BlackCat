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
    std::sort(std::begin(vecOfElems), std::end(vecOfElems));

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
TEST_F(AlgosContainersTest, DISABLED_Test_AlgosWith_ifVersions)
{
    std::vector<std::string> names = {"PHB", "Dilbert", "Asok", "Dogbert", "Ted", "Alice", "Wally"};

    auto res = find_if(std::cbegin(names), std::cend(names), ge_5());
    // First name with less than 5 characters
    // auto res = find_if_not(std::cbegin(names), std::cend(names), ge_5());

    if(res != cend(names)) {
        std::cout << "First name with more than 5 charachters: " << *res << '\n';
    }

    // General case (n defined in constructor)
    // auto res_gen = find_if(std::cbegin(names), std::cend(names), ge_n(5));
}


// [68] Lambda Expression Introduction
TEST_F(AlgosContainersTest, DISABLED_Test_LambdaExpressionIntro)
{
    std::vector<int> vec{3, 1, 4, 1, 5, 9};

    // Pass Lambda Expression
    auto odd_it = std::find_if(std::cbegin(vec), std::cend(vec),
        [](int n) {
            return (n % 2 == 1);
        });

    // Iterator of the first element true with predicate
    if(odd_it != std::cend(vec)) {
        std::cout << "First odd element is: " << *odd_it << '\n';
    }
}


// [69] Lambda Expression Practical
TEST_F(AlgosContainersTest, DISABLED_Test_LambdaExpressionPractical)
{

}


// [70] Lambda Expression Capture
TEST_F(AlgosContainersTest, DISABLED_Test_LambdaExpressionCapture)
{

}


// [79] Numeric Algorithms
TEST_F(AlgosContainersTest, Test_NumericAlgorithms)
{
/*
    // [1]
    // std::iota
    // Operators: ++, = (compiler generated)
    Test t3(3);
    Test t4(4);
   
    std::vector<Test> vec = {t3, t4};
 
    // Start value from start_value
    Test start_value(133);
    // Start adding objects t(m_id = 113), t(m_id = 114), t(m_id = 115) ...
    // Note that iterators are not const because value of vector needs to be modify
    std::iota(std::begin(vec), std::end(vec), start_value);

    for(auto s : vec)
    {
        std::cout << s.m_id << '\n';
    }
   
    // Check operators: +, ++
    */

    // [2]
    // std::accumulate
    // Operators: +, = (compiler generated), ++ (does not need to be implemented)
   
    TestClass t3(3);
    TestClass t4(4);
   
    /*
    TestClass t_0(0);  // Create default constructor
    t_0 = t3 + t4;
    std::cout << "t_0 = " << t_0.m_id << '\n';
    */
   
    std::vector<TestClass> vec = {t3, t4};
   
    // Accumulate from custom object (IMPORTANT) (Not possible from 0)
    TestClass start_point(117);

    TestClass accumulated_value = std::accumulate(cbegin(vec), cend(vec), start_point);  
    std::cout << "Accumulated value = " << accumulated_value.m_id << '\n';
   
    // Accumulate with extra argument
    TestClass accumulated_value2 = std::accumulate(cbegin(vec), cend(vec), start_point,
        [](TestClass start_point_sum, TestClass val_of_each_iter_elem) {
            // std::cout << "> sum: " << start_point_sum.m_id << '\n';
            // std::cout << "> elem: " << val_of_each_iter_elem.m_id << '\n';
           
            TestClass temp(118);
           
            // Do some logic here for summing elements ...
            // If this is even add if not do not add element to sum
            // return (n % 2 == 1) ? sum + n : sum;
           
            // return temp;

            // % operator is not implemented so we need to take .m_id
            return (val_of_each_iter_elem.m_id % 2 == 1) ?
                start_point_sum + val_of_each_iter_elem : start_point_sum;
        });
       
    std::cout << "Temp value: " << accumulated_value2.m_id << '\n';
}