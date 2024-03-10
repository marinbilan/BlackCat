#pragma once
#include "InvDevIf.h"

#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>

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


	// ---- START ----

	// [ INCOME STATEMENT ]
	std::vector<double>& getRevenueVec() // Non-const get/set
	{
		return revenueVec;
	}

	std::vector<double>& getGrossProfitVec() // Non-const get/set
	{
		return grossProfitVec;
	}

	std::vector<double>& getIncomeVec() // Non-const get/set
	{
		return netIncomeVec;
	}
	// --



	// [ BALANCE SHEET ]
	std::vector<double>& getBookValueVec() // Non-const get/set
	{
		return bookValueVec;
	}

	std::vector<double>& getTotalDebtVec() // Non-const get/set
	{
		return totalDebtVec;
	}

	std::vector<double>& getShareIssuedVec() // Non-const get/set
	{
		return shareIssuedVec;
	}
	// --



	// [ CASH FLOW STATEMENT ]
	std::vector<double>& getFreeCashFlowVec() // Non-const get/set
	{
		return freeCashFlowVec;
	}
	// --




	// ---- STOP ----








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

		return m_avgGrossProfitPerShare;
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


		for(auto s : revenueVec) {
			// Revenue
			val = s / shareIssuedVec.back();
			m_revenueVecPerShare.push_back(val);
		}

		for(auto s : grossProfitVec) {
			// Gross margin
			val = s / shareIssuedVec.back();
			m_grossProfitVecPerShare.push_back(val);
		}
		
		for(auto s : netIncomeVec) {
			// Net Income
			val = s / shareIssuedVec.back();
			m_netIncomeVecPerShare.push_back(val);
		}

		for(auto s : freeCashFlowVec) {
			// Free Cash Flow
			val = s / shareIssuedVec.back();
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

	// Setters
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
		for(auto s : revenueVec)
		{
			std::cout << s << " ";
		}
		std::cout << '\n';

		std::cout << "[STOCK] Gross Profit: ";
		for(auto s : grossProfitVec)
		{
			std::cout << s << " ";
		}
		std::cout << '\n';

		std::cout << "[STOCK] Net Income: ";
		for(auto s : netIncomeVec)
		{
			std::cout << s << " ";
		}
		std::cout << '\n';

		// [ BALANCE SHEET ]
		std::cout << "[STOCK] Book value: ";
		for(auto s : bookValueVec)
		{
			std::cout << s << " ";
		}
		std::cout << '\n';

		std::cout << "[STOCK] Total Debt: ";
		for(auto s : totalDebtVec)
		{
			std::cout << s << " ";
		}
		std::cout << '\n';

		std::cout << "[STOCK] Share Issued: ";
		for(auto s : shareIssuedVec)
		{
			std::cout << s << " ";
		}
		std::cout << '\n';

		std::cout << "[STOCK] Free Cash: ";
		for(auto s : freeCashFlowVec)
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
		std::cout << "[Market Cap = " << m_marketCap << " $]" << '\n';
		std::cout << "[Stock Price = " << m_stockPrice << " $]" << '\n' << '\n';

		std::cout << "[ BALANCE SHEET ]" << '\n';
		std::cout << '\n';

		std::cout << "Price to Book (P/B) ......... " << m_priceToBookVal << '\n'; 
		std::cout << "Total Debt Per Share ........ " << m_totalDebtPerShare << " $" << " (" << m_stockPrice << " $)" << " (" << m_totalDebtPerSharePercentage << " x 100 %)" << '\n';
		std::cout << "Years to Return Debt (FCF) .. " << m_yearsToReturnDebt << " Yrs" <<'\n';
		std::cout << '\n';
		std::cout << "Book Value (k) .............. " << m_bookValueGrowth << '\n';
		std::cout << "Shares Issued Growth (k) .... " << m_sharesIssuedGrowht << '\n';
		std::cout << '\n';

		std::cout << "[ INCOME AND FCF STATEMENT ]" << '\n';
		std::cout << "(Per Share)" << '\n';
		std::cout << '\n';

		std::cout << "[REVENUE]" << "        - ";        
		std::cout << "[";
		for(auto s : m_revenueVecPerShare) {
			std::cout << s << " ";
		}
		std::cout << "]" << " [AVG Revenue = " << m_avgRevenuePerShare << " $] " << "[k Revenue = " << m_revenueGrowth << "] " << '\n';
		// std::cout << '\n';
		
		std::cout << "[GROSS PROFIT]" << "   - ";
		std::cout << "[";
		for(auto s : m_grossProfitVecPerShare) {
			std::cout << s << " ";
		}
		std::cout << "]" << " [AVG Gross Profit = " << m_avgGrossProfitPerShare << " $]" << '\n';
		// std::cout << '\n';
		
		std::cout << "[NET INCOME]" << "     - ";
		std::cout << "[";
		for(auto s : m_netIncomeVecPerShare) {
			std::cout << s << " ";
		}
		std::cout << "]" << " [AVG Net Income = " << m_avgNetIncomePerShare << " $]" << "[k Net Income = " << m_netIncomeGrowth << "] " << '\n';
		// std::cout << '\n';
		
		std::cout << "[FREE CASH FLOW]" << " - ";
		std::cout << "[";
		for(auto s : m_freeCashFlowVecPerShare) {
			std::cout << s << " ";
		}
		std::cout << "]" << "[AVG Free Cash Flow = " << m_avgFCFPerShare << " $]" << "[k FCF = " << m_FCFGrowth << "] " << '\n';
		// std::cout << '\n';
		std::cout << "----" << '\n';
		std::cout << ">> [AVG Growth k = " << m_avgGrowth << "]" << " [PE Growth  k = " << m_peRatioGrowth << "] ";
		std::cout << "[PE Ratio Yahoo = " << m_PERatio << ", PE Ratio Calc = " << m_calculatedPE << "]" << '\n';
		std::cout << '\n';
		std::cout << "[Avg FCF per sh = " << m_avgFCFPerShare << "] " << "[Desired Return = " << m_returnRate << "]" <<'\n';
		std::cout << "-----------------" << '\n';
		std::cout << "[DCF            = " << m_DCF << " [Avg growth rate  = " << m_avgGrowth << "]" << " [DCF Error = " << m_DCFError << "]" << '\n';
		std::cout << "[DCF PE Gr      = " << m_DCFPEAvg << " [PE growth rate = " << m_peRatioGrowth << "]" << " [DCF Error = " << m_DCFPeGrErr << "]" << '\n';
		std::cout << "[DCF 0  Gr      = " << m_DCFzeroGrowth << " [Zero growth rate = " << "0.0" << "]" << " [DCF Error = " << m_zeroGrError << "]" << '\n';
		std::cout << "-----------------" << '\n';
		std::cout << "[Price          = " << m_stockPrice << "]" << '\n';

		std::cout << "--------" << '\n';
	}

	void printYearsToReturnDebt() {
		std::cout << "[Stock: " << m_name << " " << m_fullName << "] [Yrs to Ret Debt = " << m_yearsToReturnDebt << "]" << '\n';
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


	void printStocksByFinalIncomeStatementScr() {
		std::cout << "[Stock: " << m_name << " " << m_fullName << "]" <<  
					 "[Score = " << m_totalScoreIncStatement << "]" << 
					  "[PE Ratio = " << m_PERatio << "]" <<
					  "[Gross Margin = " << m_avgGrossProfitPerShare << "]" <<
					  "[Avg Growth = " << m_avgGrowth << "]" << '\n';

	}


	// PRINT INCOME STATEMENT
	void printStocksByPE() {
		std::cout << "[Stock: " << m_name << " " << m_fullName << "] [PE Ratio = " << m_PERatio << "]" << '\n';
	}

	void printStocksByGrossProfitPerShare() {
		std::cout << "[Stock: " << m_name << " " << m_fullName << "] [Gross Profit per Share = " << m_avgGrossProfitPerShare << "]" << '\n';
	}

	void printStocksByAvgGr() {
		std::cout << "[Stock: " << m_name << " " << m_fullName << "] [Avg Growth = " << m_avgGrowth << "]" << '\n';
	}

	
	void printStocksByBalanceAndIncomeSt() {
		std::cout << "[Stock: " << m_name << " " << m_fullName << "] [Total Points = " << m_totalScoreBalanceAndIncomeStatement << "]" << '\n';
	}

	// INTRINSIC VALUE
	void printStockByIntrinsicValueGr() {

		// Calculate Stock Ticker diff
		int diffStock = 51 - m_fullName.length();
		std::string str0(diffStock, ' ');
		std::cout << m_fullName << str0 << m_name; // << "    " << m_totalScoreBalanceAndIncomeStatement << '\n';

		// Calculate Score diff
		int diffScore = 62 - 50 - m_name.length();
		std::string str1(diffScore, ' ');
		std::cout << str1 << m_totalScoreBalanceAndIncomeStatement;

		// Calculate Income diff
		int diffIncome = 74 - 62 - std::to_string(m_totalScoreBalanceAndIncomeStatement).length();
		std::string str2(diffIncome, ' ');
		std::cout << str2 << m_totalScoreIncStatement;

		// Calculate Balance diff
		int diffBalance = 87 - 74 - std::to_string(m_totalScoreIncStatement).length();
		std::string str3(diffBalance, ' ');
		std::cout << str3 << m_totalScore;

		// Calculate Stock Price diff
		int diffStockPrice = 100 - 86 - std::to_string(m_totalScore).length();
		std::string str4(diffStockPrice, ' ');
		std::cout << str4 << m_stockPrice;

		// Calculate 0 Growth diff
		int diff0Growth = 112 - 100 - 5; // - std::to_string(m_stockPrice).length();
		std::string str5(diff0Growth, ' ');
		std::cout << str5 << m_intrValueZeroGrDiff;

		// Calculate PE Growth diff
		int diffPEGrowth = 127 - 112 - 7; // - std::to_string(m_intrValueZeroGrDiff).length();
		std::string str6(diffPEGrowth, ' ');
		std::cout << str6 << m_intrValuePEGrDiff;

		// Calculate Company Growth diff
		int diffCompanyGrowth = 144 - 127 - 5; // - std::to_string(m_intrValuePEGrDiff).length();
		std::string str7(diffCompanyGrowth, ' ');
		std::cout << str7 << m_intrValueCompanyGrDiff << '\n';






		/*
		std::cout << "[" << m_fullName << "] " <<
		             "[Price = " << m_stockPrice << " $] " <<
					 "0 Gr (" << m_intrValueZeroGrDiff << " $) " << 
					 "PE Gr (" << m_intrValuePEGrDiff << " $) " <<
					 "Comp Gr (" << m_intrValueCompanyGrDiff << " $)" << '\n';
		*/			 
	}


// TODO: Write setter and getter because this is used explicitly
// ----
int m_totalScore;  // Balance Sheet Statement
int m_totalScoreIncStatement;  // Income Statement
int m_totalScoreBalanceAndIncomeStatement;  // Total Score


private:
std::string m_name;
std::string m_fullName;

// SUMMARY
double m_stockPrice;

// [ INCOME STATEMENT ]
std::vector<double> revenueVec;
std::vector<double> m_revenueVecPerShare;
std::vector<double> grossProfitVec;
std::vector<double> m_grossProfitVecPerShare;
std::vector<double> netIncomeVec;
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
std::vector<double> freeCashFlowVec;
std::vector<double> m_freeCashFlowVecPerShare;

double m_avgFCFPerShare;


// [ BALANCE SHEET ]
std::vector<double> bookValueVec;
std::vector<double> totalDebtVec;
std::vector<double> shareIssuedVec;

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
	void collectData();

	void calculateData();

	void storeData();




	// ---- POSTPROCESS ----

	// BALANCE SHEET
	void sortStocksByYearsToReturnDebt();
	void sortStocksByDebtPerSharePrice();
	void sortStocksByPriceToBookValue();
	void sortStocksBySharesIssuedGrowth();
	void sortStocksByFinalScore();

	// INCOME STATEMENT
	void sortStocksByPERatio();
	void sortStocksByGrossProfit();
	void sortStocksByAvrGrowth();
	void sortStocksByFinalIncomeStatementScore();

	void sortStocksByZeroGrowthIntrinsicValue();

	void calculateTotalScore();
	void sortStocksByBalanceSheetAndIncomeStatementScore();

	

	// 

	// PRINT BALANCE SHEET
	void printStocksByYearsToReturnDebt();
	void printStocksByDebtPerSharePercentage();
	void printStocksByPriceToBookValue();
	void printStocksBySharesIssuedGrowth();
	void printStocksByFinalBalanceSheetScore();

	// PRINT INCOME STATEMENT
	void printStocksByPERatio();
	void printStocksByGrossProfit();
	void printStocksByAvgGrowth();
	void printStocksByFinalIncomeStatementScore();

	void printStocksByBalanceAndIncomeStatement();

	// INTRINSIC VALUE
	void printStocksByIntrinsicValue();
	// ---- POSTPROCESS ----




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