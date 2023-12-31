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
	Stock(const std::string& name) : m_name(name) {}

	bool operator<(const Stock& rhs) {
		return m_avgFCFPerShare < rhs.getAvgFCFPerShare();
	}

	std::string& getName()
	{
		return m_name;
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


	// [ INCOME STATEMENT ]
	std::vector<double>& getRevenueVec()
	{
		return revenueVec;
	}

	std::vector<double>& getGrossProfitVec()
	{
		return grossProfitVec;
	}

	std::vector<double>& getIncomeVec()
	{
		return netIncomeVec;
	}


	// [ BALANCE SHEET ]
	std::vector<double>& getBookValueVec()
	{
		return bookValueVec;
	}

	std::vector<double>& getTotalDebtVec()
	{
		return totalDebtVec;
	}

	std::vector<double>& getShareIssuedVec()
	{
		return shareIssuedVec;
	}


	// [ CASH FLOW STATEMENT ]
	std::vector<double>& getFreeCashFlowVec()
	{
		return freeCashFlowVec;
	}

	double getAvgFCFPerShare() const {
		return m_avgFCFPerShare;
	}

	void calcVecsPerShare() {
		double val = 0.0; 

		std::vector<double>::iterator itGM = grossProfitVec.begin();
		std::vector<double>::iterator itNI = netIncomeVec.begin();
		std::vector<double>::iterator itFCF = freeCashFlowVec.begin();

		for(auto s : revenueVec) {
			// Revenue
			val = s /  shareIssuedVec.back();
			m_revenueVecPerShare.push_back(val);

			// Gross margin
			val = *itGM /  shareIssuedVec.back();
			m_grossProfitVecPerShare.push_back(val);

			// Net Income
			val = *itNI /  shareIssuedVec.back();
			m_netIncomeVecPerShare.push_back(val);

			// Free Cash Flow
			val = *itFCF /  shareIssuedVec.back();
			m_freeCashFlowVecPerShare.push_back(val);

			itGM++;
			itNI++;
			itFCF++;
		}

		// Calculate average per share
		m_avgRevenuePerShare = std::accumulate(m_revenueVecPerShare.begin(), m_revenueVecPerShare.end(), 0.0) / m_revenueVecPerShare.size();
		m_avgGrossProfitPerShare = std::accumulate(m_grossProfitVecPerShare.begin(), m_grossProfitVecPerShare.end(), 0.0) / m_grossProfitVecPerShare.size();
		m_avgNetIncomePerShare = std::accumulate(m_netIncomeVecPerShare.begin(), m_netIncomeVecPerShare.end(), 0.0) / m_netIncomeVecPerShare.size();
		m_avgFCFPerShare = std::accumulate(m_freeCashFlowVecPerShare.begin(), m_freeCashFlowVecPerShare.end(), 0.0) / m_freeCashFlowVecPerShare.size();	
	}


	// Setters
	void setIncomeFCFStatements(const double& revenueGrowth, const double& netIncomeGrowth, 
		const double& FCFGrowth, const double& avgGrowth, const double& peRatioGrowth, 
		const double& calculatedPE, const double& avgFCFPerShare, const double& DCF, 
		const double& DCFPeAvg, const double& DCFPeGrErr, const double& returnRate, const double& DCFError)
	{
		m_revenueGrowth = revenueGrowth;
		m_netIncomeGrowth = netIncomeGrowth;
		m_FCFGrowth = FCFGrowth;
		m_avgGrowth = avgGrowth;

		m_peRatioGrowth = peRatioGrowth;
		m_calculatedPE = calculatedPE;
		m_avgFCFPerShare = avgFCFPerShare;

		m_DCF = DCF;
		m_DCFPEAvg = DCFPeAvg;
		m_DCFPeGrErr = DCFPeGrErr;

		m_returnRate = returnRate;
		m_DCFError = DCFError;
	}

	void setBalanceSheet(const double& bookValueGrowth, const double& priceToBookVal, 
		const double& totalDebtPerShare, const double& yearsToReturnDebt, const double& sharesIssuedGrowht) {
		
		m_bookValueGrowth = bookValueGrowth;
		m_priceToBookVal = priceToBookVal;
		m_totalDebtPerShare = totalDebtPerShare;
		m_yearsToReturnDebt = yearsToReturnDebt;
		m_sharesIssuedGrowht = sharesIssuedGrowht;
	}

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
		std::cout << "[Price     = " << m_stockPrice << "]" << '\n';
		std::cout << "[DCF       = " << m_DCF << " (return rate = " << m_returnRate << ")" << "]" << " [FCF growth rate = " << m_avgGrowth << "]" << " [DCF Error = " << m_DCFError << "]" << '\n';
		std::cout << "[DCF PE Gr = " << m_DCFPEAvg << " (return rate = " << m_returnRate << ")" << "]" << " [FCF growth rate = " << m_peRatioGrowth << "]" << " [DCF Error = " << m_DCFPeGrErr << "]" << '\n'<<'\n';

		std::cout << " ---- [BALANCE SHEET] ----" << '\n';

		std::cout << "[P/B = " << m_priceToBookVal << "] [Book Value k = " << m_bookValueGrowth << "]" << '\n';
		std::cout << "[Years to Return Debt = " << m_yearsToReturnDebt << "] [Total Debt Per Share = " << m_totalDebtPerShare << "]" << '\n';
		std::cout << "--------" << '\n';
		std::cout << "[Shares Issued k = " << m_sharesIssuedGrowht << "]" << '\n';

		std::cout << "=====================================" << '\n';
	}

private:
std::string m_name;

// SUMMARY
double m_stockPrice;

// [ INCOME STATEMENT ]
std::vector<double> revenueVec;
std::vector<double> m_revenueVecPerShare;
std::vector<double> grossProfitVec;
std::vector<double> m_grossProfitVecPerShare;
std::vector<double> netIncomeVec;
std::vector<double> m_netIncomeVecPerShare;
// [ CASH FLOW STATEMENT ]
std::vector<double> freeCashFlowVec;
std::vector<double> m_freeCashFlowVecPerShare;

double m_avgRevenuePerShare;
double m_avgGrossProfitPerShare; 
double m_avgNetIncomePerShare; 
double m_avgFCFPerShare;

// Growth
double m_revenueGrowth;
// TODO Gross Margin
double m_netIncomeGrowth;
double m_FCFGrowth;
double m_avgGrowth;

double m_PERatio;
double m_calculatedPE;
double m_peRatioGrowth;

// DCF Calculations
double m_DCF;
double m_DCFPEAvg;

double m_returnRate;
double m_DCFError;
double m_DCFPeGrErr;


// [ BALANCE SHEET ]
std::vector<double> bookValueVec;
std::vector<double> totalDebtVec;
std::vector<double> shareIssuedVec;

double m_bookValueGrowth;
double m_priceToBookVal;
double m_totalDebtPerShare;
double m_yearsToReturnDebt;
double m_sharesIssuedGrowht;



// ----


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

	void sortStocksByAvgFCFPerShare();

	// private
	bool calcLinearRegressCoeffs(const std::vector<double>& x, 
                                 const std::vector<double>& y,
                                 double& a, 
                                 double& b);

	void calculateGrowth(Stock& stock);

	double calculateDCF(const double& incrRate, const double& FCFPerS, double& error);

private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;

std::vector<Stock> m_stocksVec;
};

} // End of namespace Services