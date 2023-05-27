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


TEST_F(ServiceXUnitTest, ModernCPP_ServiceXTest)
{
	Services::ServiceX srvX("Test", "Test");

	// Create Widget
	Services::Widget w(0);

}