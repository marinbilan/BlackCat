#include "cppMemMng0.h"
#include <gmock/gmock.h>




// TEST FIXTURE //
class cppMemMng0_Test : public ::testing::Test
{
public:

	cppMemMng0_Test()
	{
	}

	~cppMemMng0_Test()
	{
	}
};


TEST_F(cppMemMng0_Test, cppMemMng0_0)
{
	std::cout << "Test test test ... Test Nebo" << '\n';


}


TEST_F(cppMemMng0_Test, polymorphic_0)
{
	std::cout << "-------- Polymorphic --------" << '\n';
	
	// Prepare
	std::cout << "[Test 1]" << '\n';
	Polymorphic::TestClass tc(0);

	// Test 1
	Polymorphic::X x{ 3 };
	std::cout << "&x: " << &x << '\n';
	std::unique_ptr<Polymorphic::X> px { tc.duplicate(&x) };

	// Prepare
	std::cout << "[Test 2]" << '\n';
	/*
	B
	|
	D0 (Create this and pass this ptr to base ptr arg in method)
	Create first (destruct in reverse order)
		Base object
		Derived object
	*/
	Polymorphic::D0 d0{ 4 };
	// Test 2
	std::cout << "&d0: " << &d0 << '\n';
	std::unique_ptr<Polymorphic::B> pb{ tc.duplicate(&d0) }; // trouble ahead

	// We will need cloning (duplicating)

}


/*
Suppose we want to develop a skeleton of dup_ptr<T> that copies types that are not derived from
cloneable and clones types that are. To that effect, we can use the std::conditional type trait and
choose between two function object types, a Copier type that copies and a Cloner type that clones.

...

With this implementation, any type that derives from cloneable will be cloned and
other types will be copied, unless the user code supplies an exotic implementation of the Dup type.
*/
TEST_F(cppMemMng0_Test, Cloneable_0)
{
	// Cloneable::X x(0);

	std::cout << "[Test 1] " << '\n';
	Cloneable_0::A a;

	/*
	class dup_ptr is template for concrete classes as A and B : Cloneable { clone }

	Copier template <class T>    Cloner template <class T>
	| Creator of A (normal)      | Creator of B (Cloneable)
	|                            | operator(T* p) { calls p->clone()}
	template <class T, class Dup ...>


	A - normal class (call Copier template)

	Clonable
	|
	B - class with clone (call Clone template)
	*/


	/*
	<class T, class Dup = ...> class dup_ptr - constructor - T* p = Dup{}(&a) // Call operator() in struct Copier
	1st Execute operator() in Copier (init mamber vars T* {})
	2nd Execute constructor body of dup_ptr

	dup_ptr is container around (&a) - we can call it's methods like =, get, empty
	dup_ptr.get()-> ... now we get underlying raw ptr (call methods from A or B)
	*/

    Cloneable_0::dup_ptr<Cloneable_0::A> a_ptr(&a);  // Uses Copier
	
	std::cout << '\n' << "[Test 2]" << '\n';
    Cloneable_0::B b;
    Cloneable_0::dup_ptr<Cloneable_0::B> b_ptr(&b);  // Uses Cloner (inherits from Cloneable)
	

    std::cout << a_ptr.get()->x << std::endl;
    std::cout << b_ptr.get()->y << std::endl;
}


/*
Here’s a simple unit test for your dup_ptr class that demonstrates the behavior of choosing 
between Cloner and Copier, depending on whether the class has a .clone() method.
I'll use minimal mock classes and assert() from <cassert> for testing.
*/
TEST_F(cppMemMng0_Test, Cloneable_1)
{
using namespace Cloneable_1;

    // Test ... TODO

	/*
	The advantage of this implementation over the previous one is that this one checks 
	for the existence of a suitably written clone() member function, whereas the previous 
	one checks for the existence of a specic base class.

	Implementing a function is a lighter contract than deriving from a specic base class.
	*/
}


TEST_F(cppMemMng0_Test, Cloneable_2)
{
using namespace Cloneable_2;

	X x{42};
    non_null_ptr<X> ptr{&x};

    std::cout << extract_value(ptr) << "\n"; // prints 42

    try 
	{
        non_null_ptr<X> bad_ptr{nullptr}; // throws
    } catch (const invalid_pointer&) 
	{
        std::cout << "Caught invalid_pointer\n";
    }

}