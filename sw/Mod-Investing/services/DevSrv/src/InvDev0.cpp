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
		{ "TMHC", "LGIH", "TPH", "DFH", "DHI", "TOL", "CCS", "MHO", "MTH", "BZH", "LEN", "KBH"};

	for(auto stockName : stocksVec)
	{
		// std::string stockName(s);
		Stock stock(stockName);

		// Create HTTPSProxy via Factory and get from Container
		std::shared_ptr<Services::HTTPSProxySrvIf> objHTTPSProxy = std::make_shared<Services::HTTPSProxySrv>("Test", "Test");


		// [1] Try to get value from Cash Flow Statement
		bool ok = objHTTPSProxy->_getFromCashFlowStatement(stockName, stock.getFreeCashFlowVec(), true);
		
		if(ok) {  // Get rest	
			// IMPORTANT: Values are in thousands!
			objHTTPSProxy->_getFromIncomeStatement(stockName, stock.getRevenueVec(), stock.getGrossProfitVec(), stock.getIncomeVec(), ok);
			objHTTPSProxy->_getFromBalanceSheet(stockName, stock.getBookValueVec(), stock.getTotalDebtVec(), stock.getShareIssuedVec(), ok);
		
		} else {  // Repeat with new regex set

			objHTTPSProxy->_getFromCashFlowStatement(stockName, stock.getFreeCashFlowVec(), false);
			// IMPORTANT: Values are in thousands!
			objHTTPSProxy->_getFromIncomeStatement(stockName, stock.getRevenueVec(), stock.getGrossProfitVec(), stock.getIncomeVec(), false);
			objHTTPSProxy->_getFromBalanceSheet(stockName, stock.getBookValueVec(), stock.getTotalDebtVec(), stock.getShareIssuedVec(), false);
		}

		objHTTPSProxy->_getFromSummary(stockName, stock.getStockPrice(), stock.getPERatio());


		// TODO: Remove this and put verification is Stock class
		
		// Store only if we have data
		if(stock.getFreeCashFlowVec().size()) m_stocksVec.push_back(stock);

	}
}


void Services::InvDev::calculateData()
{
	std::cout << "[MB] Services::InvDev calculateData ..." << '\n';

	// -- Calculate CASH FLOW STATEMENT --
	// Foreach stock calculate data
	for(auto& s : m_stocksVec)
	{
		calculateGrowth(s);
	}

}


void Services::InvDev::storeData()
{
	std::cout << "[MB] Services::InvDev storeData ..." << '\n';

}


// ---- POSTPROCESS ----
// ---- POSTPROCESS ----
void Services::InvDev::sortStocksByAvgFCFPerShare() {
	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { return lhs < rhs; });
}


// BALANCE SHEET
void Services::InvDev::sortStocksByYearsToReturnDebt() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getYearsToPayDebt() < rhs.getYearsToPayDebt(); 
		});

	// ... and rate debt for each stock (Start from 1)
	int rate = 1;

	for(auto& s : m_stocksVec) {	
		s.m_totalScore += rate;

		++rate;
	}
}


void Services::InvDev::sortStocksByDebtPerSharePrice() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getDebtPerSharePercentage() < rhs.getDebtPerSharePercentage(); 
		});

	// ... and rate
	int rate = 1;

	for(auto& s : m_stocksVec) {
		s.m_totalScore += rate;

		++rate;
	}
}


void Services::InvDev::sortStocksByPriceToBookValue() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getPriceToBookValue() < rhs.getPriceToBookValue(); 
		});

	// ... and rate
	int rate = 1;

	for(auto& s : m_stocksVec) {
		s.m_totalScore += rate;

		++rate;
	}
}


void Services::InvDev::sortStocksBySharesIssuedGrowth() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getStocksIssuedGrowth() < rhs.getStocksIssuedGrowth(); 
		});

	// ... and rate debt for each stock (Start from 1)
	int rate = 1;

	for(auto& s : m_stocksVec) {
		s.m_totalScore += rate;

		++rate;
	}
}


// Final Sort
void Services::InvDev::sortStocksByFinalScore() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
		return lhs.m_totalScore < rhs.m_totalScore; 
	});

}


// PRINT
void Services::InvDev::printStocksByYearsToReturnDebt() {

	std::cout << " ======== YEARS TO RETURN DEBT ========" << '\n';
	for(auto s : m_stocksVec) {
		s.printYearsToReturnDebt();
	}
	std::cout << " ======== ========" << '\n';
}

void Services::InvDev::printStocksByDebtPerSharePercentage() {

	std::cout << " ======== DEBT PER SHARE PERCENTAGE ========" << '\n';
	for(auto s : m_stocksVec) {
		s.printDebtPerSharePercentage();
	}
	std::cout << " ======== ========" << '\n';
}

void Services::InvDev::printStocksByPriceToBookValue() {

	std::cout << " ======== PRICE TO BOOK VALUE ========" << '\n';
	for(auto s : m_stocksVec) {
		s.printPriceToBook();
	}
}

void Services::InvDev::printStocksBySharesIssuedGrowth() {

	std::cout << " ======== SHARES ISSUED GROWTH ========" << '\n';
	for(auto s : m_stocksVec) {
		s.printStocksBySharesIssuedGr();
	}
}

void Services::InvDev::printStocksByFinalScore() {

	std::cout << " ______________________________________" << '\n';
	std::cout << " ======== SHARES FINAL SCORE ========" << '\n';
	for(auto s : m_stocksVec) {
		s.printStocksByFinalScr();
	}
}
// ---- POSTPROCESS ----
// ---- POSTPROCESS ----


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
	// [1] Revenue k (Growth)
	calcLinearRegressCoeffs(stock.getRevenueVec(), a, b);

	double nextYearVal = a + b * (stock.getRevenueVec().size() + 1);  //  5th year
	double nextNextYearVal = a + b * (stock.getRevenueVec().size() + 2);  //  6th year	
	double revenueGrowth = 1 - nextYearVal / nextNextYearVal;

	// TODO: Gross Margin k

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



	// ---- DCF ----
	// Get this value for particular industry
	double desiredReturn = 0.15;

	// ----
	// If growth is higher than 0.095 (9.5%) limit company growth to 0.095% (9.5%) 
	double upperGrowthError = 0.0;
	double upperGrowth = (avgGrowth >= 0.095) ? 0.095 : avgGrowth;
	double DCFUpperValue = calculateDCF(upperGrowth, avgFCFPerShare, upperGrowthError);
	// ----	
	// ----
	double upperPEGrowthError = 0.0;
	double upperPEGrowth = (peGrowth >= 0.095) ? 0.095 : peGrowth;
	double DCFPEValue = calculateDCF(upperPEGrowth, avgFCFPerShare, upperPEGrowthError);
	// ----
	// ----
	double zeroGrowthError = 0.0;
	double zeroGrowth = 0.0;
	double DCFzeroValue = calculateDCF(zeroGrowth, avgFCFPerShare, zeroGrowthError);
	// ----


	stock.setIncomeAndFCFStatements(
		revenueGrowth, 
		netIncomeGrowth, 
		FCFGrowth, 
		avgGrowth, 
		peGrowth, 
		calculatedPE, 
		avgFCFPerShare, 
		//
		desiredReturn, 
		// 
		DCFUpperValue, 
		upperGrowthError,
		DCFPEValue,
		upperPEGrowthError, 
		DCFzeroValue,
		zeroGrowthError);


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

	// [13]
	double totalDebtPerSharePercentage = totalDebtPerShare / stock.getStockPrice();


	// [14] Years to Return Debt - (Total Debt / Avg FCF)
	double lastYearTotalDebt = stock.getTotalDebtVec().back();
	double yearsToReturnDebt = lastYearTotalDebt / avgFCF;

	// [15] Issued Shares k
	calcLinearRegressCoeffs(stock.getShareIssuedVec(), a, b);

	nextYearVal = a + b * (stock.getShareIssuedVec().size() + 1);  //  5th year
	nextNextYearVal = a + b * (stock.getShareIssuedVec().size() + 2);  //  6th year	
	double sharesIssuedGrowth = 1 - nextYearVal / nextNextYearVal;



	stock.setBalanceSheet(
		BookValueGrowth, 
		priceToBookVal, 
		totalDebtPerShare,
		totalDebtPerSharePercentage, 
		yearsToReturnDebt, 
		sharesIssuedGrowth);

	stock.calcVecsPerShare();

	stock.printStock();	
}


double Services::InvDev::calculateDCF(const double& incrRate, const double& FCFPerS, double& error)
{
	double incRate = incrRate;
	double FCFPS = FCFPerS;

	// How much money (percentage) to make	
	double interestRate = 0.15;
	double numerator = 1 + interestRate;

    double previousSum = 0.0;
	double nextSum = 0.0;

	double DCFError = 0.0;

	for (int i = 1; i <= 100; ++i)
	{
		double summand = FCFPS / pow(numerator, i);
	    
	    // Calculate next FCFPS
	    FCFPS = FCFPS + incRate * FCFPS;
	    
		previousSum = nextSum;
		nextSum = nextSum + summand;

		DCFError = nextSum - previousSum;

		if (DCFError < 0.05)
		{
		    break;
		}
	}

	error = DCFError;
	return nextSum;
}

