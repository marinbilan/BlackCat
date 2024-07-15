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

	return m_name;
}


void Services::InvDev::preInit()
{

}


void Services::InvDev::postInit()
{

}


void Services::InvDev::collectData(const std::vector<std::string>& portfolio)
{
	std::cout << "[MB] Services::InvDev collectData ..." << '\n';

	// foreach stock ...

	for(const auto& stockName : portfolio)
	{
		Stock stock(stockName);

		// Create HTTPSProxy via Factory and get from Container
		FACTORY.getLog()->LOGFILE(LOG "Create HTTPSProxy via Factory and get from Container");
		std::shared_ptr<Services::HTTPSProxySrvIf> objHTTPSProxy = std::make_shared<Services::HTTPSProxySrv>("Test", "Test");


		objHTTPSProxy->_getFromSummary(stock);
		objHTTPSProxy->_getRatios(stock);
		objHTTPSProxy->_getDCF(stock);

		objHTTPSProxy->_getFromIncomeStatement(stock);
		objHTTPSProxy->_getFromBalanceSheet(stock);
		objHTTPSProxy->_getFromCashFlowStatement(stock);


		// Prepare vectors
		stock.reverseVectors();


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
	// Foreach stock calculate data
	for(auto& s : m_stocksVec)
	{
		calculateGrowth(s);
	}

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
	FACTORY.getLog()->LOGFILE(LOG "AvgGrowth " + stock.getName() + ": " + std::to_string(avgGrowth));



	// [] Gross Profit Margin Ratio
	stock.m_grossProfitRatio = stock.getGrossProfitVec().back() / stock.getRevenueVec().back();

	// Calc revenue slope value
	calcLinearRegressCoeffs(stock.getRevenueVec(), a, b);
	double revenueSlopeVal = a + b * stock.getRevenueVec().size();

	calcLinearRegressCoeffs(stock.getGrossProfitVec(), a, b);
	double grossProfitSlopeVal = a + b * stock.getRevenueVec().size();

	stock.m_grossProfitRatioSlopeVal = grossProfitSlopeVal / revenueSlopeVal;



	// ----------------
	// [5] Yahoo PE Ratio
	double peRatio = stock.getPERatio();

	// [6] Growth from current point
	double peGrowth = 1 / peRatio;

	// [7] Calculated PE ratio
	// Market Cap (in thousands) = Stock Price * Num of Stocks
	double marketCap = stock.getStockPrice() * stock.getShareIssuedVec().back();
	// Average Net Income (in thousands) last N years
	double avgNetIncome = std::accumulate(stock.getIncomeVec().begin(), stock.getIncomeVec().end(), 0.0) / stock.getIncomeVec().size();	

	// If avg net income is less then zero 
	// double calculatedPE = marketCap / avgNetIncome;
	double calculatedPE = (avgNetIncome > 0.0) ? marketCap / avgNetIncome : 0.1;

	// [8] Calculate average FCF (per share)
	double avgFCF = std::accumulate(stock.getFreeCashFlowVec().begin(), stock.getFreeCashFlowVec().end(), 0.0) / stock.getFreeCashFlowVec().size();
	// avgFCF = (avgFCF > 0.0) ? avgFCF : 0.01;

	double avgFCFPerShare = avgFCF / stock.getShareIssuedVec().back();

	//
	stock.m_FCFToPriceRatio = avgFCFPerShare / stock.getStockPrice();


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
		zeroGrowthError,
		marketCap);


	// [BALANCE SHEET]
	//
	// [10] Calculate Book value (Equity) k
	calcLinearRegressCoeffs(stock.getBookValueVec(), a, b);
	double BookValueGrowth = calculateK(a, b, stock.getBookValueVec());

	// [11] P/B - (Market Cap)/(Book Value) - All values in thousands
	// TODO: Check this if book value is negative
	double lastYearBookVal = stock.getBookValueVec().back();
	double priceToBookVal = marketCap / lastYearBookVal;


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


void Services::InvDev::storeData()
{
	std::cout << "[MB] Services::InvDev storeData ..." << '\n';

}


// ---- POSTPROCESS POSTPROCESS POSTPROCESS----
void Services::InvDev::sortStocksByNetProfitMargin() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getNetProfitRatioAPI() < rhs.getNetProfitRatioAPI(); 
		});

	float totalScoreFloat = 1.4;
	
	for(auto& s : m_stocksVec) {
		s.m_totalScoreFloat += totalScoreFloat;

		totalScoreFloat += 1.4;
	}
}


void Services::InvDev::sortStocksByNetProfitMarginForPrint() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getNetProfitRatioAPI() > rhs.getNetProfitRatioAPI(); 
		});
}


void Services::InvDev::printStocksByNetProfitMargin() {
	// Find longest stock name
	std::string maxLengthStr = m_stocksVec.front().getFullName();

	for(auto s : m_stocksVec) {
		if(maxLengthStr.length() < s.getFullName().length()) maxLengthStr = s.getFullName();
	}

	std::cout << "______________________" << '\n';
	std::cout << "[ NET PROFIT RATIO ]" << '\n';
	for(auto s : m_stocksVec) {
		s.printStocksByNetProfit(maxLengthStr.length());
	}
	std::cout << '\n';
}

// ----

void Services::InvDev::sortStocksByYearsToReturnDebt() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getYearsToPayDebt() > rhs.getYearsToPayDebt(); 
		});

	float totalScoreFloat = 1.3;

	for(auto& s : m_stocksVec) {
		s.m_totalScoreFloat += totalScoreFloat;

		totalScoreFloat += 1.3;
	}
}


void Services::InvDev::sortStocksByYearsToReturnDebtForPrint() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getYearsToPayDebt() < rhs.getYearsToPayDebt(); 
		});
}


void Services::InvDev::printStocksByYearsToReturnDebt() {
	// Find longest stock name
	std::string maxLengthStr = m_stocksVec.front().getFullName();

	for(auto s : m_stocksVec) {
		if(maxLengthStr.length() < s.getFullName().length()) maxLengthStr = s.getFullName();
	}

	std::cout << "________________________" << '\n';
	std::cout << "[ YEARS TO RETURN DEBT ]" << '\n';
	for(auto s : m_stocksVec) {
		s.printYearsToReturnDebt(maxLengthStr.length());
	}
	std::cout << '\n';
}

// ----

void Services::InvDev::sortStocksByReturnOnEquityRatio() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getReturnOnEquityAPI() < rhs.getReturnOnEquityAPI(); 
		});

	float totalScoreFloat = 1.2;

	
	for(auto& s : m_stocksVec) {
		s.m_totalScoreFloat += totalScoreFloat;

		totalScoreFloat += 1.2;
	}
}

// ----

void Services::InvDev::sortStocksByPERatio() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getPERatio() > rhs.getPERatio(); 
		});

	float totalScoreFloat = 1.1;

	
	for(auto& s : m_stocksVec) {
		s.m_totalScoreFloat += totalScoreFloat;

		totalScoreFloat += 1.1;
	}
}

// ----

void Services::InvDev::sortStocksByPriceToBookValue() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getPriceToBookRatioAPI() > rhs.getPriceToBookRatioAPI(); 
		});

	float totalScoreFloat = 1.0;

	for(auto& s : m_stocksVec) {
		s.m_totalScoreFloat += totalScoreFloat;

		totalScoreFloat += 1.0;
	}
}



//
// FINAL VALUE SCORE
void Services::InvDev::sortStocksByTotalScore() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
		return lhs.m_totalScoreFloat > rhs.m_totalScoreFloat;
	});

}


void Services::InvDev::printStocksByTotalScore() {
	// Find longest stock name
	std::string maxLengthStr = m_stocksVec.front().getFullName();

	for(auto s : m_stocksVec) {
		if(maxLengthStr.length() < s.getFullName().length()) maxLengthStr = s.getFullName();
	}

	std::cout << "_______________" << '\n';
	std::cout << "[ TOTAL SCORE ]" << '\n';
	for(auto s : m_stocksVec) {
		s.printStocksByFinalIncomeStatementScr(maxLengthStr.length());
	}
	std::cout << '\n';
}


// DCF INTRINSIC VALUE
void Services::InvDev::sortStocksByIntrinsicValue() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
		return lhs.getIntrValueZeroGrDiff() < rhs.getIntrValueZeroGrDiff(); 
	});

}


void Services::InvDev::printStocksByIntrinsicValue() {

	// Find longest stock name
	std::string maxLengthStr = m_stocksVec.front().getFullName();

	for(auto s : m_stocksVec) {
		if(maxLengthStr.length() < s.getFullName().length()) maxLengthStr = s.getFullName();
	}

	std::cout << "______________________________" << '\n';
	std::cout << "[ SHARES DCF INTRINSIC VALUE ]" << '\n';

	size_t diff0 = maxLengthStr.size() + 4;
	std::string str0(diff0, ' ');
	std::string str1(2, ' ');
	std::cout << str0 << "[Stock]" << str1 << "[Total Score]" << str1 <<"[Price]" << str1 << 
		"[0 Gr DCF]" << '\n';

	for(auto s : m_stocksVec) {
		s.printStockByIntrinsicValueGr(maxLengthStr.length());
	}
}




// ---- HELPER ----
// ---- HELPER ----
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
