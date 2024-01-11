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
		{ "AAPL", /*"ALLY", "LLY", "JNJ"*/ };

	for(auto stockName : stocksVec)
	{
		// std::string stockName(s);
		Stock stock(stockName);

		// Create HTTPSProxy via Factory and get from Container
		std::shared_ptr<Services::HTTPSProxySrvIf> objHTTPSProxy = std::make_shared<Services::HTTPSProxySrv>("Test", "Test");


		// [1] Try to get value from Cash Flow Statement
		bool ok = objHTTPSProxy->_getFromCashFlowStatement(stockName, stock.getFreeCashFlowVec(), true);
		
		if(ok) {  // Get rest	
			std::cout << ">>>> OK " << '\n';	
			// IMPORTANT: Values are in thousands!
			objHTTPSProxy->_getFromIncomeStatement(stockName, stock.getRevenueVec(), stock.getGrossProfitVec(), stock.getIncomeVec(), ok);
			objHTTPSProxy->_getFromBalanceSheet(stockName, stock.getBookValueVec(), stock.getTotalDebtVec(), stock.getShareIssuedVec(), ok);
		
		} else {  // Repeat with new regex set
			std::cout << ">>>> NOT OK " << '\n';

			objHTTPSProxy->_getFromCashFlowStatement(stockName, stock.getFreeCashFlowVec(), false);
			// IMPORTANT: Values are in thousands!
			objHTTPSProxy->_getFromIncomeStatement(stockName, stock.getRevenueVec(), stock.getGrossProfitVec(), stock.getIncomeVec(), false);
			objHTTPSProxy->_getFromBalanceSheet(stockName, stock.getBookValueVec(), stock.getTotalDebtVec(), stock.getShareIssuedVec(), false);
		}

		objHTTPSProxy->_getFromSummary(stockName, stock.getStockPrice(), stock.getPERatio());


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
		std::cout << "xxx calc before" << '\n';
		calculateGrowth(s);
		std::cout << "xxx calc after" << '\n';
	}

}


void Services::InvDev::storeData()
{
	std::cout << "[MB] Services::InvDev storeData ..." << '\n';

}


void Services::InvDev::sortStocksByAvgFCFPerShare() {
	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { return lhs < rhs; });
}

bool Services::InvDev::calcLinearRegressCoeffs(const std::vector<double>& y,
                                 double& a, 
                                 double& b)
{

	std::vector<double> rangeYrs(y.size());
	std::iota(rangeYrs.begin(), rangeYrs.end(), 1); // 1, 2, 3, 4 ...

    double sumX = 0;
    double sumX2 = 0; 
    double sumY = 0; 
    double sumXY = 0;

    // Calculate required sums
    for(int i = 0; i <= rangeYrs.size(); i++)
    {
        sumX =  sumX + rangeYrs[i];
        sumX2 = sumX2 + rangeYrs[i] * rangeYrs[i];
        sumY =  sumY + y[i];
        sumXY = sumXY + rangeYrs[i] * y[i];
    }

    // Calculating a and b coeff
    int n = rangeYrs.size(); // Number of points

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
	// ---- [INCOME STATEMENT] ----
	//
	// [1] Revenue k (Growth)
	double a;
	double b;
	calcLinearRegressCoeffs(stock.getRevenueVec(), a, b);

	double nextYearVal = a + b * (stock.getRevenueVec().size() + 1);  //  5th year
	double nextNextYearVal = a + b * (stock.getRevenueVec().size() + 2);  //  6th year	
	double revenueGrowth = 1 - nextYearVal / nextNextYearVal;


	// [2] Net Income k (Growth)
	calcLinearRegressCoeffs(stock.getIncomeVec(), a, b);

	nextYearVal = a + b * (stock.getIncomeVec().size() + 1);  //  5th year
	nextNextYearVal = a + b * (stock.getIncomeVec().size() + 2);  //  6th year	
	double netIncomeGrowth = 1 - nextYearVal / nextNextYearVal;


	// ---- [FREE CASH FLOW STATEMENT] ----
	//
	// [3] FCF k (Growth)
	calcLinearRegressCoeffs(stock.getFreeCashFlowVec(), a, b);

	// Calculate FCF at 4th year (for DCF analysis)
	double FCF4thYear = a + b * stock.getFreeCashFlowVec().size();
	double FCF4thYearPerShare = FCF4thYear / stock.getShareIssuedVec().back();

	nextYearVal = a + b * (stock.getFreeCashFlowVec().size() + 1);  //  5th year
	nextNextYearVal = a + b * (stock.getFreeCashFlowVec().size() + 2);  //  6th year	
	double FCFGrowth = 1 - nextYearVal / nextNextYearVal;

	// [4] Average (Growth)
	double avgGrowth = (revenueGrowth + netIncomeGrowth + FCFGrowth) / 3;

	// ----
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
    double previousSum = 0.0;
	double nextSum = 0.0;
	
	double incRate = avgGrowth;
	double FCFPS = FCF4thYearPerShare;
	// double nextFCPS = 0.0;
	// How much money (percentage) to make
	
	double interestRate = 0.14;
	double numerator = 1 + interestRate;

	double DCFError = 0.0;

	for (int i = 1; i <= 100; ++i)
	{
	    // std::cout << "FCFPS = " << FCFPS << '\n';
		double summand = FCFPS / pow(numerator, i);
	    
	    // Calculate next FCFPS
	    FCFPS = FCFPS + incRate * FCFPS;
	    
		previousSum = nextSum;
		nextSum = nextSum + summand;

		DCFError = nextSum - previousSum;

		if (DCFError < 0.05)
		{
		    // std::cout << "xxxx INTRINSIC VALUE: " << nextSum << '\n';
		    break;
		}
		// std::cout << "Mem: " << summand << " Sum: " << nextSum << " Diff: " << nextSum - previous_sum << '\n';
		// std::cout << "----" << '\n';
	}
	// ****************

	// DCF
	double DCFGrError = 0.0;
	double DCFAvgGr = calculateDCF(avgGrowth, avgFCFPerShare, DCFGrError);
	
	double DCFPeGrError = 0.0;
	double DCFPEAvgGrowht = calculateDCF(peGrowth, avgFCFPerShare, DCFPeGrError);

	stock.setIncomeFCFStatements(revenueGrowth, netIncomeGrowth, FCFGrowth, avgGrowth, peGrowth, calculatedPE, avgFCFPerShare, 
		DCFAvgGr, DCFPEAvgGrowht, DCFPeGrError, interestRate, DCFGrError);

	// [BALANCE SHEET]
	//
	// [10] Calculate Book value (Equity) k
	calcLinearRegressCoeffs(stock.getBookValueVec(), a, b);

	nextYearVal = a + b * (stock.getBookValueVec().size() + 1);  //  5th year
	nextNextYearVal = a + b * (stock.getBookValueVec().size() + 2);  //  6th year	
	double BookValueGrowth = 1 - nextYearVal / nextNextYearVal;

	// [11] P/B - (Market Cap)/(Book Value) - All values in thousands
	double lastYearBookVal = stock.getBookValueVec().back();
	double priceToBookVal = MarketCap / lastYearBookVal;


	// [12] Total Debt per Shate - (Total (Last) Debt / Avrg FCF)
	double totalDebtPerShare = stock.getTotalDebtVec().back() / stock.getShareIssuedVec().back();


	// [13] Years to Return Debt - (Total Debt / Avg FCF)
	double lastYearTotalDebt = stock.getTotalDebtVec().back();
	double yearsToReturnDebt = lastYearTotalDebt / avgFCF;

	// [14] Issued Shares k
	calcLinearRegressCoeffs(stock.getShareIssuedVec(), a, b);

	nextYearVal = a + b * (stock.getShareIssuedVec().size() + 1);  //  5th year
	nextNextYearVal = a + b * (stock.getShareIssuedVec().size() + 2);  //  6th year	
	double sharesIssuedGrowth = 1 - nextYearVal / nextNextYearVal;

	stock.setBalanceSheet(BookValueGrowth, priceToBookVal, totalDebtPerShare, yearsToReturnDebt, sharesIssuedGrowth);


	stock.calcVecsPerShare();

	stock.printStock();	
}


double Services::InvDev::calculateDCF(const double& incrRate, const double& FCFPerS, double& error)
{
	double incRate = incrRate;
	double FCFPS = FCFPerS;

	// How much money (percentage) to make	
	double interestRate = 0.14;
	double numerator = 1 + interestRate;

    double previousSum = 0.0;
	double nextSum = 0.0;

	double DCFError = 0.0;

	for (int i = 1; i <= 100; ++i)
	{
	    // std::cout << "FCFPS = " << FCFPS << '\n';
		double summand = FCFPS / pow(numerator, i);
	    
	    // Calculate next FCFPS
	    FCFPS = FCFPS + incRate * FCFPS;
	    
		previousSum = nextSum;
		nextSum = nextSum + summand;

		DCFError = nextSum - previousSum;

		if (DCFError < 0.05)
		{
		    // std::cout << "xxxx INTRINSIC VALUE: " << nextSum << '\n';
		    break;
		}
		// std::cout << "Mem: " << summand << " Sum: " << nextSum << " Diff: " << nextSum - previous_sum << '\n';
		// std::cout << "----" << '\n';
	}

	error = DCFError;
	return nextSum;
}













