#include <gmock/gmock.h>
#include "ServiceIf.h"
#include "Service0.h"

// using ::testing::AtLeast;

// TEST FIXTURE //
class Mod_6_0_UnitTest : public ::testing::Test
{
public:

	Mod_6_0_UnitTest()
	{
		// 
	}

	~Mod_6_0_UnitTest()
	{
		// delete m_myClass;
	}
};


TEST_F(Mod_6_0_UnitTest, first_Mod6_Test)
{
	Service::Service0 srv0("Test", "Test");
	srv0.preInit();

}