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



bool Services::HTTPSProxySrv::_getFromSummary(Stock& stock)
{

	std::string server("financialmodelingprep.com");
	std::string path = "/api/v3/quote/" + stock.getName() + "?apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string content {};

	getDataFromServer(server, path, content);

	// Reading Content
	Document document;
	document.Parse(content.c_str());


	for (SizeType i = 0; i < document.Size(); i++) {
	 	const Value& obj = document[i];

		if (!obj.IsObject()) { 
			std::cout << "Array element at index " << i << " is not an object" << std::endl; 
			continue;
		}

		// Check if the object has the expected fields
		if (!obj.HasMember("name") || !obj.HasMember("price") || !obj.HasMember("pe") || !obj.HasMember("sharesOutstanding")) {
			std::cout << "Object at index " << i << " is missing expected fields" << std::endl;
			continue;
	  	} 

		// const Value& sharesOutstanding = obj["sharesOutstanding"];

		stock.getFullName() = obj["name"].GetString(); 
		stock.getStockPrice() = obj["price"].GetDouble();
		stock.getPERatio() = obj["pe"].GetDouble();
	}

	return true;
}



bool Services::HTTPSProxySrv::_getFromIncomeStatement(Stock& stock)
{

	std::string server("financialmodelingprep.com");
	std::string path = "/api/v3/income-statement/" + stock.getName() + "?period=annual&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string content {};

	getDataFromServer(server, path, content);

	// Reading Content
	Document document;
	document.Parse(content.c_str());


	// Iterate through the array
	for (SizeType i = 0; i < document.Size(); i++) {
	 	const Value& obj = document[i];

		// Check if the element is an object
		if (!obj.IsObject()) {
			std::cout << "Array element at index " << i << " is not an object" << std::endl;
			continue;
		}

		// Check if the object has the expected fields
		if (!obj.HasMember("revenue") || !obj.HasMember("grossProfit") || !obj.HasMember("netIncome") || !obj.HasMember("weightedAverageShsOut")) {
			std::cout << "Object at index " << i << " is missing expected fields" << std::endl;
			continue;
	  	} 


		stock.getRevenueVec().push_back(static_cast<double>(obj["revenue"].GetInt64()));
		stock.getGrossProfitVec().push_back(static_cast<double>(obj["grossProfit"].GetInt64()));
		stock.getIncomeVec().push_back(static_cast<double>(obj["netIncome"].GetInt64()));
		stock.getShareIssuedVec().push_back(static_cast<double>(obj["weightedAverageShsOut"].GetInt64()));
	}

	
	std::reverse(stock.getRevenueVec().begin(), stock.getRevenueVec().end());
	std::reverse(stock.getGrossProfitVec().begin(), stock.getGrossProfitVec().end());
	std::reverse(stock.getIncomeVec().begin(), stock.getIncomeVec().end());
	std::reverse(stock.getShareIssuedVec().begin(), stock.getShareIssuedVec().end());
	

	// Trace
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

	vecToString(vecTrace, stock.getShareIssuedVec());
	FACTORY.getLog()->LOGFILE(LOG "Shares issued " + stock.getName() + ": " + vecTrace);
	vecTrace.clear();


	return true;
}



bool Services::HTTPSProxySrv::_getRevenueAndEPSPrediction(const std::string& stockTicker,
		std::vector<double>& revenuePredictionVec, 
		std::vector<double>& epsPredictionVec)
{


	return true;
}



bool Services::HTTPSProxySrv::_getFromBalanceSheet(Stock& stock)
{

	std::string server("financialmodelingprep.com");
	std::string path = "/api/v3/balance-sheet-statement/" + stock.getName() + "?period=annual&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string content {};

	getDataFromServer(server, path, content);

	// Reading Content
	Document document;
	document.Parse(content.c_str());


	// Iterate through the array
	for (SizeType i = 0; i < document.Size(); i++) {
	 	const Value& obj = document[i];

		// Check if the element is an object
		if (!obj.IsObject()) {
			std::cout << "Array element at index " << i << " is not an object" << std::endl;
			continue;
		}

		// Check if the object has the expected fields
		if (!obj.HasMember("totalStockholdersEquity") || !obj.HasMember("totalDebt") /*|| !obj.HasMember("netIncome")*/) {
			std::cout << "Object at index " << i << " is missing expected fields" << std::endl;
			continue;
	  	} 


		stock.getBookValueVec().push_back(static_cast<double>(obj["totalStockholdersEquity"].GetInt64()));
		stock.getTotalDebtVec().push_back(static_cast<double>(obj["totalDebt"].GetInt64()));

	}

	
	std::reverse(stock.getBookValueVec().begin(), stock.getBookValueVec().end());
	std::reverse(stock.getTotalDebtVec().begin(), stock.getTotalDebtVec().end());
	

	// Trace
	std::string vecTrace{};

	vecToString(vecTrace, stock.getBookValueVec());
	FACTORY.getLog()->LOGFILE(LOG "Book Value for " + stock.getName() + ": " + vecTrace);
	vecTrace.clear();

	vecToString(vecTrace, stock.getTotalDebtVec());
	FACTORY.getLog()->LOGFILE(LOG "Total Debt for " + stock.getName() + ": " + vecTrace);
	vecTrace.clear();


	return true;
}



bool Services::HTTPSProxySrv::_getFromCashFlowStatement(Stock& stock)
{

	std::string server("financialmodelingprep.com");
	std::string path = "/api/v3/cash-flow-statement/" + stock.getName() + "?period=annual&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string content {};

	getDataFromServer(server, path, content);

	// Reading Content
	Document document;
	document.Parse(content.c_str());


	// Iterate through the array
	for (SizeType i = 0; i < document.Size(); i++) {
	 	const Value& obj = document[i];

		// Check if the element is an object
		if (!obj.IsObject()) {
			std::cout << "Array element at index " << i << " is not an object" << std::endl;
			continue;
		}

		if (!obj.HasMember("freeCashFlow")) {
			std::cout << "Object at index " << i << " is missing expected fields" << std::endl;
			continue;
	  	}

		stock.getFreeCashFlowVec().push_back(static_cast<double>(obj["freeCashFlow"].GetInt64()));

	}

	std::reverse(stock.getFreeCashFlowVec().begin(), stock.getFreeCashFlowVec().end());


	// Trace
	std::string fcfVecTrace{};
	
	vecToString(fcfVecTrace, stock.getFreeCashFlowVec());
	FACTORY.getLog()->LOGFILE(LOG "FCF for " + stock.getName() + ": " + fcfVecTrace);

	return true;
}



bool Services::HTTPSProxySrv::_getFromAnalysisStatement(const std::string& stockTicker,
		std::vector<double>& EPSEstimates,
		std::vector<double>& GrowthEstimates)
{


	return true;
}



void Services::HTTPSProxySrv::getDataFromServer(const std::string& server, const std::string& path, std::string& data) 
{
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

	Networking::HTTPSClient c(io_service, ctx, server, path, data);

	io_service.run();
}