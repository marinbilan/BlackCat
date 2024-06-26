#pragma once
#include "InvDevIf.h"

#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>


namespace Services
{

class Stock
{
public:
	Stock(const std::string& name) : 
		m_name(name), 
		m_totalScore(0), 
		m_totalScoreIncStatement(0) {}

	bool operator<(const Stock& rhs) {
		return m_avgFCFPerShare < rhs.getAvgFCFPerShare();
	}

	// Get Data START
	// SUMMARY
	std::string& getName()
	{
		return m_name;
	}

	double& getStockPrice() 
	{
		return m_stockPrice;
	}

	double& getPERatio() 
	{
		return m_PERatio;
	}


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

	std::vector<double>& getShareIssuedVec() // Non-const get/set
	{
		return shareIssuedVec;
	}


	// [ BALANCE SHEET ]
	std::vector<double>& getBookValueVec() // Non-const get/set
	{
		return bookValueVec;
	}

	std::vector<double>& getTotalDebtVec() // Non-const get/set
	{
		return totalDebtVec;
	}


	// [ CASH FLOW STATEMENT ]
	std::vector<double>& getFreeCashFlowVec() // Non-const get/set
	{
		return freeCashFlowVec;
	}

	// ---- Get Data END ----


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
		const double& zeroGrowthError)
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
	}


	// INTRINSIC VALUE
	double& getDCFZeroGr()  {
		return m_DCFzeroGrowth;
	}

	double& getDCFPEAvg()  {
		return m_DCFPEAvg;
	}

	double& getDCFCompanyGrowth()  {
		return m_DCF;
	}


	//
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


	void printStock() {

		std::cout << "=====================================" << '\n';
		std::cout << "Stock: " << m_name << '\n' << '\n';

		std::cout << " ---- [INCOME AND FCF STATEMENT] ----" << '\n';
		// --
		std::cout << "(Per Share)" << '\n';
		std::cout << "[REVENUE]       [AVG = " << m_avgRevenuePerShare << "]" << "[k = " << m_revenueGrowth << "] " << "[";
		for(auto s : m_revenueVecPerShare) {
			std::cout << s << " ";
		}
		std::cout << "]" << '\n';
		// --
		std::cout << "[GROSS PROFIT]  [AVG = " << m_avgGrossProfitPerShare << "]" << "[";
		for(auto s : m_grossProfitVecPerShare) {
			std::cout << s << " ";
		}
		std::cout << "]" << '\n';
		// --
		std::cout << "[NET INCOME]    [AVG = " << m_avgNetIncomePerShare << "]" << "[k = " << m_netIncomeGrowth << "] " << "[";
		for(auto s : m_netIncomeVecPerShare) {
			std::cout << s << " ";
		}
		std::cout << "]" << '\n';
		// --
		std::cout << "[FREE CASH FLOW][AVG = " << m_avgFCFPerShare << "]" << "[k = " << m_FCFGrowth << "] " << "[";
		for(auto s : m_freeCashFlowVecPerShare) {
			std::cout << s << " ";
		}
		std::cout << "]" << '\n';
		std::cout << "[AVG Growth k = " << m_avgGrowth << "]" << '\n';
		std::cout << "[PE Growth  k = " << m_peRatioGrowth << "][PE Ratio Yahoo = " << m_PERatio;
		std::cout << ", PE Ratio Calc = " << m_calculatedPE << "]" << '\n';
		std::cout << "--------" << '\n';
		std::cout << "[DCF       = " << m_DCF << " (Return = " << m_returnRate << ")" << "]" << " [FCF growth rate = " << m_avgGrowth << "]" << " [DCF Error = " << m_DCFError << "]" << '\n';
		std::cout << "[DCF PE Gr = " << m_DCFPEAvg << " (Return = " << m_returnRate << ")" << "]" << " [FCF growth rate = " << m_peRatioGrowth << "]" << " [DCF Error = " << m_DCFPeGrErr << "]" << '\n';
		std::cout << "[DCF 0  Gr = " << m_DCFzeroGrowth << " (Return = " << m_returnRate << ")" << "]" << " [FCF growth rate = " << "0.0" << "]" << " [DCF Error = " << m_zeroGrError << "]" << '\n';
		std::cout << "[Price     = " << m_stockPrice << "]" << '\n' << '\n';

		std::cout << " ---- [BALANCE SHEET] ----" << '\n';

		std::cout << "[P/B = " << m_priceToBookVal << "] [Book Value k = " << m_bookValueGrowth << "]" << '\n';
		std::cout << "[Years to Return Debt = " << m_yearsToReturnDebt << "] [Total Debt Per Share = " << m_totalDebtPerShare << "]" << '\n';
		std::cout << "--------" << '\n';
		std::cout << "[Shares Issued k = " << m_sharesIssuedGrowht << "]" << '\n';

		std::cout << "=====================================" << '\n';
	}


	// 1st PART DONE


	// ---- POSTPROCESS ----
	// ---- POSTPROCESS ----

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



	// TOTAL SCORE
	int& getTotalScoreBalanceAndIncomeStatement() {
		return m_totalScoreBalanceAndIncomeStatement;
	}

	void setTotalScoreBalanceAndIncomeStatement(int val) {
		m_totalScoreBalanceAndIncomeStatement = val;
	}
	//


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

	void calculateTotalSc() {
		m_totalScoreBalanceAndIncomeStatement = m_totalScore + m_totalScoreIncStatement;
	}


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


	void printYearsToReturnDebt() {
		std::cout << "[Stock: " << m_name << "] [Yrs to Ret Debt = " << m_yearsToReturnDebt << "]" << '\n';
	}

	void printDebtPerSharePercentage() {
		std::cout << "[Stock: " << m_name << "] [Debt Per Share = " << m_totalDebtPerSharePercentage << "]" << '\n';
	}

	void printPriceToBook() {
		std::cout << "[Stock: " << m_name << "] [Price To Book = " << m_priceToBookVal<< "]" << '\n';
	}

	void printStocksBySharesIssuedGr() {
		std::cout << "[Stock: " << m_name << "] [Issued Shares Growth = " << m_sharesIssuedGrowht<< "]" << '\n';
	}

	void printStocksByFinalScr() {
		std::cout << "[Stock: " << m_name << "]" <<  
					  "[Score = " << m_totalScore<< "]" << 
					  "[Yrs To Pay Dbt = " << m_yearsToReturnDebt << "]" <<
					  "[Debt To Price = " << m_totalDebtPerSharePercentage << "]" <<
					  "[Price To Book = " << m_priceToBookVal << "]" <<
					  "[Shares Issued k = " << m_sharesIssuedGrowht << "]" << '\n';

	}


	void printStocksByFinalIncomeStatementScr() {
		std::cout << "[Stock: " << m_name << "]" <<  
					 "[Score = " << m_totalScoreIncStatement << "]" << 
					  "[PE Ratio = " << m_PERatio << "]" <<
					  "[Gross Margin = " << m_avgGrossProfitPerShare << "]" <<
					  "[Avg Growth = " << m_avgGrowth << "]" << '\n';

	}


	// PRINT INCOME STATEMENT
	void printStocksByPE() {
		std::cout << "[Stock: " << m_name << "] [PE Ratio = " << m_PERatio << "]" << '\n';
	}

	void printStocksByGrossProfitPerShare() {
		std::cout << "[Stock: " << m_name << "] [Gross Profit per Share = " << m_avgGrossProfitPerShare << "]" << '\n';
	}

	void printStocksByAvgGr() {
		std::cout << "[Stock: " << m_name << "] [Avg Growth = " << m_avgGrowth << "]" << '\n';
	}

	
	void printStocksByBalanceAndIncomeSt() {
		std::cout << "[Stock: " << m_name << "] [Total Points = " << m_totalScoreBalanceAndIncomeStatement << "]" << '\n';
	}

	// INTRINSIC VALUE
	void printStockByIntrinsicValueGr() {
		std::cout << "[Stock: " << m_name << "]" <<  
					"[Zero Gr Diff = " << m_intrValueZeroGrDiff << "]" << 
					"[PE Gr Diff = " << m_intrValuePEGrDiff << "]" <<
					"[Company Gr Diff = " << m_intrValueCompanyGrDiff << "]" << '\n';
	}



// TODO: Write setter and getter because this is used explicitly
// ----
int m_totalScore;  // Balance Sheet Statement
int m_totalScoreIncStatement;  // Income Statement
int m_totalScoreBalanceAndIncomeStatement;  // Total Score


private:
std::string m_name;
double m_stockPrice;
double m_PERatio;


// [ INCOME STATEMENT ]
std::vector<double> revenueVec;
std::vector<double> grossProfitVec;
std::vector<double> netIncomeVec;
std::vector<double> shareIssuedVec;

// Per share vector
std::vector<double> m_revenueVecPerShare;
std::vector<double> m_grossProfitVecPerShare;
std::vector<double> m_netIncomeVecPerShare;
// Per share average
double m_avgRevenuePerShare;
double m_avgGrossProfitPerShare; 
double m_avgNetIncomePerShare; 


// [ BALANCE SHEET ]
std::vector<double> bookValueVec;
std::vector<double> totalDebtVec;

double m_bookValueGrowth;
double m_priceToBookVal;
double m_totalDebtPerShare;
double m_totalDebtPerSharePercentage;
double m_yearsToReturnDebt;
double m_sharesIssuedGrowht;


// [ CASH FLOW STATEMENT ]
std::vector<double> freeCashFlowVec;

// Per share vector
std::vector<double> m_freeCashFlowVecPerShare;
// Per share average
double m_avgFCFPerShare;



// Growth
double m_revenueGrowth;
// TODO Gross Margin
double m_netIncomeGrowth;
double m_FCFGrowth;
double m_avgGrowth;


double m_calculatedPE;

double m_peRatioGrowth;



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

	void calculateGrowth(Stock& stock);

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

	

	double calculateDCF(const double& incrRate, const double& FCFPerS, double& error);


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;

std::vector<Stock> m_stocksVec;
};

} // End of namespace Services