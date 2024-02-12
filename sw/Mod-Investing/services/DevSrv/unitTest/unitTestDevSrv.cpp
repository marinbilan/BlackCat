#include <gmock/gmock.h>

#include "InvDevIf.h"
#include "InvDev0.h"

// using ::testing::AtLeast;

// TEST FIXTURE //
class InvDevUnitTest : public ::testing::Test
{
public:
// Networking::ClientIf* m_httpsClient;

	InvDevUnitTest()
	{
		// m_httpsClient = new Networking::HTTPSClient("test", "test");

	}

	~InvDevUnitTest()
	{
		// delete m_myClass;
	}
};


TEST_F(InvDevUnitTest, firstInvDevTest)
{
	Services::InvDev invDev("Test", "Test");

}