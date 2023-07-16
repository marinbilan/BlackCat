#include <gmock/gmock.h>

#include "TutorialIf.h"
#include "Multithreading.h"

// using ::testing::AtLeast;

// TEST FIXTURE //
class ModernCpp0Test : public ::testing::Test
{
public:
// Networking::ClientIf* m_httpsClient;

	ModernCpp0Test()
	{
		// m_httpsClient = new Networking::HTTPSClient("test", "test");

	}

	~ModernCpp0Test()
	{
		// delete m_myClass;
	}
};


TEST_F(ModernCpp0Test, firstModernCppUnitTest)
{
	// Services::ServiceX srvX("Test", "Test");

}