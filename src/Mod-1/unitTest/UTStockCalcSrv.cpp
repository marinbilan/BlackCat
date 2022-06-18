#include <gmock/gmock.h>
#include <string>
#include <memory>
#include <map>

#include "StockCalcSrvIf.h"
#include "StockCalcSrv.h"

#include "mockStockCalcSrv.h"


// using ::testing::AtLeast;

// TEST FIXTURE //
class StockCalcSrvTest : public ::testing::Test
{
public:

	StockCalcSrvTest()
	{
		std::string dbPath("dbPath");
		std::string instName("instName");
		stockCalcUniqViaIf = std::make_unique<Service::StockCalcSrv>(dbPath, instName);

	}

	~StockCalcSrvTest()
	{

	}

std::unique_ptr<Service::StockCalcSrvIf> stockCalcUniqViaIf;
};


TEST_F(StockCalcSrvTest, DISABLED_stockCalcSrvTest)
{
	// Test creating mock
	// MockStockCalcSrv mockStockCalcSrv0;

	// Prepare vectors for test
	std::vector<double> NetIncomeVec = {53'394'000, 45'687'000, 48'351'000, 59'531'000};
	std::vector<double> FCFVec = {70'019'000, 53'497'000, 51'774'000, 64'121'000};
	std::vector<double> TotalRevenueVec = { 233'715'000, 215'639'000, 229'234'000, 265'595'000, 257'310'000, 269'680'000 };

	// Step: 1.1
	stockCalcUniqViaIf->setVectors(NetIncomeVec, FCFVec, TotalRevenueVec);
	double valueFCFdivNI = stockCalcUniqViaIf->calcFCFdivByNetIncomeForEachYear();

	double expectedValue = 1.07079;  // Calculated value
	// EXPECT_EQ(expectedValue, valueFCFdivNI);


	// Step: 1.2 - Projected revenue growth
	double valueRevGrowth = stockCalcUniqViaIf->projectPercentageRevenueGrowth();


	// Step: 1.3 - Projected revenue growth
	// Add next N years in revenu vector
	stockCalcUniqViaIf->projectRevenueNextNYears();


	// Step: 1.4 - Net Income margines (NetIncome / Revenue) foreach year
	stockCalcUniqViaIf->calcNetIncomeMargines();


	// Step: 1.5 - 
	stockCalcUniqViaIf->calcFCFProjectionForNextNYears();


	// ----

	// Step: 2.1 - 

	double interestExpanse = 3'240'000;
    double totalDebt = 102'519'000;
    double pretaxIncome = 72'902'000;
    double taxProvision = 13'372'000;
    double Rf = 0.0232;
    double Beta = 0.89;
    double Rm = 0.10;
    double marketCap = 826'629'000;
    double SharesOutstanding = 4'601'075;
    double perpetualGrowth = 0.025;

	stockCalcUniqViaIf->setParamsForRequiredRateOfReturn(interestExpanse,
		totalDebt,
		pretaxIncome,
		taxProvision,
		Rf,
		Beta,
		Rm,
		marketCap,
		SharesOutstanding,
		perpetualGrowth);

	stockCalcUniqViaIf->calcRequiredRateOfReturn();

	// Test wanted rate of return
	// double rateOfRet = 0.0714505;  // 7.1 %
	double rateOfRet = 0.125;  // 12.5 %
	stockCalcUniqViaIf->setRequiredRateOfReturn(rateOfRet);

	stockCalcUniqViaIf->calcStockFairIntrinsicValue();
}

