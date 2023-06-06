#include "InvDev0.h"

#include "HTTPSProxySrvIf.h"
#include "HTTPSProxySrv.h"


Services::InvDev::InvDev(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::cout << "Services::InvDev constructor called!" << '\n';
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

	// Collect stock names (info)

	// foreach ...
	std::string stockName("AAPL");


	// Create temp stock
	Stock stock(stockName);

	// Create HTTPSProxy via Factory and get from Container
	std::shared_ptr<Services::HTTPSProxySrvIf> objHTTPSProxy = std::make_shared<Services::HTTPSProxySrv>("Test", "Test");


	// IMPORTANT: Values are in thousands!

	// Get INCOME STATEMENT Data
	objHTTPSProxy->_getFromIncomeStatement(stockName, stock.getRevenueVec(), stock.getGrossProfitVec(), stock.getIncomeVec());

	// Get BALANCE SHEET Data
	objHTTPSProxy->_getFromBalanceSheet(stockName, stock.getBookValueVec(), stock.getTotalDebtVec(), stock.getShareIssuedVec());

	double shrIssued = stock.getShareIssuedVec().back();

	// [ CASH FLOW STATEMENT ]
	std::vector<double> freeCashFlowVec;
	// Get CASH FLOW Data
	objHTTPSProxy->_getFromCashFlowStatement(stockName, stock.getFreeCashFlowVec());


	stock.printStockInfo();


	m_stocksVec.push_back(stock);
}


void Services::InvDev::calculateData()
{
	std::cout << "[MB] Services::InvDev calculateData ..." << '\n';


}


void Services::InvDev::storeData()
{
	std::cout << "[MB] Services::InvDev storeData ..." << '\n';

}