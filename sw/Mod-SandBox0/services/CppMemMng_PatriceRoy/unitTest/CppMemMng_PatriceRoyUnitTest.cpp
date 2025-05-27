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
	Polymorphic::TestClass tc(0);

	// Test
	Polymorphic::X x{ 3 };
	std::cout << "&x: " << &x << '\n';
	std::unique_ptr<Polymorphic::X> px { tc.duplicate(&x) };

	// Prepare
	Polymorphic::D0 d0{ 4 };
	// Test
	std::cout << "&d0: " << &d0 << '\n';
	std::unique_ptr<Polymorphic::B> pb{ tc.duplicate(&d0) }; // trouble ahead

}