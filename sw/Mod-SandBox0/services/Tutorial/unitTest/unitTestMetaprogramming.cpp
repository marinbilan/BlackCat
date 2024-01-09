#include <gmock/gmock.h>
#include <iostream>
#include <vector>
#include <string>

#include "Metaprogramming.h"


// TEST FIXTURE //
class MetaprogrammingTest : public ::testing::Test
{
public:

	MetaprogrammingTest()
	{
	}

	~MetaprogrammingTest()
	{
	}
};


// Variadic Template Function
TEST_F(MetaprogrammingTest, Test_VariadicTemplateFunction_E0)
{
    // Calling log() function with 3 arguments
    log(1 , 4.3 , "Hello");
    // Calling log() function with 4 arguments
    log('a', "test", 78L, 5);
    // Calling log() function with 2 arguments
    log("sample", "test");
}


TEST_F(MetaprogrammingTest, Test_Metaprogramming_E1)
{

}


TEST_F(MetaprogrammingTest, Test_Metaprogramming_E2)
{
    std::cout << std::is_same<float, float>::type::value << '\n';  // returns 1
    std::cout << std::is_same<float, float>::value << '\n';  // returns 1
   
    // [1] Case
   
    // Custom if_
    if_<true, int, float>::type intType = 10;  // Note that true will got to first derived if_ so type (mebmer) will be int
    if_<false, int, float>::type floatType = 10.0;  // Note that true will got to second derived if_ so type (member) will be float

    // Also we can check this by using is_same
    // (10 > 5) so first 'arg' is true so we override first and type is THEN (2nd argument) so int
    std::cout << std::is_same<int, /*custom begin*/ if_<(10 > 5), int, float>::type /*custom end */ >::value << '\n';  // int type == int type
    std::cout << std::is_same<float, /*custom begin*/ if_<(10 < 5), int, float>::type /*custom end */ >::value << '\n';  // float type == float type

    // This will work
    std::tuple<int, bool, float> tuple;
    std::tuple_element_t<0, decltype(tuple)> tt = 42;

    // std::cout << "Is type contained: " << std::is_same<std::tuple_element_t<2, decltype(tuple)>, int>::value << '\n';
    std::cout << "This is val: " << contains_type<int, decltype(tuple)>::value << '\n';  // true
    // std::cout << "This is val: " << contains_type<double, decltype(tuple)>::value << '\n';  // false

}


TEST_F(MetaprogrammingTest, Test_Metaprogramming_E3)
{

}