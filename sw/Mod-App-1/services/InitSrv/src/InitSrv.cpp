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
	*/


	// ---- Calculate linear function ----
	std::vector<double> x_year = {1, 2, 3, 4};
	// NOTE: All numbers in thousands
	std::vector<double> y_FCF = {58896000, 73365000, 92953000, 111443000};  //  Ascending order

	// Also analyze buyback ratio - should be negative
	std::vector<double> sharesIssuedVec = {17772944, 16976763, 16426786, 15943425};  //  Ascending order


	// Negative test:
	std::vector<double> y_FCFNeg = {111443000, 92953000, 73365000, 58896000};  //  Ascending order
	double aNeg;
	double bNeg;
	objHTTPSProxy->calcLinearRegressCoeffs(x_year, y_FCFNeg, aNeg, bNeg);
	std::cout << ">>>> a: " << aNeg << '\n';
	std::cout << ">>>> b: " << bNeg << '\n';



	double sharesIssued = sharesIssuedVec.back();

	std::cout << "Shares issued: " << sharesIssued << '\n';


	// ---- Calculate FCF per Share ----
	std::vector<double> y_FCFPSVec;

	double fcf_ps_temp;

	for(double s : y_FCF)
	{
		fcf_ps_temp = s / sharesIssued;
		std::cout << "FCF per share: " << fcf_ps_temp << '\n';

		y_FCFPSVec.push_back(fcf_ps_temp);
	}



	// ---- Get Linear Regress Coeffs ----
	double a;
	double b;
	objHTTPSProxy->calcLinearRegressCoeffs(x_year, y_FCFPSVec, a, b);

/*
	std::cout << "Coeff: " << b << '\n';

	for(int i = 5; i <= 6; i++)
	{
		// Calc next two years
		double rev = a + b * i;
		std::cout << "Interpolated FCF: " << rev << '\n';
	}
*/

	std::cout << " -------- " << '\n';
	// Calculate DCF
	
  	double previous_sum = 0.0;
  	double sum = 0.0;
  	
  	// Average (Const) FCF per share
  	double FCFPS = 8.973;

  	double interest_rate = 0.13;
  	double num = 1 + interest_rate;


  	for (int i = 1; i <= 100; ++i)
  	{
    	double FCFPS_Lin = a + b * i;
    	std::cout << ">>>> FCFPS Linear: " << FCFPS_Lin << '\n';

    	// CALCULATION
    	double member = FCFPS_Lin / pow(num, i);

    	previous_sum = sum;
    	sum = sum + member;

   		if ((sum - previous_sum) < 0.05)
    	{
      		std::cout << "xxxx INTRINSIC VALUE: " << sum << '\n';
      		// break;
    	}
    	std::cout << "Mem: " << member << " Sum: " << sum << " Diff: " << sum - previous_sum << '\n';
    	// std::cout << "----" << '\n';
  	}
  	

}


void Services::InitSrv::postInit()
{

}