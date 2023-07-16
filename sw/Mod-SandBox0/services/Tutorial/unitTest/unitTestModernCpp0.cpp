#include <gmock/gmock.h>

#include "TutorialIf.h"
#include "Multithreading.h"

// using ::testing::AtLeast;

// TEST FIXTURE //
class MultithreadingUnitTest : public ::testing::Test
{
public:
// Networking::ClientIf* m_httpsClient;

	MultithreadingUnitTest()
	{
		// m_httpsClient = new Networking::HTTPSClient("test", "test");

	}

	~MultithreadingUnitTest()
	{
		// delete m_myClass;
	}
};


TEST_F(MultithreadingUnitTest, firstMultiThreadingTest)
{
	// Services::ServiceX srvX("Test", "Test");

}