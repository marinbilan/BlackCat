#include <gmock/gmock.h>
#include <iostream>
#include <vector>
#include <string>

#include "ModernCppConcepts.h"


// TEST FIXTURE //
class ModernCppConceptsTest : public ::testing::Test
{
public:

	ModernCppConceptsTest()
	{
	}

	~ModernCppConceptsTest()
	{
	}
};



// CASE 1 : Klaus Iglberger implementation
TEST_F(ModernCppConceptsTest, Test_ModernCppConcepts_T0)
{
    // CASE 1 : Klaus Iglberger implementation
    /*
    In case when we have defined any constructor, we need to invoke that constructor.
    For example:
    Widget w(0);  // Is ok because we have defined constructor: Widget(int id) : m_id(id) {}
    Widget w;     // This will not work because constructor is defined (this call will work if constructor is not defined)
                  // error: no matching function for call to ‘Widget::Widget()’
    */
   /*
    std::cout << "-- CASE 1 --" << '\n';

    // In this implementation we have defined constructor - so: function<void()> f0 will not work
    function<void()> f0(func);
    f0();  // out: func
    ff(f0);

    std::cout << "-- 127 --" << '\n';
    function<void(int)> f1( [](int i) { std::cout << "Given number = " << i << '\n'; } );
    f1(127);  // out: Given number = 127
    */
    std::cout << "----" << '\n';
}


// CASE 2 : Naive implementation - similar to upper
TEST_F(ModernCppConceptsTest, Test_ModernCppConcepts_T1)
{
  /*
    Note that in naive implementation we do not have defined constructor!
    So below object construction is valid
    */
    std::cout << "-- CASE 2 --" << '\n';

    naive_function<void()> f;
   
    f = func;
    f();
    f = functor();
    f();
    f = []() { cout << "lambda" << endl; };
    f();
}


// Variadic, visitor and Overload Pattern
TEST_F(ModernCppConceptsTest, Test_VariadicVisitorOverloadPattern)
{
 // **** CASE: 1 ****
    std::variant<bool, int, std::string> data;
   
    // Set data        
    data = std::string("Ginger");
    // Print data
    print(data);
   
    // ----
    // Change data
    data = 108;
    // Print data
    print(data);
   
   
    // 2. CASE
    std::vector<std::variant<int, long, double, std::string>> vec = {10, 15l, 15l, 1.5, "hello", 11, 16, 15l};
}