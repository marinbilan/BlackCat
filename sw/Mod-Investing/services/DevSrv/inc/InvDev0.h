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
		return m_PE_Ratio;
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


	// Calculate ...
	// Setters
	void setGrowths(const double& revenueGrowth, const double& netIncomeGrowth, 
		const double& FCFGrowth, const double& avgGrowth)
	{
		m_revenueGrowth = revenueGrowth;
		m_netIncomeGrowth = netIncomeGrowth;
		m_FCFGrowth = FCFGrowth;
		m_avgGrowth = avgGrowth;
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

private:
std::string m_name;

// SUMMARY
double m_stockPrice;
double m_PE_Ratio;
double m_avgGrowth;

// [ INCOME STATEMENT ]
std::vector<double> revenueVec;
std::vector<double> grossProfitVec;
std::vector<double> netIncomeVec;

double m_revenueGrowth;
double m_netIncomeGrowth;

// [ BALANCE SHEET ]
std::vector<double> bookValueVec;
std::vector<double> totalDebtVec;
std::vector<double> shareIssuedVec;

// [ CASH FLOW STATEMENT ]
std::vector<double> freeCashFlowVec;

double m_FCFGrowth;
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

	//
	void collectData();

	void calculateData();

	void storeData();

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