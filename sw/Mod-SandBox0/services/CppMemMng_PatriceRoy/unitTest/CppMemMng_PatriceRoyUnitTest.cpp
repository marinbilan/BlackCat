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


TEST_F(cppMemMng0_Test, DISABLED_cppMemMng0_0)
{
	std::cout << "Test test test ... Test Nebo" << '\n';


}


TEST_F(cppMemMng0_Test, DISABLED_polymorphic_0)
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
TEST_F(cppMemMng0_Test, DISABLED_Cloneable_0)
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
TEST_F(cppMemMng0_Test, DISABLED_Cloneable_1)
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


TEST_F(cppMemMng0_Test, DISABLED_Cloneable_2)
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


// Pt 3
TEST_F(cppMemMng0_Test, DISABLED_Pt3_0)
{
using namespace Pt3_0;
	using std::malloc, std::free;

	int* p = static_cast<int*>(malloc(3*sizeof(int)));
	*p = 11;
	*(p+1) = 12;

	std::cout << "p: " << p << " *p: " << *p << '\n';
	std::cout << "p+1: " << p+1 << " *(p+1): " << *(p+1) << '\n';

	free(p); // fine since it comes from malloc()
}



TEST_F(cppMemMng0_Test, DISABLED_Pt3_1)
{
using namespace Pt3_0;
	using std::malloc, std::free;

	std::cout << "[TEST: 1]" << '\n';
	auto p = new int{ 3 };  // operator new(4) sizeof(int) = 4 Bytes
	delete p;  // operator delete(..., 4)

	std::cout << "[TEST: 2]" << '\n';
	p = new int[10];  // operator new[](40)
	delete []p;  // operator delete[](...)

	std::cout << "[END] " << __FUNCTION__ << '\n';
}



TEST_F(cppMemMng0_Test, DISABLED_Pt3_2)
{
	std::string testStr0("MyStr");
	string_length(testStr0.c_str());

	const char* p = testStr0.c_str();

	std::cout << "p[0]: " << p[0] << '\n';
	std::cout << "p[1]: " << p[1] << '\n';
	std::cout << "p[2]: " << p[2] << '\n';
	std::cout << "p[3]: " << p[3] << '\n';
	std::cout << "p[4]: " << p[4] << '\n';
 
}


// -------- Chapter: 11 Deferred Reclamation -------- 

TEST_F(cppMemMng0_Test, DISABLED_Chapter11_01)
{
// using namespace Chapter11_01;
   std::print("Pre\n");
   std::print("{}\n", Chapter11_01::f()->m());
   std::print("Post\n");
}


TEST_F(cppMemMng0_Test, DISABLED_Chapter11_02)
{
// using namespace Chapter11_02;
   std::print("Pre\n");
   std::print("{}\n", Chapter11_02::f()->m());
   std::print("Post\n");
}


TEST_F(cppMemMng0_Test, Chapter11_03)
{
   std::cout << "Pre" << std::endl;
   f();
   std::cout << h()->m() << std::endl;
   std::cout << "Post" << std::endl;
}


TEST_F(cppMemMng0_Test, DISABLED_Chapter11_04)
{
   std::cout << "Pre" << std::endl;
   f();
   std::cout << h()->m() << std::endl;
   std::cout << "Post" << std::endl;
}


TEST_F(cppMemMng0_Test, DISABLED_Chapter11_05)
{
   std::print("Pre\n");
   std::print("{}\n", f()->m());
   std::print("Post\n");
}


TEST_F(cppMemMng0_Test, DISABLED_Chapter11_06_1)
{
   std::print("Pre\n");
   std::print("{}\n", f()->m());
   std::print("Post\n");
}


TEST_F(cppMemMng0_Test, Chapter11_06_2)
{
   std::print("Pre\n");
   std::print("{}\n", f()->m());
   std::print("Post\n");
}