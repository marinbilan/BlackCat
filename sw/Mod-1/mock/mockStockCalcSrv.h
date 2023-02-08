#include <gmock/gmock.h>
#include "StockCalcSrvIf.h"


class MockStockCalcSrv : public Service::StockCalcSrvIf
{
public:
	MOCK_METHOD(std::string&, getName, (), (override));

	MOCK_METHOD(void, preInit, (), (override));

	MOCK_METHOD(void, postInit, (), (override));
};