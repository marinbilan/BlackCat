#include <algorithm>
#include "InitSrv.h"

#include "HTTPSProxySrvIf.h"
#include "HTTPSProxySrv.h"


Services::InitSrv::InitSrv(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::cout << "Services::InitSrv constructor called!" << '\n';
}


Services::InitSrv::~InitSrv()
{
	std::cout << "Services::InitSrv destructor called!" << '\n';
}


const std::string& Services::InitSrv::getName()
{
	// std::cout << "Services::InitSrv preInit() called!" << '\n';

	return m_name;
}



void Services::InitSrv::preInit()
{
	// Get params from DB for this instance
	// std::cout << "InitSrv preInit() called!" << '\n';

	// TODO: FACTORY STUFF

	std::shared_ptr<Services::HTTPSProxySrvIf> objHTTPSProxy = std::make_shared<Services::HTTPSProxySrv>("Test", "Test");


	// [ INCOME STATEMENT ]
	std::vector<double> revenueVec;
	std::vector<double> grossProfitVec;
	std::vector<double> netIncomeVec;

	// Get Data
	objHTTPSProxy->_getFromIncomeStatement("AAPL", revenueVec, grossProfitVec, netIncomeVec);
	
	// Remove first element TTM
	revenueVec.erase(revenueVec.begin());
	grossProfitVec.erase(grossProfitVec.begin());
	netIncomeVec.erase(netIncomeVec.begin());
	// Reverse elems in vec
	std::reverse(revenueVec.begin(), revenueVec.end());
	std::reverse(grossProfitVec.begin(), grossProfitVec.end());
	std::reverse(netIncomeVec.begin(), netIncomeVec.end());

	for(auto s : revenueVec)
	{
		std::cout << "Revenue value: " << s << '\n';
	}

	for(auto s : grossProfitVec)
	{
		std::cout << "Gross Profit value: " << s << '\n';
	}

		for(auto s : netIncomeVec)
	{
		std::cout << "Net income value: " << s << '\n';
	}


	// [ BALANCE SHEET ]
	std::vector<double> bookValueVec;
	std::vector<double> totalDebtVec;
	std::vector<double> shareIssuedVec;

	// Get Data
	objHTTPSProxy->_getFromBalanceSheet("AAPL", bookValueVec, totalDebtVec, shareIssuedVec);

	// DO NOT NEED TO Remove first element TTM
	// Reverse elems in vec
	std::reverse(bookValueVec.begin(), bookValueVec.end());
	std::reverse(totalDebtVec.begin(), totalDebtVec.end());
	std::reverse(shareIssuedVec.begin(), shareIssuedVec.end());

	for(auto s : bookValueVec)
	{
		std::cout << "Book Value: " << s << '\n';
	}

	for(auto s : totalDebtVec)
	{
		std::cout << "Total Debt Value: " << s << '\n';
	}

	for(auto s : shareIssuedVec)
	{
		std::cout << "Shares Issued Value: " << s << '\n';
	}	


	// [ CASH FLOW STATEMENT ]
	std::vector<double> freeCashFlowVec;

	// Get Data
	objHTTPSProxy->_getFromCashFlowStatement("AAPL", freeCashFlowVec);

	// Remove first element TTM
	freeCashFlowVec.erase(freeCashFlowVec.begin());

	// Reverse elems in vec
	std::reverse(freeCashFlowVec.begin(), freeCashFlowVec.end());

	for(auto s : freeCashFlowVec)
	{
		std::cout << "FCF Value: " << s << '\n';
	}
}


void Services::InitSrv::postInit()
{

}