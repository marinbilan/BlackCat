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

	// Setters
	void setIncomeFCFStatements(const double& revenueGrowth, const double& netIncomeGrowth, 
		const double& FCFGrowth, const double& avgGrowth, const double& peRatioGrowth, 
		const double& calculatedPE, const double& avgFCFPerShare, const double& FCF4thYearPerShare, 
		const double& DCF, const double& returnRate, const double& DCFError)
	{
		m_revenueGrowth = revenueGrowth;
		m_netIncomeGrowth = netIncomeGrowth;
		m_FCFGrowth = FCFGrowth;
		m_avgGrowth = avgGrowth;

		m_peRatioGrowth = peRatioGrowth;
		m_calculatedPE = calculatedPE;
		m_avgFCFPerShare = avgFCFPerShare;
		m_FCF4thYearPerShare = FCF4thYearPerShare;
		m_DCF = DCF;
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
		// [INCOME STATEMENT] [FCF STATEMENT]
		std::cout << "=====================================" << '\n';
		std::cout << "Stock: " << m_name << '\n';

		std::cout << " ---- [INCOME AND FCF STATEMENT] ----" << '\n';
		std::cout << "[AVG k = " << m_avgGrowth << "] [Revenue k = " << m_revenueGrowth;
		std::cout << ", Net Income k = " << m_netIncomeGrowth;
		std::cout << ", FCF k = " << m_FCFGrowth << "]" << '\n';

		std::cout << "[PE Growth = " << m_peRatioGrowth << "] [PE Ratio Yahoo = " << m_PERatio;
		std::cout << ", PE Ratio Calc = " << m_calculatedPE << "]" << '\n';

		std::cout << "[Avg FCF per share = " << m_avgFCFPerShare << "]";
		std::cout << " [FCF 4th year = " << m_FCF4thYearPerShare << "]" << '\n';
		std::cout << "[Stock Price = " << m_stockPrice << "]" << '\n';
		std::cout << "[DCF (return rate = " << m_returnRate << ") = " << m_DCF << "]" << " [FCF growth rate = " << m_avgGrowth << "]" << " [DCF Error = " << m_DCFError << "]" << '\n';

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
std::vector<double> grossProfitVec;
std::vector<double> netIncomeVec;

double m_revenueGrowth;
double m_netIncomeGrowth;
double m_FCFGrowth;
double m_avgGrowth;

double m_PERatio;
double m_calculatedPE;
double m_peRatioGrowth;
double m_FCF4thYearPerShare;
double m_avgFCFPerShare;
double m_DCF;
double m_returnRate;
double m_DCFError;


// [ BALANCE SHEET ]
std::vector<double> bookValueVec;
std::vector<double> totalDebtVec;
std::vector<double> shareIssuedVec;

double m_bookValueGrowth;
double m_priceToBookVal;
double m_totalDebtPerShare;
double m_yearsToReturnDebt;
double m_sharesIssuedGrowht;

// [ CASH FLOW STATEMENT ]
std::vector<double> freeCashFlowVec;

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

private:
std::string m_dbPath;
std::string m_name;
std::string m_dbPathWithName;

std::vector<Stock> m_stocksVec;
};

} // End of namespace Services