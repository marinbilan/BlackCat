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

	// foreach stock ...
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


	// -- Calculate CASH FLOW STATEMENT --
	// Foreach stock
	for(auto s : m_stocksVec)
	{
		std::cout << "Stock name: " << s.getName();

		
	}

}


void Services::InvDev::storeData()
{
	std::cout << "[MB] Services::InvDev storeData ..." << '\n';

}


bool Services::InvDev::calcLinearRegressCoeffs(const std::vector<double>& x, 
                                 const std::vector<double>& y,
                                 double& a, 
                                 double& b)
{
    if(x.size() != y.size())
    {
        // Print some error
        return false;
    }

    double sumX = 0;
    double sumX2 = 0; 
    double sumY = 0; 
    double sumXY = 0;

    // Calculate required sums
    for(int i = 0; i <= x.size(); i++)
    {
        sumX =  sumX + x[i];
        sumX2 = sumX2 + x[i] * x[i];
        sumY =  sumY + y[i];
        sumXY = sumXY + x[i] * y[i];
    }

    // Calculating a and b coeff
    int n = x.size(); // Number of points

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