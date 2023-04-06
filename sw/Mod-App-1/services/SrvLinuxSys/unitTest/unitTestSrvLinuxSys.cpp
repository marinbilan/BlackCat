#include <gmock/gmock.h>

#include "SrvLinuxSysIf.h"
#include "SrvLinuxSys.h"

// using ::testing::AtLeast;

// TEST FIXTURE //
class SrvLinuxSysUnitTest : public ::testing::Test
{
public:
// Networking::ClientIf* m_httpsClient;

	SrvLinuxSysUnitTest()
	{
		// m_httpsClient = new Networking::HTTPSClient("test", "test");

	}

	~SrvLinuxSysUnitTest()
	{
		// delete m_myClass;
	}
};


TEST_F(SrvLinuxSysUnitTest, firstSrvLinuxSysTest)
{
	Services::SrvLinuxSys srvLinuxSys("Test", "Test");

}