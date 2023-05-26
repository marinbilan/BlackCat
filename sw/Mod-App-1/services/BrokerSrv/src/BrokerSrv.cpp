#include <memory>
#include "BrokerSrv.h"

#include "Database.h"
#include "ContainerIf.h"
#include "ObjectsManager.h"
#include "Factory.h"



Services::BrokerSrv::BrokerSrv(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::cout << "Services::BrokerSrv constructor called!" << '\n';
}


Services::BrokerSrv::~BrokerSrv()
{
	std::cout << "Services::BrokerSrv destructor called!" << '\n';
}


const std::string& Services::BrokerSrv::getName()
{
	// std::cout << "Services::BrokerSrv getName() called!" << '\n';

	return m_name;
}



void Services::BrokerSrv::preInit()
{
	// Get here all configuration info from xml file




	// [ FACTORY ] - Create other instances and devices

    // Create and Set Database in Factory (get this info from config xml)
    std::unique_ptr<Common::Database> database = std::make_unique<Common::Database>("Database/database_0.txt");
    Common::Factory::Factory::getInstance().setDatabase(database);

    // Create and Set Objects Manager in Factory (get name infro from config xml)
    // Create this as normal object in Factory
    std::shared_ptr<Services::ContainerIf> objectsManager = std::make_shared<Services::ObjectsManager>("objMgr", "objMgr");
    Common::Factory::Factory::getInstance().setObjectsManager(objectsManager);

    // Register and create instances from database
    Common::Factory::Factory::getInstance().registerClassConstructor();
    Common::Factory::Factory::getInstance().getInterfacesAndCreateInstances();

    // Try to get instances via name

    // Create Master Class in Factory - Access devices via ObjectManager




	// [ CLIENT MESSAGE DISPATCHER SERVICE ]
	// Separate in different thread
	// Connect to MasterBrokerMod






	// std::shared_ptr<Services::HTTPSProxySrvIf> objHTTPSProxy = std::make_shared<Services::HTTPSProxySrv>("Test", "Test");

	/*
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

	double shrIssued = shareIssuedVec.back();
	std::cout << "Shares Issued: " << shrIssued << '\n';



	// [ CASH FLOW STATEMENT ]
	std::vector<double> freeCashFlowVec;
	// Get Data
	objHTTPSProxy->_getFromCashFlowStatement("AAPL", freeCashFlowVec);


	// ---- Calculate linear function ----
	std::vector<double> x_year = {1, 2, 3, 4};	
	std::vector<double> y_freeCashFlowPerShareVec;

	for(auto s : freeCashFlowVec)
	{
		double fcf_per_share = s / shrIssued;
		// Calc FCF per share
		std::cout << "FCF per share Value: " << fcf_per_share << '\n';

		y_freeCashFlowPerShareVec.push_back(fcf_per_share);
	}


	// ---- Get Linear Regress Coeffs ----
	double a;
	double b;
	objHTTPSProxy->calcLinearRegressCoeffs(x_year, y_freeCashFlowPerShareVec, a, b);

	// Calc next years (i)
	// double rev = a + b * i;

	std::cout << " ---- DCF ---- " << '\n';
	
  	double previous_sum = 0.0;
  	double sum = 0.0;
  	
  	// Average (Const) FCF per share
  	// double FCFPS_Avrg_Const = 8.973;  //  In case of negative b

  	double interest_rate = 0.15;
  	double num = 1 + interest_rate;


  	for (int i = 1; i <= 100; ++i)
  	{
    	double FCFPS_Lin = a + b * i;
    	std::cout << "FCFPS Linear: " << FCFPS_Lin << '\n';

    	// CALCULATION
    	double member = FCFPS_Lin / pow(num, i);

    	previous_sum = sum;
    	sum = sum + member;

   		if ((sum - previous_sum) < 0.05)
    	{
      		std::cout << "INTRINSIC VALUE: " << sum << '\n';
      		// break;
    	}
    	std::cout << "Mem: " << member << " Sum: " << sum << " Diff: " << sum - previous_sum << '\n';
  	}
 */ 	
}


void Services::BrokerSrv::postInit()
{

}