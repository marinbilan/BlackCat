#pragma once
#include "InvDevIf.h"

#include <iostream>
#include <cstdint>
#include <numeric>
#include <iomanip>
#include <algorithm>

#include "CommonTypes.h"


namespace Services
{
/// NEW NEW NEW NEW 


/*
1] GET API DATA (DONE)


2] NORMALIZE (per share) (7 vectors)

// Income Statement
	revenue
	revenueQuartal

	netIncomeRatio
	netIncomeRatioQuartal

	netIncome
	IncomeQuartal

// Balance Sheet
	cashAndCashEquivalentsVec;
	cashAndCashEquivalentsQuartalVec;

	totalStockholdersEquityVec;
	totalStockholdersEquityQuartalVec;

	totalDebtVec;
	totalDebtQuartalVec;

// Cash Flow Statement
	freeCashFlowVec;
	freeCashFlowQuartalVec;


3] CALCULATE k VALUEs (values)


4] CALCULATE
	INCOME STATEMENT
		- Net Profit Ratio (k)
		- P/E (k)

	BALANCE SHEET
		- [CALC] Years to return debt (last year) using (k FCF)
		- Cash and Cash Equivalence per share - percentage
		- Total Debt per share (last vec value) - percentage

		- Return on Equity
		- P/B (k)
		
		- Shares Issued Growth (k)

	CASH FLOW STATEMENT

*/

class Data
{
public:
std::string m_period;
double      m_value;

	Data(const std::string& period, double value) :
		m_period(period), m_value(value) {}
};



class Company {
public:
	Company(const std::string& companyTicker) : 
	m_companyTicker(companyTicker) {};

	const std::string& getCompanyTicker() const;

	void setSummary(const std::string& companyName,
		double stockPrice,
		int64_t marketCap,
		double eps,
		double pe,
		int64_t numOfSharesOutstanding);

	// INCOME STATEMENT
	void setIncomeStatement(const Data& revenue,
		const Data& netIncomeRatio,
		const Data& netIncome);

	void setIncomeStatementQuartal(const Data& revenueQuartal,
		const Data& netIncomeRatioQuartal,
		const Data& netIncomeQuartal);

	std::vector<Data>& getRevenueVec();
	std::vector<Data>& getRevenueQuartalVec();

	std::vector<Data>& getNetIncomeRatioVec();
	std::vector<Data>& getNetIncomeVec();


	// BALANCE SHEET
	void setBalanceSheet(const Data& cashAndCashEquivalents,
		const Data& totalStockholdersEquity,
		const Data& totalDebt);

	void setBalanceSheetQuartal(const Data& cashAndCashEquivalentsQuartal,
		const Data& totalStockholdersEquityQuartal,
		const Data& totalDebtQuartal);

	std::vector<Data>& getCashAndCashEqVec();
	std::vector<Data>& getStockholdersEquityVec();
	std::vector<Data>& getTotalDebtVec();

	// CASH FLOW STATEMENT
	void setCashFlowStatement(const Data& freeCashFlow);

	void setCashFlowStatementQuartal(const Data& freeCashFlowQuartal);	

	std::vector<Data>& getFreeCashFlowVec();


	void setRatios(const double& currentRatio, const double& netProfitMargin, const double& returnOnEquity,
		const double& priceToBookRatio, const double& priceEarningsRatio, const double& priceFairValue, const double& dividendYield);


	// NORMALIZATION (per share)
	void normalizeValues();

	void reverseVectors();

	// CALCULATIONS
	// void calculateValues();

	void setCalculatedData(
		double revL,
		double revH,
		double revAvg,
		double revCAGR,

		double netIncRatioL,
		double netIncRatioH,
		double netIncRatioAvg,
		double netIncRatioCAGR,

		double netIncL,
		double netIncH,
		double netIncAvg,
		double netIncCAGR,

		double cashL,
		double cashH,
		double cashAvg,
		double cashCAGR,

		double shEqL,
		double shEqH,
		double shEqAvg,
		double shEqCAGR,

		double totDebtL,
		double totDebtH,
		double totDebtAvg,
		double totDebtCAGR,

		double fcfL,
		double fcfH,
		double fcfAvg,
		double fcfCAGR);

	void setCalculatedValueParams(int PE_Mark, int PB_Mark, int ROE_Mark, int NetMargin_Mark, int DebtToEquity_Mark, int CurrentRatio_Mark, int YrsToRetDebtFCF_Mark,
		int TotalMark, double DebtToEquity_calc, double YrsToRetDebtFCF_calc);

	void setDCFCalculatedValues(double peGrowthRate, double peGrowthPrice, double peGrowthError,
		double zeroGrowthRate, double zeroGrowthPrice, double zeroGrowthRateError, double grahmPrice);

	void printCompanyInfo();


public:
// Summary
std::string m_companyTicker;
std::string m_companyName {};

double  m_stockPrice;
int64_t m_marketCap;
double  m_eps;
double  m_pe;
int64_t m_numOfSharesOutstanding {};

// Income Statement
std::vector<Data> m_revenueVec;
std::vector<Data> m_revenueQuartalVec;
std::vector<Data> m_netIncomeRatioVec;
std::vector<Data> m_netIncomeRatioQuartalVec;
std::vector<Data> m_netIncomeVec;
std::vector<Data> m_netIncomeQuartalVec;

// Balance Sheet
std::vector<Data> m_cashAndCashEquivalentsVec;
std::vector<Data> m_cashAndCashEquivalentsQuartalVec;
std::vector<Data> m_totalStockholdersEquityVec;
std::vector<Data> m_totalStockholdersEquityQuartalVec;
std::vector<Data> m_totalDebtVec;
std::vector<Data> m_totalDebtQuartalVec;

// Cash Flow Statement
std::vector<Data> m_freeCashFlowVec;
std::vector<Data> m_freeCashFlowQuartalVec;

// Ratios
double m_currentRatio {};
double m_netProfitMargin {};    // Check diff
double m_returnOnEquity {};
double m_priceToBookRatio {};
double m_priceEarningsRatio {}; // NOTE: This is EPS
double m_priceFairValue {};
double m_dividendYield {};


// public:
// Calculated Data
double m_revL {};
double m_revH {};
double m_revAvg {};
double m_revCAGR {};

double m_netIncRatioL {};
double m_netIncRatioH {};
double m_netIncRatioAvg {};
double m_netIncRatioCAGR {};

double m_netIncL {};
double m_netIncH {};
double m_netIncAvg {};
double m_netIncCAGR {};

double m_cashL {};
double m_cashH {};
double m_cashAvg {};
double m_cashCAGR {};

double m_shEqL {};
double m_shEqH {};
double m_shEqAvg {};
double m_shEqCAGR {};

double m_totDebtL {};
double m_totDebtH {};
double m_totDebtAvg {};
double m_totDebtCAGR {};

double m_fcfL {};
double m_fcfH {};
double m_fcfAvg {};
double m_fcfCAGR {};


// Calculated Value Marks
int m_PE_Mark; 
int m_PB_Mark; 
int m_ROE_Mark; 
int m_NetMargin_Mark; 
int m_DebtToEquity_Mark; 
int m_CurrentRatio_Mark; 
int m_YrsToRetDebtFCF_Mark;
int m_TotalMark;

double m_DebtToEquity_calc; 
double m_YrsToRetDebtFCF_calc;


// Calculated DCF values
double m_peGrowthRate;
double m_peGrowthPrice;
double m_peGrowthError;

double m_zeroGrowthRate;
double m_zeroGrowthPrice;
double m_zeroGrowthRateError;

double m_grahmPrice;
};







/// NEW NEW NEW NEW 










class Stock
{
public:
	Stock(const std::string& name) : 
		m_name(name),
		m_fullName(),
		m_totalScore(0),
		m_totalScoreFloat(0.0),
		m_totalScoreIncStatement(0),
		m_FCFToPriceRatio(0.0) {}

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



	// -------- API Args --------
	double& getNetProfitRatioAPI() {
		return m_NetProfitRatioAPI;
	}

	double& getReturnOnEquityAPI() {
		return m_ReturnOnEquityAPI;
	}

	double& getFreeCashFlowPerShareAPI() {
		return m_FreeCashFlowPerShareAPI;
	}

	double& getPriceToBookRatioAPI() {
		return m_PriceToBookRatioAPI;
	}

	double& getDCFAPI() {
		return m_DCFAPI;
	}

	// -------- API Args --------



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

	std::vector<double>& getIncomeVec() // Non-const get
	{
		return m_netIncomeVec;
	}

	std::vector<double>& getFreeCashFlowVec() // Non-const get
	{
		return m_freeCashFlowVec;
	}

	std::vector<double>& getShareIssuedVec() // Non-const get
	{
		return m_shareIssuedVec;
	}

	std::vector<double>& getGrossProfitVec() // Non-const get
	{
		return m_grossProfitVec;
	}

	const double& getPERatio() const {

		return m_PERatio;
	}

	double& getPERatio() 
	{
		return m_PERatio;
	}

	double& getStockPrice() 
	{
		return m_stockPrice;
	}

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



	void setBalanceSheetParams(double bookValue, double totalDebt) {
		m_bookValueVec.push_back(bookValue);
		m_totalDebtVec.push_back(totalDebt);
	}

	std::vector<double>& getTotalDebtVec() // Non-const get
	{
		return m_totalDebtVec;
	}

	double& getDCFZeroGr()  {
		return m_DCFzeroGrowth;
	}

	void setIntrValueZeroGrDiff(double val) {
		m_intrValueZeroGrDiff = val;
	}

	double& getDCFPEAvg()  {
		return m_DCFPEAvg;
	}

	void setIntrValuePEGrDiff(double val) {
		m_intrValuePEGrDiff = val;
	}	

	double& getDCFCompanyGrowth()  {
		return m_DCF;
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

	void printStock() {

		std::cout << '\n' << '\n';
		std::cout << "| " << m_fullName << '\n';
		std::cout << "| " << m_name << '\n';
		std::cout << "  Price: " << m_stockPrice << " $    " << "Market Cap: " << m_marketCap << '\n' << '\n';

		std::cout << "Net Profit Ratio:           " << m_NetProfitRatioAPI << '\n';
		std::cout << "Years to Return Debt (FCF): " << m_yearsToReturnDebt << " Yrs" <<'\n';
		std::cout << "Return on Equity (Mgm Eff): " << m_ReturnOnEquityAPI <<'\n';
		std::cout << "PE Ratio:                   " << m_PERatio << "    (Calc = " << m_calculatedPE << ")" << '\n';
		std::cout << "Price to Book (P/B):        " << m_PriceToBookRatioAPI << '\n' << '\n';

		std::cout << "[FCF to Price:               " << m_FCFToPriceRatio  << "]" << '\n';
		std::cout << "Total Debt Per Share ....... " << m_totalDebtPerShare << " $" << " (" << m_stockPrice << " $)" << " (" << m_totalDebtPerSharePercentage << ")" << '\n';
		std::cout << "Shares Issued Growth (k) ... " << m_sharesIssuedGrowht << '\n';
		std::cout << '\n';

		std::cout << "[ INCOME AND FCF STATEMENT ]" << '\n' << '\n';
		std::cout << "(Per Share)" << '\n';
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

		std::cout << ">> (AVG Growth k: " << m_avgGrowth << ") " << "(PE Growth  k: " << m_peRatioGrowth << ") ";

		std::cout << '\n';
		std::cout << "(Avg FCF per sh: " << m_avgFCFPerShare << ") " << "(" << m_FreeCashFlowPerShareAPI << " $) " << "(Desired Return: " << m_returnRate << ")" <<'\n';
		std::cout << "-----------------" << '\n';
		std::cout << "[DCF            = " << m_DCF << " $] [Avg growth rate  = " << m_avgGrowth << "]" << " [DCF Error = " << m_DCFError << "]" << '\n';
		std::cout << "[DCF PE Gr      = " << m_DCFPEAvg << " $] [PE growth rate = " << m_peRatioGrowth << "]" << " [DCF Error = " << m_DCFPeGrErr << "]" << '\n';
		std::cout << "[DCF 0  Gr      = " << m_DCFzeroGrowth << " $] [Zero growth rate = " << "0.0" << "]" << " [DCF Error = " << m_zeroGrError << "]" << '\n';
		std::cout << "[DCF API        = " << m_DCFAPI << " $]" << '\n';
		std::cout << "-----------------" << '\n';
		std::cout << "[Price          = " << m_stockPrice << " $]" << '\n';

		std::cout << "--------" << '\n' << '\n';
	}

	// --------------------------------------------

	// ---- POSTPROCESS POSTPROCESS POSTPROCESS----
	// Remove
	const double& getGrossProfitPerShare() const {

		return m_grossProfitRatio;
	}


	void printStocksByNetProfit(size_t maxStringSize) {
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
		std::cout << str1 << m_NetProfitRatioAPI << '\n';

	}

	const double& getYearsToPayDebt() const {

		return m_yearsToReturnDebt;
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


	const double& getPriceToBookValue() const {

		return m_priceToBookVal;
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

	double& getIntrValueZeroGrDiff()  {
		return m_intrValueZeroGrDiff;
	}

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
		allignmentSize = 15 - std::to_string(m_intrValueZeroGrDiff).length();
		std::string str3(allignmentSize, ' ');

		// [4] Print Price
		std::cout << str3 << m_intrValueZeroGrDiff << '\n';

	}
	// --------------------------------------------


	// [ OTHER ]


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


	std::vector<double>& getBookValueVec() // Non-const get
	{
		return m_bookValueVec;
	}


	double getAvgFCFPerShare() const {
		return m_avgFCFPerShare;
	}



// TODO: Write setter and getter because this is used explicitly

// -------- API --------
double m_NetProfitRatioAPI;
double m_ReturnOnEquityAPI;
double m_FreeCashFlowPerShareAPI;
double m_PriceToBookRatioAPI;

double m_DCFAPI;

// -------- API --------

// ----
float m_totalScoreFloat;

int m_totalScore;  // Balance Sheet Statement
int m_totalScoreIncStatement;  // Income Statement

double m_grossProfitRatio; 
double m_grossProfitRatioSlopeVal; 
double m_FCFToPriceRatio;


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


	// NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW 
	void _new_calculateData(Company& company);

	void _new_calcParameters(std::vector<Data>& dataVec, double& lowVal, double& highVal, double& avgValue, double& CAGR);

	void _new_calcLinearRegressCoeffs(const std::vector<Data>& y, double& a, double& b);

	void _new_calculateValueParams(Company& company);

	void _new_calcLinearValues(const std::vector<Data>& dataVec, double& a, double& b, double& lowValue, double& highValue);

	double _new_CAGR(std::vector<Data>& vec, const double& first_value, const double& last_value);

	void _new_calculatePrice(Company& company);


	// NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW 









	void storeData();


	// ---- POSTPROCESS POSTPROCESS POSTPROCESS----
	void sortStocksByNetProfitMargin();
	void sortStocksByNetProfitMarginForPrint();
	void printStocksByNetProfitMargin();

	void sortStocksByYearsToReturnDebt();
	void sortStocksByYearsToReturnDebtForPrint();
	void printStocksByYearsToReturnDebt();

	void sortStocksByReturnOnEquityRatio();

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

	double _new_calculateK(const double& a, const double& b, std::vector<Data>& vec);

	void calculateGrowth(Stock& stock);

	double calculateK(const double& a, const double& b, std::vector<double>& vec);

	double calculateDCF(const double& incrRate, const double& FCFPerS, double& error);


private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;

std::vector<Stock> m_stocksVec;
std::vector<Company> m_companyVec;
};

} // End of namespace Services