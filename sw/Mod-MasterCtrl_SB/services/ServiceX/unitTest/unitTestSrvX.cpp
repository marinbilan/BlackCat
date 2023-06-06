#include <gmock/gmock.h>


#include "ServiceIf.h"
#include "ServiceX.h"

// using ::testing::AtLeast;

// TEST FIXTURE //
class ServiceXUnitTest : public ::testing::Test
{
public:
// Networking::ClientIf* m_httpsClient;

	ServiceXUnitTest()
	{
		// m_httpsClient = new Networking::HTTPSClient("test", "test");

	}

	~ServiceXUnitTest()
	{
		// delete m_myClass;
	}
};


// Sandbox test
TEST_F(ServiceXUnitTest, DISABLED_firstServiceXTest)
{
	Services::ServiceX srvX("Test", "Test");

	// Create Widget
	Services::Widget w(0);

}


/*
YT (Bo Qian): Learn C++ 11 in 20 Minutes - Part I
*/
TEST_F(ServiceXUnitTest, DISABLED_Part_I_Test)
{

	// [ITEM 8] - Delegating constructor
	Services::Dog dog(42);


	// [ITEM 9] - Override for virtual functions
}


/*
YT (Bo Qian): Learn C++ 11 in 20 Minutes - Part II
*/
TEST_F(ServiceXUnitTest, DISABLED_Part_II_Test)
{

	// [ITEM 9] - Override for virtual functions

	// [ITEM 10] - final
	/*
	For virtual functions and for classes
	- No class can not be derived from dog
	- No class (derived) can override method

	class dog final 
	{
		// No child of dog can override bark() function
		virtual void bark() final; // No class can override bark()
	};
	*/

	// [ITEM 11] - If no consturctor - compiler will generate
	// Handy if default constructor is what you want
	Services::Dog2 d2;

	// [ITEM 12] - delete 
	// Set delete if you do not want implicit conversion


	// [ITEM 13] - constexpr


	// [ITEM 14] - New string literals
	// Better support for unicode


	// [ITEM 15] - Lambda function (anonimus funtions)
	std::cout << [](int x, int y) { return x + y; }(3, 4) << '\n';
	// or
	auto f = [](int x, int y) { return x + y; };
	std::cout << f(3, 4) << '\n';


	// FUNCTIONAL PROGRAMMING
	/*
	std::vector<int> v = {1, 2, 3, 4, 5, 6};
	// x is what we apply in filter (template) function (each elem of vector)
	Services::filter([](int x) { return x > 3; }, v);  // Out: 4, 5, 6
	Services::filter([](int x) { return (x > 2 && x < 5); }, v);  // Out: 3, 4

	// By passing local reference
	int y = 4;
	Services::filter([&](int x) { return (x > y);}, v); // Out: 5, 6
	*/
}



// C++ 11: Rvalue Reference -- Move Semantics
TEST_F(ServiceXUnitTest, DISABLED_moveSemantics)
{
	// What is rvalue reference?
	// int a = 5;   // a is lvalue
	// int& b = a;  // b is lvalue reference (before C++ 11 - reference)
	// int&& c;     // c is rvalue reference

	// Services::printInt(a);  // Call printInt(int& i)  - a is lvalue
	// Services::printInt(6);  // Call printInt(int&& i) - 6 is rvalue

	Services::boVector v(10);   // Calling Constructor
	Services::boVector v2 = v;  // Calling Copy (implemented) Constructor (boVector on left side)
	std::cout << "----" << '\n';

	// 1] Creates boVector (Calling boVector constructor)
	// Here compiler optimize and call default move constructor (no print)
	Services::boVector reusable = Services::createBoVector();
	std::cout << "----" << '\n';

	// Calling copy constructor
	/*
	This is ok because in foo method I want to use copy of boVector
	Services::boVector v = reusable;  // boVector on left side
	*/
	Services::foo(reusable);  // Copy constructor
	std::cout << "----" << '\n';

	/*
	Calling boVector constructor to create temporary object
	In theory after that copy constructor should be called but compiler optimize and calls 
	default move constructor
	createBoVector returns rvalue - temporary which will be destroyed momentarly
	No point to create copy of temporary and pass to foo 
	Why not use directly created temp object and pass it to foo 
	*/
	foo(Services::createBoVector());

	/*
	We could achive this by define two methods:
	void foo_by_value(boVector v);
	void foo_by_ref(boVector& v);

	foo_by_value(reusable);  // No constructor call (ref to reusable)
	foo_by_ref(Services::createBoVector());
	*/

	// Explicitly move in foo 
	// This will force move constructor
	// Services::boVector bo = std::move(reusable);  // boVector on left
	// Be careful - reusable is no more valid
	Services::foo(std::move(reusable));  // Move constructor
}



// C++ 11: Rvalue Reference -- Perfect Forwarding
TEST_F(ServiceXUnitTest, perfectForwarding)
{
	// T&& depends how arg is initialized
	/*
	If arg is init with rvalue -> T&& is rvalue reference
	If arg is init wiht lvalue -> T& is lvalue reference

	// T&& var is rvalue
	relay(9); =>  T = int&&  =>  T&& = int&& && = int&&

	// T&& var is lvalue
	relay(x); =>  T = int&  =  T&& = int& && = int&

	T&& is Universal Reference (rvalue, lvalue, const, non-const ...)
	if two condition are met:
	1] T is a template type
	2] Type deduction (reference collapsing) happens to T.
	   - T is a function template type, not class template type
	*/


}