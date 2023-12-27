#include <math.h>
#include "InvDev0.h"

#include "HTTPSProxySrvIf.h"
#include "HTTPSProxySrv.h"

#include "Factory.h"


Services::InvDev::InvDev(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::string trace = "[MB][MasterSrv][InvDev] Services::InvDev constructor";
	Common::Factory::Factory::getInstance().getClientServerSrv()->TRACE(trace);

	trace = "[MB][MasterSrv][InvDev] Services::InvDev name: " + m_name;
	Common::Factory::Factory::getInstance().getClientServerSrv()->TRACE(trace);
}


Services::InvDev::~InvDev()
{
	std::cout << "Services::InvDev destructor called!" << '\n';
}


const std::string& Services::InvDev::getName()
{
	// std::cout << "Services::InvDev preInit() called!" << '\n';

	return m_name;
}


void Services::InvDev::preInit()
{
	// Get params from DB for this instance
	// std::cout << "InvDev preInit() called!" << '\n';
}


void Services::InvDev::postInit()
{

}


void Services::InvDev::collectData()
{
	std::cout << "[MB] Services::InvDev collectData ..." << '\n';

	// foreach stock ...
	std::vector<std::string> stocksVec = 
		{ "AAPL", "MSFT" };

	for(auto stockName : stocksVec)
	{
		// std::string stockName(s);
		Stock stock(stockName);

		// Create HTTPSProxy via Factory and get from Container
		std::shared_ptr<Services::HTTPSProxySrvIf> objHTTPSProxy = std::make_shared<Services::HTTPSProxySrv>("Test", "Test");

		objHTTPSProxy->_getFromSummary(stockName, stock.getStockPrice(), stock.getPERatio());
		// IMPORTANT: Values are in thousands!
		objHTTPSProxy->_getFromIncomeStatement(stockName, stock.getRevenueVec(), stock.getGrossProfitVec(), stock.getIncomeVec());
		objHTTPSProxy->_getFromBalanceSheet(stockName, stock.getBookValueVec(), stock.getTotalDebtVec(), stock.getShareIssuedVec());

		double shrIssued = stock.getShareIssuedVec().back();

		std::vector<double> freeCashFlowVec;
		objHTTPSProxy->_getFromCashFlowStatement(stockName, stock.getFreeCashFlowVec());

		// Store data
		
		// stock.printStockInfo();

		m_stocksVec.push_back(stock);

	}
}


void Services::InvDev::calculateData()
{
	std::cout << "[MB] Services::InvDev calculateData ..." << '\n';


	// -- Calculate CASH FLOW STATEMENT --
	// Foreach stock calculate data
	for(auto s : m_stocksVec)
	{
		std::cout << "Calculate data for stock: " << s.getName() << '\n';

		// 1] Calculate Growth
		calculateGrowth(s);

	}

}


void Services::InvDev::storeData()
{
	std::cout << "[MB] Services::InvDev storeData ..." << '\n';

}


void Services::InvDev::sortStocksByAvgFCFPerShare() {
	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { return lhs < rhs; });
}

bool Services::InvDev::calcLinearRegressCoeffs(const std::vector<double>& x, 
                                 const std::vector<double>& y,
                                 double& a, 
                                 double& b)
{
    if(x.size() != y.size())
    {
        // Print some error
        return false;
    }

    double sumX = 0;
    double sumX2 = 0; 
    double sumY = 0; 
    double sumXY = 0;

    // Calculate required sums
    for(int i = 0; i <= x.size(); i++)
    {
        sumX =  sumX + x[i];
        sumX2 = sumX2 + x[i] * x[i];
        sumY =  sumY + y[i];
        sumXY = sumXY + x[i] * y[i];
    }

    // Calculating a and b coeff
    int n = x.size(); // Number of points

    b = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    a = (sumY - b * sumX) / n;

    // LINEAR FUNCTION: y = a + b * x
 
    // Test data
    // std::vector<double> x = {1, 2, 3, 4};
    // std::vector<double> y = {265595, 260174, 274515, 365817};

    /*
    double year5 = a + b * 5;  //  5th year
    double year6 = a + b * 6;  //  6th year
    */

    return true;

}


void Services::InvDev::calculateGrowth(Stock& stock)
{
	// Do some check that this is legal call - vec != 0

	std::vector<double> years = {1, 2, 3, 4};

	double a;
	double b;

	// ---- [INCOME STATEMENT] ----
	//
	// [1] Revenue k (Growth)
	calcLinearRegressCoeffs(years, stock.getRevenueVec(), a, b);

	double year5 = a + b * 5;  //  5th year
	double year6 = a + b * 6;  //  6th year
	
	double revenueGrowth = 1 - year5 / year6;

	// [2] Net Income k (Growth)
	calcLinearRegressCoeffs(years, stock.getIncomeVec(), a, b);

	year5 = a + b * 5;  //  5th year
	year6 = a + b * 6;  //  6th year

	double netIncomeGrowth = 1 - year5 / year6;

	// ---- [FREE CASH FLOW STATEMENT] ----
	//
	// [3] FCF k (Growth)
	calcLinearRegressCoeffs(years, stock.getFreeCashFlowVec(), a, b);

	year5 = a + b * 5;  //  5th year
	year6 = a + b * 6;  //  6th year

	double FCFGrowth = 1 - year5 / year6;

	// [4] Average (Growth)
	double avgGrowth = (revenueGrowth + netIncomeGrowth + FCFGrowth) / 3;

	// [5] Yahoo PE Ratio
	double peRatio = stock.getPERatio();

	// [6] Growth from current point
	double peGrowth = 1 / peRatio;

	// [7] Calculated PE ratio
	// Market Cap (in thousands) = Stock Price * Num of Stocks
	double MarketCap = stock.getStockPrice() * stock.getShareIssuedVec().back();
	// Average Net Income (in thousands) last N years
	double avgNetIncome = std::accumulate(stock.getIncomeVec().begin(), stock.getIncomeVec().end(), 0.0) / stock.getIncomeVec().size();	
	double calculatedPE = MarketCap / avgNetIncome;

	// [8] Calculate average FCF (per share)
	double avgFCF = std::accumulate(stock.getFreeCashFlowVec().begin(), stock.getFreeCashFlowVec().end(), 0.0) / stock.getFreeCashFlowVec().size();
	double avgFCFPerShare = avgFCF / stock.getShareIssuedVec().back();

	// ****************
	// [9] DCF - Intrinsic value (for 10(%), 20(%), 25(%), 0(%))

	double previous_sum = 0.0;
	double sum = 0.0;
	// This is constant - should be calc for each year (linear)
	double FCFPS = 8.973;
	// How much money (percentage) to make
	double interest_rate = 0.13;
	double num = 1 + interest_rate;

	for (int i = 1; i <= 100; ++i)
	{
		double member = FCFPS / pow(num, i);

		previous_sum = sum;
		sum = sum + member;

		if ((sum - previous_sum) < 0.05)
		{
		// std::cout << "xxxx INTRINSIC VALUE: " << sum << '\n';
		// break;
		}
		// std::cout << "Mem: " << member << " Sum: " << sum << " Diff: " << sum - previous_sum << '\n';
		// std::cout << "----" << '\n';
	}
	// ****************

	stock.setIncomeFCFStatements(revenueGrowth, netIncomeGrowth, FCFGrowth, avgGrowth, peGrowth, calculatedPE, avgFCFPerShare);


	// [BALANCE SHEET]
	//
	// [10] Calculate Book value (Equity) k
	calcLinearRegressCoeffs(years, stock.getBookValueVec(), a, b);
	year5 = a + b * 5;  //  5th year
	year6 = a + b * 6;  //  6th year

	double BookValueGrowth = 1 - year5 / year6;

	// [11] P/B - (Market Cap)/(Book Value) - All values in thousands
	double lastYearBookVal = stock.getBookValueVec().back();
	double priceToBookVal = MarketCap / lastYearBookVal;
	
	// [12] Total Debt per Shate - (Total (Last) Debt / Avrg FCF)
	double totalDebtPerShare = stock.getTotalDebtVec().back() / stock.getShareIssuedVec().back();

	// [13] Years to Return Debt - (Total Debt / Avg FCF)
	double lastYearTotalDebt = stock.getTotalDebtVec().back();
	double yearsToReturnDebt = lastYearTotalDebt / avgFCF;

	// [14] Issued Shares k
	calcLinearRegressCoeffs(years, stock.getShareIssuedVec(), a, b);
	year5 = a + b * 5;  //  5th year
	year6 = a + b * 6;  //  6th year

	double sharesIssuedGrowth = 1 - year5 / year6;

	stock.setBalanceSheet(BookValueGrowth, priceToBookVal, totalDebtPerShare, yearsToReturnDebt, sharesIssuedGrowth);




	stock.printStock();
}