#include <regex>
#include <fstream>

#include "Factory.h"

#include "HTTPSProxySrv.h"
#include "HTTPSClient.h"
#include "CommonTypes.h"

#include "/home/marin/dev/BlackCat/libs/rapidjson/document.h"
#include "/home/marin/dev/BlackCat/libs/rapidjson/writer.h"
#include "/home/marin/dev/BlackCat/libs/rapidjson/stringbuffer.h"

using namespace rapidjson;


Services::HTTPSProxySrv::HTTPSProxySrv(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::cout << "HTTPSProxySrv constructor called" << '\n';
}


Services::HTTPSProxySrv::~HTTPSProxySrv()
{
}


void Services::HTTPSProxySrv::preInit()
{
	// Get params from DB for this instance
	std::cout << "HTTPSProxySrv preInit() called!" << '\n';
}


void Services::HTTPSProxySrv::postInit()
{

}


// Summary
bool Services::HTTPSProxySrv::_getFromSummary(Stock& stock)
// bool Services::HTTPSProxySrv::_getFromSummary(const std::string& stockTicker, std::string& stockName, double& stockPrice, double& PE_Ratio)
{
	// Stock Full Name
	// Stock Price
	// PE Ratio
	// sharesOutstanding

	// https://financialmodelingprep.com/api/v3/quote/AAPL?apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2	
	// https://financialmodelingprep.com/api/v3/quote/AAPL?apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2
	// Backup
	// https://site.financialmodelingprep.com/developer/docs#company-profile-company-information

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

	std::string server("financialmodelingprep.com");
	std::string path = "/api/v3/quote/" + stock.getName() + "?apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string content {};
	Networking::HTTPSClient c(io_service, ctx, server, path, content);

	io_service.run();

	// Reading content
	
	Document document;
	document.Parse(content.c_str());


	// Iterate through the array
	for (SizeType i = 0; i < document.Size(); i++) {
	 	const Value& obj = document[i];

		// Check if the element is an object
		/*
		if (!obj.IsObject()) {
			std::cerr << "Array element at index " << i << " is not an object" << std::endl;
			continue;
		}
		*/

		// Check if the object has the expected fields
		if (!obj.HasMember("name") || !obj.HasMember("price") || !obj.HasMember("pe") || !obj.HasMember("sharesOutstanding")) {
			std::cout << "Object at index " << i << " is missing expected fields" << std::endl;
			continue;
	  	} 

		// Access the fields
		const Value& name = obj["name"];
		const Value& price = obj["price"];
		const Value& pe = obj["pe"];
		const Value& sharesOutstanding = obj["sharesOutstanding"];
		
  		// Print out the fields
		//std::cout << "name: " << name.GetString() << '\n';
		//std::cout << "price: " << price.GetDouble() << '\n';
		//std::cout << "pe: " << pe.GetDouble() << '\n';
		//std::cout << "sharesOutstanding: " << sharesOutstanding.GetUint64() << '\n';


		stock.getFullName() = name.GetString(); 
		stock.getStockPrice() = price.GetDouble();
		stock.getPERatio() = pe.GetDouble();


		// Push_back in vector
	}

	// Float shares (Number of shares)
	// https://financialmodelingprep.com/api/v4/historical/shares_float?symbol=AAPL&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2
	



	return true;
}


// [INCOME STATEMENT]

bool Services::HTTPSProxySrv::_getFromIncomeStatement(Stock& stock, bool standard)
{
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

	std::string server("financialmodelingprep.com");
	// std::string path("/api/v3/income-statement/AAPL?period=annual&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2");
	std::string path = "/api/v3/income-statement/" + stock.getName() + "?period=annual&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string content {};
	Networking::HTTPSClient c(io_service, ctx, server, path, content);

	io_service.run();

	// Reading content
	
	Document document;
	document.Parse(content.c_str());


	// Iterate through the array
	for (SizeType i = 0; i < document.Size(); i++) {
	 	const Value& obj = document[i];

		// Check if the element is an object
		/*
		if (!obj.IsObject()) {
			std::cerr << "Array element at index " << i << " is not an object" << std::endl;
			continue;
		}
		*/

		// Check if the object has the expected fields
		if (!obj.HasMember("revenue") || !obj.HasMember("grossProfit") || !obj.HasMember("netIncome") || !obj.HasMember("weightedAverageShsOut")) {
			std::cout << "Object at index " << i << " is missing expected fields" << std::endl;
			continue;
	  	} 

		// Access the fields
		const Value& revenue = obj["revenue"];
		const Value& grossProfit = obj["grossProfit"];
		const Value& netIncome = obj["netIncome"];
		const Value& weightedAverageShsOut = obj["weightedAverageShsOut"];
		
  		// Print out the fields
		//std::cout << "revenue: " << revenue.GetUint64() << '\n';
		//std::cout << "grossProfit: " << grossProfit.GetUint64() << '\n';
		//std::cout << "netIncome: " << netIncome.GetUint64() << '\n';
		//std::cout << "weightedAverageShsOut: " << weightedAverageShsOut.GetUint64() << '\n';

		stock.getRevenueVec().push_back(static_cast<double>(revenue.GetInt64()));
		stock.getGrossProfitVec().push_back(static_cast<double>(grossProfit.GetInt64()));
		stock.getIncomeVec().push_back(static_cast<double>(netIncome.GetInt64()));
		stock.getShareIssuedVec().push_back(static_cast<double>(weightedAverageShsOut.GetInt64()));
		// Push_back in vector
	}


	// CHECK if this is needed: Reverse elems in vec
	
	std::reverse(stock.getRevenueVec().begin(), stock.getRevenueVec().end());
	std::reverse(stock.getGrossProfitVec().begin(), stock.getGrossProfitVec().end());
	std::reverse(stock.getIncomeVec().begin(), stock.getIncomeVec().end());
	std::reverse(stock.getShareIssuedVec().begin(), stock.getShareIssuedVec().end());
	


	
	// Trace

	/*
	std::string vecTrace{};

	vecToString(vecTrace, stock.getRevenueVec());
	FACTORY.getLog()->LOGFILE(LOG "Revenue for " + stock.getName() + ": " + vecTrace);
	vecTrace.clear();

	vecToString(vecTrace, stock.getGrossProfitVec());
	FACTORY.getLog()->LOGFILE(LOG "Gross Profit for " + stock.getName() + ": " + vecTrace);
	vecTrace.clear();

	vecToString(vecTrace, stock.getIncomeVec());
	FACTORY.getLog()->LOGFILE(LOG "Net Income for " + stock.getName() + ": " + vecTrace);
	vecTrace.clear();
	*/


	return true;
}


// FUTURE REVENUE AND EPS STATEMENT
bool Services::HTTPSProxySrv::_getRevenueAndEPSPrediction(const std::string& stockTicker,
		std::vector<double>& revenuePredictionVec, 
		std::vector<double>& epsPredictionVec)
{


	return true;
}


/*
[ BALANCE SHEET ]

*/
bool Services::HTTPSProxySrv::_getFromBalanceSheet(Stock& stock, bool standard)
{

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

	std::string server("financialmodelingprep.com");
	// std::string path("/api/v3/balance-sheet-statement/AAPL?period=annual&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2");
	std::string path = "/api/v3/balance-sheet-statement/" + stock.getName() + "?period=annual&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string content {};
	Networking::HTTPSClient c(io_service, ctx, server, path, content);

	io_service.run();

	// Reading content
	
	Document document;
	document.Parse(content.c_str());


	// Iterate through the array
	for (SizeType i = 0; i < document.Size(); i++) {
	 	const Value& obj = document[i];

		// Check if the element is an object
		/*
		if (!obj.IsObject()) {
			std::cerr << "Array element at index " << i << " is not an object" << std::endl;
			continue;
		}
		*/

		// Check if the object has the expected fields
		if (!obj.HasMember("totalStockholdersEquity") || !obj.HasMember("totalDebt") /*|| !obj.HasMember("netIncome")*/) {
			std::cout << "Object at index " << i << " is missing expected fields" << std::endl;
			continue;
	  	} 

		// Access the fields
		const Value& totalStockholdersEquity = obj["totalStockholdersEquity"];
		const Value& totalDebt = obj["totalDebt"];
		// const Value& netIncome = obj["netIncome"];
		
  		// Print out the fields
		//std::cout << "totalStockholdersEquity: " << totalStockholdersEquity.GetUint64() << '\n';
		//std::cout << "totalDebt: " << totalDebt.GetUint64() << '\n';
		// std::cout << "netIncome: " << netIncome.GetUint64() << '\n';

		stock.getBookValueVec().push_back(static_cast<double>(totalStockholdersEquity.GetInt64()));
		stock.getTotalDebtVec().push_back(static_cast<double>(totalDebt.GetInt64()));

		// Push_back in vector
	}


	// CHECK if this is needed: Reverse elems in vec

	
	std::reverse(stock.getBookValueVec().begin(), stock.getBookValueVec().end());
	std::reverse(stock.getTotalDebtVec().begin(), stock.getTotalDebtVec().end());
	



	// Trace

	/*
	std::string vecTrace{};

	vecToString(vecTrace, stock.getBookValueVec());
	FACTORY.getLog()->LOGFILE(LOG "Book Value for " + stock.getName() + ": " + vecTrace);
	vecTrace.clear();

	vecToString(vecTrace, stock.getTotalDebtVec());
	FACTORY.getLog()->LOGFILE(LOG "Total Debt for " + stock.getName() + ": " + vecTrace);
	vecTrace.clear();

	vecToString(vecTrace, stock.getShareIssuedVec());
	FACTORY.getLog()->LOGFILE(LOG "Shares Issued for " + stock.getName() + ": " + vecTrace);
	vecTrace.clear();	
	*/

	return true;
}


// CASH FLOW STATEMENT
bool Services::HTTPSProxySrv::_getFromCashFlowStatement(Stock& stock, bool standard)
{

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

	std::string server("financialmodelingprep.com");
	//std::string path("/api/v3/cash-flow-statement/AAPL?period=annual&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2");
	std::string path = "/api/v3/cash-flow-statement/" + stock.getName() + "?period=annual&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";
	

	std::string content {};
	Networking::HTTPSClient c(io_service, ctx, server, path, content);

	io_service.run();

	// Reading content
	
	Document document;
	document.Parse(content.c_str());


	// Iterate through the array
	for (SizeType i = 0; i < document.Size(); i++) {
	 	const Value& obj = document[i];

		// Check if the element is an object
		/*
		if (!obj.IsObject()) {
			std::cerr << "Array element at index " << i << " is not an object" << std::endl;
			continue;
		}
		*/

		// Check if the object has the expected fields
		if (!obj.HasMember("freeCashFlow") /* || !obj.HasMember("totalDebt") || !obj.HasMember("netIncome")*/) {
			std::cout << "Object at index " << i << " is missing expected fields" << std::endl;
			continue;
	  	} 

		// Access the fields
		const Value& freeCashFlow = obj["freeCashFlow"];
		// const Value& totalDebt = obj["totalDebt"];
		// const Value& netIncome = obj["netIncome"];
		
  		// Print out the fields
		// std::cout << "freeCashFlow: " << freeCashFlow.GetInt64() << '\n';
		// std::cout << "totalDebt: " << totalDebt.GetUint64() << '\n';
		// std::cout << "netIncome: " << netIncome.GetUint64() << '\n';

		stock.getFreeCashFlowVec().push_back(static_cast<double>(freeCashFlow.GetInt64()));


		// Push_back in vector
	}


	// CHECK if this is needed: Reverse elems in vec

	std::reverse(stock.getFreeCashFlowVec().begin(), stock.getFreeCashFlowVec().end());


	// Check if we get on standard way
	// if(!stock.getFreeCashFlowVec().size()) return false;


	// Remove first element TTM
	/*
	if(standard) {
		stock.getFreeCashFlowVec().erase(stock.getFreeCashFlowVec().begin());
	} else {
		// Do nothing
	}*/
	

	/*
	std::string fcfVecTrace{};
	
	vecToString(fcfVecTrace, stock.getFreeCashFlowVec());
	FACTORY.getLog()->LOGFILE(LOG "FCF for " + stock.getName() + ": " + fcfVecTrace);
	*/

	return true;
}


// ANALYSIS
bool Services::HTTPSProxySrv::_getFromAnalysisStatement(const std::string& stockTicker,
		std::vector<double>& EPSEstimates,
		std::vector<double>& GrowthEstimates)
{


	return true;
}

