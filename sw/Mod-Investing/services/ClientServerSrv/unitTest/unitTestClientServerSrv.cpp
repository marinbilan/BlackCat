#include <gmock/gmock.h>

#include "ClientServerSrvIf.h"
#include "ClientServerSrv.h"

// using ::testing::AtLeast;

// TEST FIXTURE //
class ClientServerSrvUnitTest : public ::testing::Test
{
public:
// Networking::ClientIf* m_httpsClient;

	ClientServerSrvUnitTest()
	{
		// m_httpsClient = new Networking::HTTPSClient("test", "test");

	}

	~ClientServerSrvUnitTest()
	{
		// delete m_myClass;
	}
};


TEST_F(ClientServerSrvUnitTest, firstClientServerSrvTest)
{
	Services::ClientServerSrv clientServerSrv("Test", "Test");

}