#include <gmock/gmock.h>

#include "MasterSrvIf.h"
#include "MasterSrv.h"

// using ::testing::AtLeast;

// TEST FIXTURE //
class MasterSrvUnitTest : public ::testing::Test
{
public:
// Networking::ClientIf* m_httpsClient;

	MasterSrvUnitTest()
	{
		// m_httpsClient = new Networking::HTTPSClient("test", "test");

	}

	~MasterSrvUnitTest()
	{
		// delete m_myClass;
	}
};


TEST_F(MasterSrvUnitTest, firstMasterSrvTest)
{
	Services::MasterSrv masterSrv("Test", "Test");

}