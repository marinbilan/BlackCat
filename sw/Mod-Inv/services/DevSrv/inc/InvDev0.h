#pragma once
#include "InvDevIf.h"

#include <iostream>

#include <numeric>
#include <iomanip>
#include <algorithm>

#include "CommonTypes.h"


namespace Services
{

class Stock
{
public:
	Stock(const std::string& name) : 
		m_name(name),
		m_fullName(),
		m_totalScore(0),
		m_totalScoreFloat(0.0),
		m_totalScoreIncStatement(0) {}

	bool operator<(const Stock& rhs) {
		return m_avgFCFPerShare < rhs.getAvgFCFPerShare();
	}

	std::string& getName()
	{
		return m_name;
	}

	std::string& getFullName()
	{
		return m_fullName;
	}


	// [ INCOME STATEMENT ]
	// Set
	void setIncomeStatementParams(double revenue, double grossProfit, double netIncome, double sharesIssued) {
		m_revenueVec.push_back(revenue);
		m_grossProfitVec.push_back(grossProfit);
		m_netIncomeVec.push_back(netIncome);
		m_shareIssuedVec.push_back(sharesIssued);
	}


	// Get
	std::vector<double>& getRevenueVec() // Non-const get
	{
		return m_revenueVec;
	}

	std::vector<double>& getGrossProfitVec() // Non-const get
	{
		return m_grossProfitVec;
	}

	std::vector<double>& getIncomeVec() // Non-const get
	{
		return m_netIncomeVec;
	}

	std::vector<double>& getShareIssuedVec() // Non-const get
	{
		return m_shareIssuedVec;
	}


	// [ BALANCE SHEET ]
	// Set
	void setBalanceSheetParams(double bookValue, double totalDebt) {
		m_bookValueVec.push_back(bookValue);
		m_totalDebtVec.push_back(totalDebt);
	}


	// Get
	std::vector<double>& getBookValueVec() // Non-const get
	{
		return m_bookValueVec;
	}

	std::vector<double>& getTotalDebtVec() // Non-const get
	{
		return m_totalDebtVec;
	}


	// [ CASH FLOW STATEMENT ]
	std::vector<double>& getFreeCashFlowVec() // Non-const get
	{
		return m_freeCashFlowVec;
	}



	void reverseVectors() {
		// Income Statement
		std::reverse(m_revenueVec.begin(), m_revenueVec.end());
		std::reverse(m_grossProfitVec.begin(), m_grossProfitVec.end());
		std::reverse(m_netIncomeVec.begin(), m_netIncomeVec.end());
		std::reverse(m_shareIssuedVec.begin(), m_shareIssuedVec.end());
		// Balance Sheet
		std::reverse(m_bookValueVec.begin(), m_bookValueVec.end());
		std::reverse(m_totalDebtVec.begin(), m_totalDebtVec.end());
		// Free Cash Flow
		std::reverse(m_freeCashFlowVec.begin(), m_freeCashFlowVec.end());
	}



	// Sort
	const double& getYearsToPayDebt() const {

		return m_yearsToReturnDebt;
	}

	const double& getDebtPerSharePercentage() const {

		return m_totalDebtPerSharePercentage;
	}

	const double& getPriceToBookValue() const {

		return m_priceToBookVal;
	}

	const double& getStocksIssuedGrowth() const {

		return m_sharesIssuedGrowht;
	}

	const double& getPERatio() const {

		return m_PERatio;
	}

	const double& getGrossProfitPerShare() const {

		// return m_avgGrossProfitPerShare;
		return m_grossProfitRatio;
	}

	const double& getAvgGrowht() const {
		
		return m_avgGrowth;
	}



	// SUMMARY
	double& getStockPrice() 
	{
		return m_stockPrice;
	}

	double& getPERatio() 
	{
		return m_PERatio;
	}


	// INTRINSIC VALUE
	double& getIntrValueZeroGrDiff()  {
		return m_intrValueZeroGrDiff;
	}

	void setIntrValueZeroGrDiff(double val) {
		m_intrValueZeroGrDiff = val;
	}

	double& getIntrValuePEGrDiff()  {
		return m_intrValuePEGrDiff;
	}

	void setIntrValuePEGrDiff(double val) {
		m_intrValuePEGrDiff = val;
	}

	double& getIntrValueCompanyGrDiff()  {
		return m_intrValueCompanyGrDiff;
	}

	void setIntrValueCompanyGrDiff(double val) {
		m_intrValueCompanyGrDiff = val;
	}

	// TOTAL SCORE
	int& getTotalScoreBalanceAndIncomeStatement() {
		return m_totalScoreBalanceAndIncomeStatement;
	}

	void setTotalScoreBalanceAndIncomeStatement(int val) {
		m_totalScoreBalanceAndIncomeStatement = val;
	}

	//
	double& getDCFZeroGr()  {
		return m_DCFzeroGrowth;
	}

	double& getDCFPEAvg()  {
		return m_DCFPEAvg;
	}

	double& getDCFCompanyGrowth()  {
		return m_DCF;
	}




	double getAvgFCFPerShare() const {
		return m_avgFCFPerShare;
	}

	int getTotalScore() const {

		return m_totalScore;
	}

	int getTotalScoreIncomeStatement() const {

		return m_totalScoreIncStatement;
	}

	// TODO: MOVE TO CALC Class
	void calcVecsPerShare() {
		double val = 0.0; 


		for(auto s : m_revenueVec) {
			// Revenue
			val = s / m_shareIssuedVec.back();
			m_revenueVecPerShare.push_back(val);
		}

		for(auto s : m_grossProfitVec) {
			// Gross margin
			val = s / m_shareIssuedVec.back();
			m_grossProfitVecPerShare.push_back(val);
		}
		
		for(auto s : m_netIncomeVec) {
			// Net Income
			val = s / m_shareIssuedVec.back();
			m_netIncomeVecPerShare.push_back(val);
		}

		for(auto s : m_freeCashFlowVec) {
			// Free Cash Flow
			val = s / m_shareIssuedVec.back();
			m_freeCashFlowVecPerShare.push_back(val);
		}
			
		// Calculate average per share
		m_avgRevenuePerShare = std::accumulate(m_revenueVecPerShare.begin(), m_revenueVecPerShare.end(), 0.0) / m_revenueVecPerShare.size();
		m_avgGrossProfitPerShare = std::accumulate(m_grossProfitVecPerShare.begin(), m_grossProfitVecPerShare.end(), 0.0) / m_grossProfitVecPerShare.size();
		m_avgNetIncomePerShare = std::accumulate(m_netIncomeVecPerShare.begin(), m_netIncomeVecPerShare.end(), 0.0) / m_netIncomeVecPerShare.size();
		m_avgFCFPerShare = std::accumulate(m_freeCashFlowVecPerShare.begin(), m_freeCashFlowVecPerShare.end(), 0.0) / m_freeCashFlowVecPerShare.size();	
	}

	void calculateTotalSc() {
		m_totalScoreBalanceAndIncomeStatement = m_totalScore + m_totalScoreIncStatement;
	}


	// Set Calculated Values
	void setIncomeAndFCFStatements(
		const double& revenueGrowth, 
		const double& netIncomeGrowth, 
		const double& FCFGrowth, 
		const double& avgGrowth, 
		const double& peGrowth, 
		const double& calculatedPE, 
		const double& avgFCFPerShare,
		//
		const double& desiredReturn, 
		//
		const double& DCFUpperValue, 
		const double& upperGrowthError,
		const double& DCFPEValue, 
		const double& upperPEGrowthError, 
		const double& DCFzeroValue, 
		const double& zeroGrowthError,
		const double& marketCap)
	{
		m_revenueGrowth = revenueGrowth;
		m_netIncomeGrowth = netIncomeGrowth;
		m_FCFGrowth = FCFGrowth;
		m_avgGrowth = avgGrowth;
		m_peRatioGrowth = peGrowth;
		m_calculatedPE = calculatedPE;
		m_avgFCFPerShare = avgFCFPerShare;
		//
		m_returnRate = desiredReturn;
		//
		m_DCF = DCFUpperValue;
		m_DCFError = upperGrowthError;
		m_DCFPEAvg = DCFPEValue;
		m_DCFPeGrErr = upperPEGrowthError;
		m_DCFzeroGrowth = DCFzeroValue;
		m_zeroGrError = zeroGrowthError;
		m_marketCap = marketCap;
	}


	void setBalanceSheet(
		const double& bookValueGrowth, 
		const double& priceToBookVal, 
		const double& totalDebtPerShare,
		const double& totalDebtPerSharePercentage,  
		const double& yearsToReturnDebt, 
		const double& sharesIssuedGrowht) {
		
		m_bookValueGrowth = bookValueGrowth;
		m_priceToBookVal = priceToBookVal;
		m_totalDebtPerShare = totalDebtPerShare;
		m_totalDebtPerSharePercentage = totalDebtPerSharePercentage;
		m_yearsToReturnDebt = yearsToReturnDebt;
		m_sharesIssuedGrowht = sharesIssuedGrowht;
	}

	//


	// ----
	void printStockInfo()
	{
		// [ INCOME STATEMENT ]
		std::cout << "[STOCK] Revenue: ";
		for(auto s : m_revenueVec)
		{
			std::cout << s << " ";
		}
		std::cout << '\n';

		std::cout << "[STOCK] Gross Profit: ";
		for(auto s : m_grossProfitVec)
		{
			std::cout << s << " ";
		}
		std::cout << '\n';

		std::cout << "[STOCK] Net Income: ";
		for(auto s : m_netIncomeVec)
		{
			std::cout << s << " ";
		}
		std::cout << '\n';

		// [ BALANCE SHEET ]
		std::cout << "[STOCK] Book value: ";
		for(auto s : m_bookValueVec)
		{
			std::cout << s << " ";
		}
		std::cout << '\n';

		std::cout << "[STOCK] Total Debt: ";
		for(auto s : m_totalDebtVec)
		{
			std::cout << s << " ";
		}
		std::cout << '\n';

		std::cout << "[STOCK] Share Issued: ";
		for(auto s : m_shareIssuedVec)
		{
			std::cout << s << " ";
		}
		std::cout << '\n';

		std::cout << "[STOCK] Free Cash: ";
		for(auto s : m_freeCashFlowVec)
		{
			std::cout << s << " ";
		}
		std::cout << '\n';
	}

	void printStock() {

		std::cout << '\n';
		std::cout << "--------" << '\n';
		std::cout << "[STOCK: " << m_name << "]" << '\n';
		std::cout << m_fullName << '\n';
		std::cout << "Market Cap = " << m_marketCap << " $" << '\n';
		std::cout << "Stock Price = " << m_stockPrice << " $" << '\n' << '\n';

		std::cout << "[ BALANCE SHEET ]" << '\n';
		std::cout << '\n';
		std::cout << "Gross Profit Ratio .......... " << m_grossProfitRatio << "   Gross Profit Ratio Slope .......... " << m_grossProfitRatioSlopeVal << '\n';
		std::cout << '\n';	
		std::cout << "Years to Return Debt (FCF) .. " << m_yearsToReturnDebt << " Yrs" <<'\n';
		std::cout << "Total Debt Per Share ........ " << m_totalDebtPerShare << " $" << " (" << m_stockPrice << " $)" << " (" << m_totalDebtPerSharePercentage << " x 100 %)" << '\n';
		std::cout << "Price to Book (P/B) ......... " << m_priceToBookVal << '\n'; 
		std::cout << '\n';
		std::cout << "Book Value (k) .............. " << m_bookValueGrowth << '\n';
		std::cout << "Shares Issued Growth (k) .... " << m_sharesIssuedGrowht << '\n';
		std::cout << '\n';

		std::cout << "[ INCOME AND FCF STATEMENT ]" << '\n' << '\n';
		std::cout << "(Per Share)" << '\n';
		// std::cout << '\n';

		std::cout << "[REVENUE]" << "        - ";        
		std::cout << "[";
		for(auto s : m_revenueVecPerShare) {
			std::cout << s << " ";
		}
		std::cout << "]" << " .... AVG = " << m_avgRevenuePerShare << " $" << " .... k = " << m_revenueGrowth << '\n';
		
		std::cout << "[GROSS PROFIT]" << "   - ";
		std::cout << "[";
		for(auto s : m_grossProfitVecPerShare) {
			std::cout << s << " ";
		}
		std::cout << "]" << " .... AVG = " << m_avgGrossProfitPerShare << " $]" << '\n';
		
		std::cout << "[NET INCOME]" << "     - ";
		std::cout << "[";
		for(auto s : m_netIncomeVecPerShare) {
			std::cout << s << " ";
		}
		std::cout << "]" << " .... AVG = " << m_avgNetIncomePerShare << " $" << " .... k = " << m_netIncomeGrowth << '\n';
		
		std::cout << "[FREE CASH FLOW]" << " - ";
		std::cout << "[";
		for(auto s : m_freeCashFlowVecPerShare) {
			std::cout << s << " ";
		}
		std::cout << "]" << " .... AVG = " << m_avgFCFPerShare << " $" << " .... k = " << m_FCFGrowth << '\n';
		std::cout << "----" << '\n';

		std::cout << ">> [AVG Growth k = " << m_avgGrowth << "]" << " [PE Growth  k = " << m_peRatioGrowth << "] ";
		std::cout << "[PE Ratio = " << m_PERatio << ", PE Ratio Calc = " << m_calculatedPE << "]" << '\n';
		std::cout << '\n';
		std::cout << "[Avg FCF per sh = " << m_avgFCFPerShare << "] " << "[Desired Return = " << m_returnRate << "]" <<'\n';
		std::cout << "-----------------" << '\n';
		std::cout << "[DCF            = " << m_DCF << " [Avg growth rate  = " << m_avgGrowth << "]" << " [DCF Error = " << m_DCFError << "]" << '\n';
		std::cout << "[DCF PE Gr      = " << m_DCFPEAvg << " [PE growth rate = " << m_peRatioGrowth << "]" << " [DCF Error = " << m_DCFPeGrErr << "]" << '\n';
		std::cout << "[DCF 0  Gr      = " << m_DCFzeroGrowth << " [Zero growth rate = " << "0.0" << "]" << " [DCF Error = " << m_zeroGrError << "]" << '\n';
		std::cout << "-----------------" << '\n';
		std::cout << "[Price          = " << m_stockPrice << "]" << '\n';

		std::cout << "--------" << '\n' << '\n';
	}

	void printYearsToReturnDebt(size_t maxStringSize) {
		// [1] Print Full Name
		std::cout << m_fullName;

		// ----
		// Calculate diff
		size_t allignmentSize = maxStringSize - m_fullName.length() + 4;
		std::string str0(allignmentSize, ' ');

		// [2] Print Ticker Name
		std::cout << str0 << m_name;

		// ----
		// Calculate diff
		allignmentSize = 9 - m_name.length();
		std::string str1(allignmentSize, ' ');

		// [3] Print Total Score
		std::cout << str1 << m_yearsToReturnDebt << '\n';

	}

	void printDebtPerSharePercentage() {
		std::cout << "[Stock: " << m_name << " " << m_fullName << "] [Debt Per Share = " << m_totalDebtPerSharePercentage << "]" << '\n';
	}

	void printPriceToBook() {
		std::cout << "[Stock: " << m_name << " " << m_fullName << "] [Price To Book = " << m_priceToBookVal<< "]" << '\n';
	}

	void printStocksBySharesIssuedGr() {
		std::cout << "[Stock: " << m_name << " " << m_fullName << "] [Issued Shares Growth = " << m_sharesIssuedGrowht<< "]" << '\n';
	}

	void printStocksByFinalScr() {
		std::cout << "[Stock: " << m_name << " " << m_fullName << "]" <<  
					  "[Score = " << m_totalScore<< "]" << 
					  "[Yrs To Pay Dbt = " << m_yearsToReturnDebt << "]" <<
					  "[Debt To Price = " << m_totalDebtPerSharePercentage << "]" <<
					  "[Price To Book = " << m_priceToBookVal << "]" <<
					  "[Shares Issued k = " << m_sharesIssuedGrowht << "]" << '\n';

	}


	void printStocksByFinalIncomeStatementScr(size_t maxStringSize) {
		// [1] Print Full Name
		std::cout << m_fullName;

		// ----
		// Calculate diff
		size_t allignmentSize = maxStringSize - m_fullName.length() + 4;
		std::string str0(allignmentSize, ' ');

		// [2] Print Ticker Name
		std::cout << str0 << m_name;

		// ----
		// Calculate diff
		allignmentSize = 9 - m_name.length();
		std::string str1(allignmentSize, ' ');

		// [3] Print Total Score
		std::cout << str1 << m_totalScoreFloat << '\n';

	}


	// PRINT INCOME STATEMENT
	void printStocksByPE() {
		std::cout << "[Stock: " << m_name << " " << m_fullName << "] [" << m_PERatio << "]" << '\n';
	}

	void printStocksByGrossProfitPerShare(size_t maxStringSize) {
		// [1] Print Full Name
		std::cout << m_fullName;

		// ----
		// Calculate diff
		size_t allignmentSize = maxStringSize - m_fullName.length() + 4;
		std::string str0(allignmentSize, ' ');


		// [2] Print Ticker Name
		std::cout << str0 << m_name;

		// ----
		// Calculate diff
		allignmentSize = 9 - m_name.length();
		std::string str1(allignmentSize, ' ');

		// [3] Print Total Score
		std::cout << str1 << m_grossProfitRatio << '\n';

	}

	void printStocksByAvgGr() {
		std::cout << "[Stock: " << m_name << " " << m_fullName << "] [Avg Growth = " << m_avgGrowth << "]" << '\n';
	}

	
	void printStocksByBalanceAndIncomeSt() {
		std::cout << "[Stock: " << m_name << " " << m_fullName << "] [Total Points = " << m_totalScoreBalanceAndIncomeStatement << "]" << '\n';
	}

	// INTRINSIC VALUE
	void printStockByIntrinsicValueGr(size_t maxStringSize) {

		// [1] Print Full Name
		std::cout << m_fullName;

		// ----
		// Calculate diff
		size_t allignmentSize = maxStringSize - m_fullName.length() + 5;
		std::string str0(allignmentSize, ' ');

		// [2] Print Ticker Name
		std::cout << str0 << m_name;

		// ----
		// Calculate diff
		allignmentSize = 9 - m_name.length();
		std::string str1(allignmentSize, ' ');

		// [3] Print Total Score
		std::cout << str1 << m_totalScoreFloat;

		// ----
		// Calculate diff
		allignmentSize = 21 - std::to_string(m_stockPrice).length();
		std::string str2(allignmentSize, ' ');

		// [4] Print Price
		std::cout << str2 << m_stockPrice;

		// ----
		// Calculate diff
		allignmentSize = 15 - std::to_string(m_intrValuePEGrDiff).length();
		std::string str3(allignmentSize, ' ');

		// [4] Print Price
		std::cout << str3 << m_intrValuePEGrDiff << '\n';

	}


// TODO: Write setter and getter because this is used explicitly
// ----
float m_totalScoreFloat;

int m_totalScore;  // Balance Sheet Statement
int m_totalScoreIncStatement;  // Income Statement
int m_totalScoreBalanceAndIncomeStatement;  // Total Score
double m_grossProfitRatio; 
double m_grossProfitRatioSlopeVal; 


private:
std::string m_name;
std::string m_fullName;

// SUMMARY
double m_stockPrice;

// [ INCOME STATEMENT ]
std::vector<double> m_revenueVec;
std::vector<double> m_revenueVecPerShare;
std::vector<double> m_grossProfitVec;
std::vector<double> m_grossProfitVecPerShare;
std::vector<double> m_netIncomeVec;
std::vector<double> m_netIncomeVecPerShare;

double m_avgRevenuePerShare;
double m_avgGrossProfitPerShare; 
double m_avgNetIncomePerShare; 

// Growth
double m_revenueGrowth;
// TODO Gross Margin
double m_netIncomeGrowth;
double m_FCFGrowth;
double m_avgGrowth;

double m_PERatio;
double m_calculatedPE;

double m_peRatioGrowth;
double m_marketCap;


// [ CASH FLOW STATEMENT ]
std::vector<double> m_freeCashFlowVec;
std::vector<double> m_freeCashFlowVecPerShare;

double m_avgFCFPerShare;


// [ BALANCE SHEET ]
std::vector<double> m_bookValueVec;
std::vector<double> m_totalDebtVec;
std::vector<double> m_shareIssuedVec;

double m_bookValueGrowth;
double m_priceToBookVal;
double m_totalDebtPerShare;
double m_totalDebtPerSharePercentage;
double m_yearsToReturnDebt;
double m_sharesIssuedGrowht;


// DCF Calculations
double m_DCF;
double m_DCFError;
double m_DCFPEAvg;
double m_DCFPeGrErr;
double m_DCFzeroGrowth;
double m_zeroGrError;

double m_returnRate;

// ----
// INTRINSIC VALUE
double m_intrValueZeroGrDiff {};
double m_intrValuePEGrDiff {};
double m_intrValueCompanyGrDiff {};

};




class InvDev : public InvDevIf
{
public:
	InvDev(const std::string& dbPath, const std::string& name);

	~InvDev();

	const std::string& getName();

	void preInit();
	void postInit();

	// PROCEDURE
	void collectData(const std::vector<std::string>& portfolio);
	void calculateData();
	void storeData();


	// ---- POSTPROCESS POSTPROCESS POSTPROCESS----
	void sortStocksByGrossProfit();
	void sortStocksByGrossProfitForPrint();
	void printStocksByGrossProfit();

	void sortStocksByYearsToReturnDebt();
	void sortStocksByYearsToReturnDebtForPrint();
	void printStocksByYearsToReturnDebt();

	void sortStocksByPERatio();
	void sortStocksByPriceToBookValue();

	//
	// FINAL VALUE SCORE
	void sortStocksByTotalScore();
	void printStocksByTotalScore();

	// DCF INTRINSIC VALUE
	void sortStocksByIntrinsicValue();
	void printStocksByIntrinsicValue();


	// private
	bool calcLinearRegressCoeffs(const std::vector<double>& y,
                                 double& a, 
                                 double& b);

	void calculateGrowth(Stock& stock);

	double calculateK(const double& a, const double& b, std::vector<double>& vec);

	double calculateDCF(const double& incrRate, const double& FCFPerS, double& error);


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;

std::vector<Stock> m_stocksVec;
};

} // End of namespace Services