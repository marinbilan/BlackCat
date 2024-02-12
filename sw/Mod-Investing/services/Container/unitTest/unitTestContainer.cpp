#include <gmock/gmock.h>

#include "ContainerIf.h"
#include "ObjectsManager.h"

// using ::testing::AtLeast;

// TEST FIXTURE //
class ContainerUnitTest : public ::testing::Test
{
public:
// Networking::ClientIf* m_httpsClient;

	ContainerUnitTest()
	{
		// m_httpsClient = new Networking::HTTPSClient("test", "test");

	}

	~ContainerUnitTest()
	{
		// delete m_myClass;
	}
};


TEST_F(ContainerUnitTest, firstContainerTest)
{
	Services::ObjectsManager objectsManager("Test", "Test");

}