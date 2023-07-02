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



/*
YT (Bo Qian): Learn C++ 11 in 20 Minutes - Part I
*/

/*
[ITEM 1] - Initializer list
*/
TEST_F(ServiceXUnitTest, DISABLED_Pt_I_Item_1_Test)
{
    // SO FAR
    // In C++ 03 it is possible to init array like this:
    int arr[4] = {3, 2, 4, 5}; // This is initializer list
   
    // C++ 03 only way is to push (back) value in vector
    std::vector<int> v;
    v.push_back(3);
    v.push_back(2);
    v.push_back(4);
    v.push_back(5);

    // C++ 11 extends support of Initializer list to many other classes
    // All the relevant STL containers have been updated to accept initializer_list
    std::vector<int> v0 = {3, 4, 1, 9};  //  Calling initializer_list constructor
   
    // C++ 11 Possible to define own initializer_list constructor for custom class

    // Custom vector now is possible init as array init
    Services::boVector_0 v1 = {0, 2, 3, 4};
    Services::boVector_0 v2{0, 2, 3, 4};
}


/*
[ITEM 2] - Uniform Initialization
*/
TEST_F(ServiceXUnitTest, DISABLED_Pt_I_Item_2_Test)
{
    // C++ 03 - Aggregate class or struct
    Services::Dog_0 d1 = {5, "Henry"}; // Aggregate Initialization

    // C++ 11 - Extendet the scope of curly brace init (constructor)
    Services::Dog_1 d2 = {5, "Henry"};

    /*
	// So far the same format of curly braces {} init:
	1] Initialization List init
	2] Aggregate init
	3] Extended scope of curly brace init - Call regulat constructor with {}

	Curly brace init - together called Uniform Initialization
   
    They all above called Uniform Initialization
    In exes of compiler all three init have different prios:
    1. Initializer_list constructor
    2. Regular constructor that takes the appropriete parameters
    3. Aggregate initializer
    */

    Services::Dog_2 dog_2_0{3};  // Found Init list - first choice
}


/*
[ITEM 3] - Auto Type
*/
TEST_F(ServiceXUnitTest, DISABLED_Pt_I_Item_3_Test)
{
	// 03 C++
	std::vector<int> vec = {2, 3, 4, 5};

    for(std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
       
    }

    // C++ 11
    for(auto it = vec.begin(); it != vec.end(); ++it)
    {
        std::cout << "Num: " << *it << '\n';
    }

    auto a = 5; // auto = int 
}


/*
[ITEM 4] - foreach
*/
TEST_F(ServiceXUnitTest, DISABLED_Pt_I_Item_4_Test)
{
	// C++ 03
	// Iterators via standard containers

	// C++ 11
	std::vector<int> vec = {2, 3, 4, 5};

	 // Assign every item in vec to i (copy) and operate on i
    // Works on any class that has begin() and end() function
    for(auto i : vec)  // for(int i : vec)
    {
        std::cout << "i: " << i << '\n';
    }
   
    // Change items in vec
    for(auto& i : vec)
    {
        i++;
        std::cout << "i : " << i << '\n';
    }
}


/*
[ITEM 5] - nullptr
*/
TEST_F(ServiceXUnitTest, DISABLED_Pt_I_Item_5_Test)
{
	// C++ 03 - NULL - defined as integer 0
	// Services::foo_0(NULL);  // Ambiguity

	// C++ 11
	Services::foo_0(nullptr);

}


/*
[ITEM 6] - Enum Class
*/
TEST_F(ServiceXUnitTest, DISABLED_Pt_I_Item_6_Test)
{
    // enum class
    // C++ 03 - Enums are integers
    /*
    enum apple {green_a, red_a};
    enum orange {big_o, small_o};
    apple  a = green_a;
    orange o = big_o;
   
    // Compare integeres
    if(a == o) std::cout << "Green apple and big orange are the same" << '\n';
    else std::cout << "Green apple and big orange are not the same" << '\n';
    */

    // C++ 11 - Enum class
 	
    enum class apple {red, green};
    enum class orange {big, small};
    apple  a = apple::red;
    orange o = orange::big;
   
    // This fails because no operator == defined ==(apple, orange)
    // if(a == o) std::cout << "The same ..." << '\n';

}


/*
[ITEM 7] - Static Assert
*/
TEST_F(ServiceXUnitTest, DISABLED_Pt_I_Item_7_Test)
{
    // static assert
    // run-time assert
    // assert (myPointer != NULL);
   
    // C++ 11
    // static_assert(sizeof(int) == 4);
}


/*
[ITEM 8] - Delegating Constructors
*/
TEST_F(ServiceXUnitTest, DISABLED_Pt_I_Item_8_Test)
{
	Services::Dog_3 dog(42);
}



// ---- ==== ----



/*
YT (Bo Qian): Learn C++ 11 in 20 Minutes - Part II
*/


/*
[ITEM 9] - Override (for virtual functions)
*/
TEST_F(ServiceXUnitTest, Pt_II_Item_9_Test)
{
	
}



TEST_F(ServiceXUnitTest, DISABLED_Part_II_Test)
{

	

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