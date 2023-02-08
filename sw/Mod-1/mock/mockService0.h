#include <gmock/gmock.h>
#include "ServiceIf.h"


class MockService0 : public Service::ServiceIf
{
public:
	MOCK_METHOD(void, preInit, (), (override));

	MOCK_METHOD(void, testFactoryImpl, (), (override));

	MOCK_METHOD(void, setService, (ServiceIf* srvIfPtr), (override));

	MOCK_METHOD(void, testFactory, (), (override));

	MOCK_METHOD(void, setFactoryImplPtr, (Common::FactoryIf& factoryPtr), (override));
};