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


TEST_F(ServiceXUnitTest, firstServiceXTest)
{
	Services::ServiceX srvX("Test", "Test");

}