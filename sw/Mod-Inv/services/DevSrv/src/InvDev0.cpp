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
		{ "TMHC"/*, "LGIH", "TPH", "DFH", "DHI", "TOL", "CCS", "MHO", "MTH", "BZH", "LEN", "KBH"*/};
		// { "AAPL", "MSFT"/*, "NVDA", "V" , "META", "BN", "ABBV", "ALLY"*/ };

	std::vector<std::string> stocksVec2 =
		{ "ABNB", "AMZN", "APTV", "AZO", "BBWI", "BBY", "BKNG", "BWA", "CZR", "KMX",
		"CCL", "CMG", "DRI", "DPZ", "DHI", "EBAY", "ETSY", "EXPE,", "F",
		 "GRMN", "GM", "GPC", "HAS", "HLT", "HD", "LVS", "LEN", "LKQ",
		 "LOW", "LULU", "MAR", "MCD", "MGM", "MHK", "NKE", "NCLH", "NVR",
		  "ORLY", "POOL", "PHM", "RL", "ROST", "RCL", "SBUX", "TPR", "TSLA",
		 "TJX", "TSCO", "ULTA", "VFC", "WHR", "WYNN", "YUM" };

	
	std::vector<std::string> stocksVec3 =
		{ "CCL", "CMG", "DRI", "DPZ", "DHI", "EBAY", "ETSY", "EXPE,", "F" };

	std::vector<std::string> stocksVec4 =
		{ "GRMN", "GM", "GPC", "HAS", "HLT", "HD", "LVS", "LEN", "LKQ" };

	std::vector<std::string> stocksVec5 =
		{"LOW", "LULU", "MAR", "MCD", "MGM", "MHK", "NKE", "NCLH", "NVR" };

	std::vector<std::string> stocksVec6 =
		{"ORLY", "POOL", "PHM", "RL", "ROST", "RCL", "SBUX", "TPR", "TSLA" };

	std::vector<std::string> stocksVec7 =
		{"TJX", "TSCO", "ULTA", "VFC", "WHR", "WYNN", "YUM"};



	// WILLIAM VON MUEFFLING
	// =====================
	std::vector<std::string> William_Von_Mueffling_1 = {
		"AVGO", "SPGI", "ADI", "GOOGL", "V", "AMAT", "MSFT", "A", "ICE", "EFX", "CBRE", "TMO", "AMT", "IQV", "ADSK"
	};
	std::vector<std::string> William_Von_Mueffling_2 = {
		"TNET", "BLK", "CME", "FERG", "VRSN", "AON", "FAST", "TSM", "CDW", "IBKR", "ENTG", "SSNC", "ZTS", "LYV", "ECL", "BC" 
	};
	std::vector<std::string> William_Von_Mueffling_3 = {
		"ADBE", "BDX", "MCO", "CP", "CACC", "TW"
	};
	// =====================


	for(const auto& stockName : William_Von_Mueffling_3)
	{
		Stock stock(stockName);

		// Create HTTPSProxy via Factory and get from Container
		FACTORY.getLog()->LOGFILE(LOG "Create HTTPSProxy via Factory and get from Container");
		std::shared_ptr<Services::HTTPSProxySrvIf> objHTTPSProxy = std::make_shared<Services::HTTPSProxySrv>("Test", "Test");


		// [1] Try to get data from Cash Flow Statement
		bool ok = objHTTPSProxy->_getFromCashFlowStatement(stock, true);
		
		if(ok) {  // Get rest	
			// [2] Get data from Income Statement
			objHTTPSProxy->_getFromIncomeStatement(stock, ok);
			// [3] Get data from Balance Sheet
			objHTTPSProxy->_getFromBalanceSheet(stock, ok);
		
		} else {  // Repeat with new regex set

			objHTTPSProxy->_getFromCashFlowStatement(stock, false);
			// IMPORTANT: Values are in thousands!
			objHTTPSProxy->_getFromIncomeStatement(stock, false);
			objHTTPSProxy->_getFromBalanceSheet(stock, false);
		}


		objHTTPSProxy->_getFromSummary(stockName, stock.getStockPrice(), stock.getPERatio());


		// TODO: Remove this and put verification is Stock class
		// TODO: Refactor this using map and iterator
		// Store for processing and postprocessing only if we have all data
		if(stock.getRevenueVec().size() &&
		stock.getGrossProfitVec().size() &&
		stock.getIncomeVec().size() &&
		stock.getBookValueVec().size() &&
		stock.getTotalDebtVec().size() &&
		stock.getShareIssuedVec().size() &&
		stock.getFreeCashFlowVec().size()) {
			FACTORY.getLog()->LOGFILE(LOG "Store " + stock.getName());

			m_stocksVec.push_back(stock);
		}

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

	// ... and rate stock (Start from 1)
	int rate = 1;

	for(auto& s : m_stocksVec) {
		s.m_totalScore += rate;

		++rate;
	}
}


// Final Balance Sheet Sort
void Services::InvDev::sortStocksByFinalScore() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
		return lhs.m_totalScore < rhs.m_totalScore; 
	});

}


// SORT BALANCE SHEET AND INCOME STATEMENT SCORE
void Services::InvDev::sortStocksByBalanceSheetAndIncomeStatementScore() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
		return lhs.getTotalScoreBalanceAndIncomeStatement()< rhs.getTotalScoreBalanceAndIncomeStatement(); 
	});
}





// INTRINSIC VALUE
// Sort stocks by zero growth intrinsic value
void Services::InvDev::sortStocksByZeroGrowthIntrinsicValue() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
		return lhs.getIntrValueZeroGrDiff() < rhs.getIntrValueZeroGrDiff(); 
	});

}



// INCOME STATEMENT
void Services::InvDev::sortStocksByPERatio() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getPERatio() < rhs.getPERatio(); 
		});

	// ... and rate stock (Start from 1)
	int rate = 1;

	
	for(auto& s : m_stocksVec) {
		s.m_totalScoreIncStatement += rate;

		++rate;
	}
}

void Services::InvDev::sortStocksByGrossProfit() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getGrossProfitPerShare() > rhs.getGrossProfitPerShare(); 
		});

	// ... and rate stock (Start from 1)
	int rate = 1;

	
	for(auto& s : m_stocksVec) {
		s.m_totalScoreIncStatement += rate;

		++rate;
	}
}

void Services::InvDev::sortStocksByAvrGrowth() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getAvgGrowht() > rhs.getAvgGrowht(); 
		});

	// ... and rate stock (Start from 1)
	int rate = 1;

	
	for(auto& s : m_stocksVec) {
		s.m_totalScoreIncStatement += rate;

		++rate;
	}
}

// Final Income Statement Sort
void Services::InvDev::sortStocksByFinalIncomeStatementScore() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
		return lhs.m_totalScoreIncStatement < rhs.m_totalScoreIncStatement; 
	});

}



// -------------------
// PRINT BALANCE SHEET
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

void Services::InvDev::printStocksByFinalBalanceSheetScore() {

	std::cout << " ______________________________________" << '\n';
	std::cout << " ======== SHARES BALANCE SHEET SCORE ========" << '\n';
	for(auto s : m_stocksVec) {
		s.printStocksByFinalScr();
	}
}



// PRINT INCOME STATEMENT
void Services::InvDev::printStocksByPERatio() {

	std::cout << " ======== PE RATIO ========" << '\n';
	for(auto s : m_stocksVec) {
		s.printStocksByPE();
	}
}

void Services::InvDev::printStocksByGrossProfit() {

	std::cout << " ======== GROSS PROFIT PER SHARE ========" << '\n';
	for(auto s : m_stocksVec) {
		s.printStocksByGrossProfitPerShare();
	}
}

void Services::InvDev::printStocksByAvgGrowth() {

	std::cout << " ======== AVG GROWTH ========" << '\n';
	for(auto s : m_stocksVec) {
		s.printStocksByAvgGr();
	}
}

void Services::InvDev::printStocksByFinalIncomeStatementScore() {

	std::cout << " ______________________________________" << '\n';
	std::cout << " ======== SHARES INCOME STATEMENT SCORE ========" << '\n';
	for(auto s : m_stocksVec) {
		s.printStocksByFinalIncomeStatementScr();
	}
}


void Services::InvDev::printStocksByBalanceAndIncomeStatement() {

	std::cout << " _________________________________________" << '\n';
	std::cout << " =========== TOTAL VALUE TABLE ===========" << '\n';
	for(auto s : m_stocksVec) {
		s.printStocksByBalanceAndIncomeSt();
	}
	std::cout << " =========================================" << '\n';
}

void Services::InvDev::printStocksByIntrinsicValue() {

	std::cout << " ________________________________________" << '\n';
	std::cout << " ======== SHARES INTRINSIC VALUE ========" << '\n';
	for(auto s : m_stocksVec) {
		s.printStockByIntrinsicValueGr();
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
	double revenueGrowth = calculateK(a, b, stock.getRevenueVec());

	// TODO: Gross Margin k

	// [2] Net Income k (Growth)
	calcLinearRegressCoeffs(stock.getIncomeVec(), a, b);
	double netIncomeGrowth = calculateK(a, b, stock.getIncomeVec());

	// ---- [FREE CASH FLOW STATEMENT] ----
	//
	// [3] FCF k (Growth)
	calcLinearRegressCoeffs(stock.getFreeCashFlowVec(), a, b);

	// Calculate FCF at 4th year (for DCF analysis)
	double FCF4thYear = a + b * stock.getFreeCashFlowVec().size();
	double FCF4thYearPerShare = FCF4thYear / stock.getShareIssuedVec().back();

	double FCFGrowth = calculateK(a, b, stock.getFreeCashFlowVec());

	// [4] Average (Growth)
	double avgGrowth = (revenueGrowth + netIncomeGrowth + FCFGrowth) / 3;
	FACTORY.getLog()->LOGFILE(LOG "avgGrowth " + stock.getName() + ": " + std::to_string(avgGrowth));


	// ----------------
	// [5] Yahoo PE Ratio
	double peRatio = stock.getPERatio();

	// [6] Growth from current point
	double peGrowth = 1 / peRatio;

	// [7] Calculated PE ratio
	// Market Cap (in thousands) = Stock Price * Num of Stocks
	double MarketCap = stock.getStockPrice() * stock.getShareIssuedVec().back();
	// Average Net Income (in thousands) last N years
	double avgNetIncome = std::accumulate(stock.getIncomeVec().begin(), stock.getIncomeVec().end(), 0.0) / stock.getIncomeVec().size();	

	// If avg net income is less then zero 
	// double calculatedPE = MarketCap / avgNetIncome;
	double calculatedPE = (avgNetIncome > 0.0) ? MarketCap / avgNetIncome : 0.1;

	// [8] Calculate average FCF (per share)
	double avgFCF = std::accumulate(stock.getFreeCashFlowVec().begin(), stock.getFreeCashFlowVec().end(), 0.0) / stock.getFreeCashFlowVec().size();
	// avgFCF = (avgFCF > 0.0) ? avgFCF : 0.01;

	double avgFCFPerShare = avgFCF / stock.getShareIssuedVec().back();



	// ---- DCF ----
	// Get this value for particular industry
	double desiredReturn = 0.15;

	// ----
	double upperGrowthError = 0.0;
	double DCFUpperValue = calculateDCF(avgGrowth, avgFCFPerShare, upperGrowthError);
	// ----	
	// If PE ratio growth is higher than 0.095 (9.5%) limit company growth to 0.095% (9.5%) 
	double upperPEGrowthError = 0.0;
	double upperPEGrowth = (peGrowth >= 0.095) ? 0.095 : peGrowth;
	double DCFPEValue = calculateDCF(upperPEGrowth, avgFCFPerShare, upperPEGrowthError);
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
	double BookValueGrowth = calculateK(a, b, stock.getBookValueVec());

	// [11] P/B - (Market Cap)/(Book Value) - All values in thousands
	// TODO: Check this if book value is negative
	double lastYearBookVal = stock.getBookValueVec().back();
	double priceToBookVal = MarketCap / lastYearBookVal;


	// [12] Total Debt per Shate - (Total (Last) Debt / Avrg FCF)
	double totalDebtPerShare = stock.getTotalDebtVec().back() / stock.getShareIssuedVec().back();

	// [13]
	double totalDebtPerSharePercentage = totalDebtPerShare / stock.getStockPrice();


	// [14] Years to Return Debt - (Total Debt / Avg FCF)
	double lastYearTotalDebt = stock.getTotalDebtVec().back();
	// If FCF is negative set 0.01
	avgFCF = (avgFCF > 0.0) ? avgFCF : 0.01;
	double yearsToReturnDebt = lastYearTotalDebt / avgFCF;

	// [15] Issued Shares k
	calcLinearRegressCoeffs(stock.getShareIssuedVec(), a, b);
	double sharesIssuedGrowth = calculateK(a, b, stock.getShareIssuedVec());


	// [16] Calculate Intrinsic Value
	double val = stock.getStockPrice() - stock.getDCFZeroGr();
	stock.setIntrValueZeroGrDiff(val);

	val = stock.getStockPrice() - stock.getDCFPEAvg();
	stock.setIntrValuePEGrDiff(val);

	val = stock.getStockPrice() - stock.getDCFCompanyGrowth();
	stock.setIntrValueCompanyGrDiff(val);


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


void Services::InvDev::calculateTotalScore() {

	for(auto& s : m_stocksVec) {
		s.calculateTotalSc();
	}
}


double Services::InvDev::calculateK(const double& a, const double& b, std::vector<double>& vec) {

	double nextYearVal = a + b * (vec.size() + 1);
	double nextNextYearVal = a + b * (vec.size() + 2);

	// Percentage growth (k)
	double growth = nextNextYearVal / nextYearVal - 1;

	// Two special cases
	if(nextYearVal < 0.0 && nextNextYearVal < 0.0) {
		growth = -growth;
	} else if (nextYearVal < 0.0 && nextNextYearVal > 0.0) {
		growth = -growth;
	} else if (nextYearVal < 0.0 && nextNextYearVal < 0.0 && nextYearVal < nextNextYearVal) {
		growth = -growth;
	}
	

	return growth;
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

