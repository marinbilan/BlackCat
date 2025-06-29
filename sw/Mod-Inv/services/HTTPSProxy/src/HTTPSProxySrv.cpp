#include <regex>
#include <fstream>
#include <memory>

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
}


Services::HTTPSProxySrv::~HTTPSProxySrv()
{
}


void Services::HTTPSProxySrv::preInit()
{
}


void Services::HTTPSProxySrv::postInit()
{
}



void Services::HTTPSProxySrv::_new_GetDataFromServer(Company& company)
{
	std::cout << "---------------- " << company.getCompanyTicker()  << " GETTING DATA FROM SERVER START ----------------" << '\n' << '\n';
	// FACTORY.getLog()->LOGFILE(LOG "[NEW TRACE] Get data from server (one monolith method)");

	std::string server("financialmodelingprep.com");
	// https://financialmodelingprep.com/api/v3/quote/AAPL/?apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2
	std::string path = "/api/v3/quote/" + company.getCompanyTicker() + "?apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string content {};
	Document document;

	std::cout << "[1.1] Get Summary data for " << company.getCompanyTicker()  << " from server START" << '\n';
	getDataFromServer(server, path, content);
	std::cout << "[1.2] Get Summary data for " << company.getCompanyTicker()  << " from server END" << '\n';

	{
		document.Parse(content.c_str());

		for (SizeType i = 0; i < document.Size(); i++) {
		 	const Value& obj = document[i];

		 	// TODO: Check is "name" string
			if(obj["price"].IsNumber() && 
			   obj["marketCap"].IsNumber() &&
			   obj["eps"].IsNumber() &&
			   obj["pe"].IsNumber() &&
			   obj["sharesOutstanding"].IsNumber()) {

			   	company.setSummary(obj["name"].GetString(), 
			   		obj["price"].GetDouble(),
			   		obj["marketCap"].GetInt64(),
			   		obj["eps"].GetDouble(),
			   		obj["pe"].GetDouble(),
			   		obj["sharesOutstanding"].GetInt64());
			}
		}
	}

	// FACTORY.getLog()->LOGFILE(LOG "[NEW TRACE] Parsing done");
	std::cout << "[1.3] Parsing Summary data " << company.getCompanyTicker()  << " DONE" << '\n' << '\n';


	// ---- INCOME STATEMENT ----

	// Annual

	// server = "financialmodelingprep.com";
	// https://financialmodelingprep.com/api/v3/income-statement/AAPL?period=quarter&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2
	path = "/api/v3/income-statement/" + company.getCompanyTicker() + "?period=annual&limit=7&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string contentISAnnual {};

	std::cout << "[2.1] Get Income Statement (Annual) data for " << company.getCompanyTicker()  << " from server START" << '\n';
	getDataFromServer(server, path, contentISAnnual);
	std::cout << "[2.2] Get Income Statement (Annual) data for " << company.getCompanyTicker()  << " from server END" << '\n';

	{
		document.Parse(contentISAnnual.c_str());

		for (SizeType i = 0; i < document.Size(); i++) 
		{
		 	const Value& obj = document[i];

		  	if(obj["calendarYear"].IsString() &&
		  	   obj["revenue"].IsNumber() &&   
		  	   obj["netIncomeRatio"].IsNumber() && 
		  	   obj["netIncome"].IsNumber())
		  	{
		  		Data revenue(obj["calendarYear"].GetString(), static_cast<double>(obj["revenue"].GetInt64()));
		  		Data netIncomeRatio(obj["calendarYear"].GetString(), static_cast<double>(obj["netIncomeRatio"].GetDouble()));
		  		Data netIncome(obj["calendarYear"].GetString(), static_cast<double>(obj["netIncome"].GetInt64()));

		  		company.setIncomeStatement(revenue, netIncomeRatio, netIncome);
		  		//
		  	} else {
		  		std::cout << "Error: not valid data type" << '\n';
		  	}
		}
	}

	std::cout << "[2.3] Parsing Income Statement (Annual) " << company.getCompanyTicker()  << " DONE" << '\n' << '\n';

	// Quartal

	// https://financialmodelingprep.com/api/v3/income-statement/AAPL?period=quarter&limit=7&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2
	// server = "financialmodelingprep.com";
	path = "/api/v3/income-statement/" + company.getCompanyTicker() + "?period=quarter&limit=13&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string contentISQuartal {};

	std::cout << "[3.1] Get Income Statement (Quartal) data for " << company.getCompanyTicker()  << " from server START" << '\n';
	getDataFromServer(server, path, contentISQuartal);
	std::cout << "[3.2] Get Income Statement (Quartal) data for " << company.getCompanyTicker()  << " from server END" << '\n';

	{
		document.Parse(contentISQuartal.c_str());

		for (SizeType i = 0; i < document.Size(); i++) 
		{
		 	const Value& obj = document[i];

		  	if(obj["calendarYear"].IsString() &&
		  	   obj["period"].IsString() &&
		  	   obj["revenue"].IsNumber() &&
		  	   obj["netIncomeRatio"].IsNumber() &&
		  	   obj["netIncome"].IsNumber())
		  	{
		  		// Period
		  		std::string periodQuartal(std::move(obj["calendarYear"].GetString()));
		  		std::string periodYear(std::move(obj["period"].GetString()));
		  		std::string period = periodQuartal + "_" + periodYear;

		  		Data revenueQuartal(period, static_cast<double>(obj["revenue"].GetInt64()));
		  		Data netIncomeRatioQuartal(period, static_cast<double>(obj["netIncomeRatio"].GetDouble()));
		  		Data netIncomeQuartal(period, static_cast<double>(obj["netIncome"].GetInt64()));

		  		company.setIncomeStatementQuartal(revenueQuartal, netIncomeRatioQuartal, netIncomeQuartal);

		  		//	
		  	} else {
		  		std::cout << "Error: not valid Income Statement data type" << '\n';
		  	}
		}
	}
	// FACTORY.getLog()->LOGFILE(LOG "[NEW TRACE] Get/Set Income Statement (Annual/Quartal) from server done");
	std::cout << "[3.3] Parsing Income Statement (Quartal) " << company.getCompanyTicker()  << " DONE" << '\n' << '\n';


	// ---- BALANCE SHEET ----

	// Annual

	// std::string server("financialmodelingprep.com");
	// financialmodelingprep.com/api/v3/balance-sheet-statement/AAPL/?period=annual&limit=7&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2
	path = "/api/v3/balance-sheet-statement/" + company.getCompanyTicker() + "?period=annual&limit=7&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string contentBSAnnual {};

	std::cout << "[4.1] Get Balance Sheet (Annual) data for " << company.getCompanyTicker()  << " from server START" << '\n';
	getDataFromServer(server, path, contentBSAnnual);
	std::cout << "[4.2] Get Balance Sheet (Annual) data for " << company.getCompanyTicker()  << " from server END" << '\n';

	{
		document.Parse(contentBSAnnual.c_str());

		for (SizeType i = 0; i < document.Size(); i++) 
		{
		 	const Value& obj = document[i];
		 	// TODO: totalAssets, totalLiabilities, retainedEarnings
		  	if(obj["calendarYear"].IsString() &&
		  	   obj["cashAndCashEquivalents"].IsNumber() &&
		  	   obj["totalStockholdersEquity"].IsNumber() &&
		  	   obj["totalDebt"].IsNumber())
		  	{
		  		// Period
		  		Data cashAndCashEquivalents(obj["calendarYear"].GetString(), static_cast<double>(obj["cashAndCashEquivalents"].GetInt64()));
		  		Data totalStockholdersEquity(obj["calendarYear"].GetString(), static_cast<double>(obj["totalStockholdersEquity"].GetInt64()));
		  		Data totalDebt(obj["calendarYear"].GetString(), static_cast<double>(obj["totalDebt"].GetInt64()));

		  		company.setBalanceSheet(cashAndCashEquivalents, totalStockholdersEquity, totalDebt);

		  		//	
		  	} else {
		  		std::cout << "Error: not valid Balance Sheet data type" << '\n';
		  	}
		}
	}

	std::cout << "[4.3] Parsing Balance Sheet (Annual) " << company.getCompanyTicker()  << " DONE" << '\n' << '\n';

	// Quartal

	// https://financialmodelingprep.com/api/v3/income-statement/AAPL?period=quarter&limit=7&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2
	path = "/api/v3/balance-sheet-statement/" + company.getCompanyTicker() + "?period=quarter&limit=13&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string contentBSQuartal {};

	std::cout << "[5.1] Get Balance Sheet (Quartal) data for " << company.getCompanyTicker()  << " from server START" << '\n';
	getDataFromServer(server, path, contentBSQuartal);
	std::cout << "[5.2] Get Balance Sheet (Quartal) data for " << company.getCompanyTicker()  << " from server END" << '\n';

	{
		document.Parse(contentBSQuartal.c_str());

		for (SizeType i = 0; i < document.Size(); i++) 
		{
		 	const Value& obj = document[i];

		  	if(obj["calendarYear"].IsString() &&
		  	   obj["period"].IsString() &&
		  	   obj["cashAndCashEquivalents"].IsNumber() &&
		  	   obj["totalStockholdersEquity"].IsNumber() &&
		  	   obj["totalDebt"].IsNumber())
		  	{
		  		// Period
		  		std::string periodQuartal(std::move(obj["calendarYear"].GetString()));
		  		std::string periodYear(std::move(obj["period"].GetString()));
		  		std::string period = periodQuartal + "_" + periodYear;

		  		Data cashAndCashEquivalentsQuartal(period, static_cast<double>(obj["cashAndCashEquivalents"].GetInt64()));
		  		Data totalStockholdersEquityQuartal(period, static_cast<double>(obj["totalStockholdersEquity"].GetInt64()));
		  		Data totalDebtQuartal(period, static_cast<double>(obj["totalDebt"].GetInt64()));

		  		company.setBalanceSheetQuartal(cashAndCashEquivalentsQuartal, totalStockholdersEquityQuartal, totalDebtQuartal);

		  		//	
		  	} else {
		  		std::cout << "Error: not valid Balance Sheet data type" << '\n';
		  	}
		}
	}
	// FACTORY.getLog()->LOGFILE(LOG "[NEW TRACE] Get/Set Balance Sheet (Annual/Quartal) from server done");
	std::cout << "[5.3] Parsing Balance Sheet (Quartal) " << company.getCompanyTicker()  << " DONE" << '\n' << '\n';


	// ---- CASH FLOW STATEMENT ----

	// Annual
	// std::string server("financialmodelingprep.com");
	// financialmodelingprep.com/api/v3/cash-flow-statement/AAPL?period=annual&limit=7&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2
	path = "/api/v3/cash-flow-statement/" + company.getCompanyTicker() + "?period=annual&limit=7&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string contentCFSAnnual {};

	std::cout << "[6.1] Get Cash Flow (Annual) data for " << company.getCompanyTicker()  << " from server START" << '\n';
	getDataFromServer(server, path, contentCFSAnnual);
	std::cout << "[6.2] Get Cash Flow (Annual) data for " << company.getCompanyTicker()  << " from server END" << '\n';

	{
		document.Parse(contentCFSAnnual.c_str());

		for (SizeType i = 0; i < document.Size(); i++) 
		{
		 	const Value& obj = document[i];
		  	if(obj["calendarYear"].IsString() &&
		  	   obj["freeCashFlow"].IsNumber())
		  	{
		  		// Period
		  		Data freeCashFlow(obj["calendarYear"].GetString(), static_cast<double>(obj["freeCashFlow"].GetInt64()));

		  		company.setCashFlowStatement(freeCashFlow);

		  		//	
		  	} else {
		  		std::cout << "Error: not valid Balance Sheet data type" << '\n';
		  	}
		}
	}

	std::cout << "[6.3] Parsing Cash Flow (Annual) " << company.getCompanyTicker()  << " DONE" << '\n' << '\n';

	// Quartal
	// financialmodelingprep.com/api/v3/cash-flow-statement/AAPL?period=quarter&limit=13&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2
	path = "/api/v3/cash-flow-statement/" + company.getCompanyTicker() + "?period=quarter&limit=13&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string contentCFSQuartal {};

	std::cout << "[7.1] Get Cash Flow (Quartal) data for " << company.getCompanyTicker()  << " from server START" << '\n';
	getDataFromServer(server, path, contentCFSQuartal);
	std::cout << "[7.2] Get Cash Flow (Quartal) data for " << company.getCompanyTicker()  << " from server START" << '\n';

	{
		document.Parse(contentCFSQuartal.c_str());

		for (SizeType i = 0; i < document.Size(); i++) 
		{
		 	const Value& obj = document[i];
		  	if(obj["calendarYear"].IsString() &&
		  	   obj["period"].IsString() &&
		  	   obj["freeCashFlow"].IsNumber() && obj["freeCashFlow"].IsInt64())
		  	{
		  		// Period
		  		std::string periodQuartal(std::move(obj["calendarYear"].GetString()));
		  		std::string periodYear(std::move(obj["period"].GetString()));
		  		std::string period = periodQuartal + "_" + periodYear;

		  		Data freeCashFlowQuartal(period, static_cast<double>(obj["freeCashFlow"].GetInt64()));

		  		company.setCashFlowStatementQuartal(freeCashFlowQuartal);
		  	} else {
		  		std::cout << "Error: not valid Cash Flow data type" << '\n';
		  	}
		}
	}
	// FACTORY.getLog()->LOGFILE(LOG "[NEW TRACE] Get/Set Cash Flow Statement (Annual/Quartal) from server done");
	std::cout << "[7.3] Parsing Cash Flow (Quartal) " << company.getCompanyTicker()  << " DONE" << '\n' << '\n';


	// ---- RATIOS ----
	// financialmodelingprep.com/api/v3/ratios/AAPL?period=annual&limit=5&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2
	path = "/api/v3/ratios/" + company.getCompanyTicker() + "?period=annual&limit=1&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string contentRatiosAnnual {};

	std::cout << "[8.1] Get Ratios data for " << company.getCompanyTicker()  << " from server START" << '\n';
	getDataFromServer(server, path, contentRatiosAnnual);
	std::cout << "[8.2] Get Ratios data for " << company.getCompanyTicker()  << " from server END" << '\n';

	{
		document.Parse(contentRatiosAnnual.c_str());

		for (SizeType i = 0; i < document.Size(); i++) 
		{
		 	const Value& obj = document[i];

		  	if(obj["currentRatio"].IsNumber() &&
		  	   obj["netProfitMargin"].IsNumber() &&
		  	   obj["returnOnEquity"].IsNumber() &&
		  	   obj["priceToBookRatio"].IsNumber() &&
		  	   obj["priceEarningsRatio"].IsNumber() && 
		  	   obj["priceFairValue"].IsNumber() && 
	  		   obj["dividendYield"].IsNumber()) {

		  	company.setRatios(obj["currentRatio"].GetDouble(), obj["netProfitMargin"].GetDouble(), obj["returnOnEquity"].GetDouble(),
		  		obj["priceToBookRatio"].GetDouble(), obj["priceEarningsRatio"].GetDouble(), obj["priceFairValue"].GetDouble(),
		  		obj["dividendYield"].GetDouble());

	  	} else {
	  		std::cout << "Error: not double" << '\n';
	  	}
		}
	}
	// FACTORY.getLog()->LOGFILE(LOG "[NEW TRACE] Get/Set Ratios from server done");
	std::cout << "[8.3] Parsing Balance Sheet (Annual) " << company.getCompanyTicker()  << " DONE" << '\n' << '\n';

	std::cout << "---------------- " << company.getCompanyTicker()  << " GETTING DATA FROM SERVER DONE ----------------" << '\n' << '\n';
}










// LEGACY


// CHECK TODO
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

		stock.getFullName() = obj["name"].GetString();

		if(obj["price"].IsNumber() && obj["pe"].IsNumber()) {
			stock.getStockPrice() = obj["price"].GetDouble();
			stock.getPERatio() = obj["pe"].GetDouble();
		}

	}

	// FACTORY.getLog()->LOGFILE(LOG "Company Name: " + stock.getFullName());
	// FACTORY.getLog()->LOGFILE(LOG "Stock Price: " + std::to_string(stock.getStockPrice()));
	// FACTORY.getLog()->LOGFILE(LOG "PE Ratio: " + std::to_string(stock.getPERatio()));

	return true;
}


// CHECK DONE
bool Services::HTTPSProxySrv::_getRatios(Stock& stock)
{
	std::string server("financialmodelingprep.com");
	std::string path = "/api/v3/ratios/" + stock.getName() + "?period=annual&limit=5&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

	std::string content {};

	getDataFromServer(server, path, content);

	// Reading Content
	Document document;
	document.Parse(content.c_str());

	std::vector<double> netProfitMarginVec;
	std::vector<double> returnOnEquityVec;
	std::vector<double> freeCashFlowPerShareVec;
	std::vector<double> priceToBookRatioVec;


	for (SizeType i = 0; i < document.Size(); i++) {
	 	const Value& obj = document[i];

		if (!obj.IsObject()) { 
			std::cout << "Array element at index " << i << " is not an object" << std::endl; 
			continue;
		}

		// Check if the object has the expected fields
		if (!obj.HasMember("netProfitMargin") || !obj.HasMember("returnOnEquity") 
			|| !obj.HasMember("freeCashFlowPerShare") || !obj.HasMember("priceToBookRatio")) {
			std::cout << "Object at index " << i << " is missing expected fields" << std::endl;
			continue;
	  	} 


	  	if(obj["netProfitMargin"].IsNumber() && obj["returnOnEquity"].IsNumber() && 
	  		obj["freeCashFlowPerShare"].IsNumber() && obj["priceToBookRatio"].IsNumber()) {

			netProfitMarginVec.push_back(obj["netProfitMargin"].GetDouble());
	  		returnOnEquityVec.push_back(obj["returnOnEquity"].GetDouble());
	  		freeCashFlowPerShareVec.push_back(obj["freeCashFlowPerShare"].GetDouble());
	  		priceToBookRatioVec.push_back(obj["priceToBookRatio"].GetDouble());

	  	} else {
	  		std::cout << "Error: not double" << '\n';
	  	}

	}

	// Set last 4 years avg
	stock.getNetProfitRatioAPI() = std::accumulate(netProfitMarginVec.begin(), netProfitMarginVec.end(), 0.0) / netProfitMarginVec.size();
	stock.getReturnOnEquityAPI() = std::accumulate(returnOnEquityVec.begin(), returnOnEquityVec.end(), 0.0) / returnOnEquityVec.size();
	stock.getFreeCashFlowPerShareAPI() = std::accumulate(freeCashFlowPerShareVec.begin(), freeCashFlowPerShareVec.end(), 0.0) / freeCashFlowPerShareVec.size();
	stock.getPriceToBookRatioAPI() = std::accumulate(priceToBookRatioVec.begin(), priceToBookRatioVec.end(), 0.0) / priceToBookRatioVec.size();

	/*
	FACTORY.getLog()->LOGFILE(LOG "netProfitMargin: " + std::to_string(stock.getNetProfitRatioAPI()));
	FACTORY.getLog()->LOGFILE(LOG "returnOnEquity: " + std::to_string(stock.getReturnOnEquityAPI()));
	FACTORY.getLog()->LOGFILE(LOG "freeCashFlowPerShare: " + std::to_string(stock.getFreeCashFlowPerShareAPI()));
	FACTORY.getLog()->LOGFILE(LOG "priceToBookRatio: " + std::to_string(stock.getPriceToBookRatioAPI()));
	*/

	return true;
}


// CHECK TODO
bool Services::HTTPSProxySrv::_getDCF(Stock& stock)
{

	std::string server("financialmodelingprep.com");
	std::string path = "/api/v3/discounted-cash-flow/" + stock.getName() + "?&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";


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
		if (!obj.HasMember("dcf")) {
			std::cout << "Object at index " << i << " is missing expected fields" << std::endl;
			continue;
	  	}

	  	stock.getDCFAPI() = obj["dcf"].GetDouble(); 

	}


	return true;
}


// CHECK DONE
bool Services::HTTPSProxySrv::_getFromIncomeStatement(Stock& stock)
{
	std::string server("financialmodelingprep.com");
	std::string path = "/api/v3/income-statement/" + stock.getName() + "?period=annual&limit=7&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

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


	  	if(obj["revenue"].IsInt64() && obj["grossProfit"].IsInt64() && 
	  		obj["netIncome"].IsInt64() && obj["weightedAverageShsOut"].IsInt64()) {

			stock.setIncomeStatementParams(static_cast<double>(obj["revenue"].GetInt64()),
			static_cast<double>(obj["grossProfit"].GetInt64()),
			static_cast<double>(obj["netIncome"].GetInt64()),
			static_cast<double>(obj["weightedAverageShsOut"].GetInt64()));  		
	  	} else {
	  		std::cout << "Error: not int 64" << '\n';
	  	}

	}
	
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

	vecToString(vecTrace, stock.getShareIssuedVec());
	FACTORY.getLog()->LOGFILE(LOG "Shares issued " + stock.getName() + ": " + vecTrace);
	vecTrace.clear();
	*/

	return true;
}


// NOTHING
bool Services::HTTPSProxySrv::_getRevenueAndEPSPrediction(const std::string& stockTicker,
		std::vector<double>& revenuePredictionVec, 
		std::vector<double>& epsPredictionVec)
{


	return true;
}


// CHECK TODO
bool Services::HTTPSProxySrv::_getFromBalanceSheet(Stock& stock)
{
	std::string server("financialmodelingprep.com");
	// financialmodelingprep.com/api/v3/balance-sheet-statement/AAPL/?period=annual&limit=7&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2
	std::string path = "/api/v3/balance-sheet-statement/" + stock.getName() + "?period=annual&limit=7&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

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


		stock.setBalanceSheetParams(static_cast<double>(obj["totalStockholdersEquity"].GetInt64()),
			static_cast<double>(obj["totalDebt"].GetInt64()));
	}


	// Trace
	/*
	std::string vecTrace{};

	vecToString(vecTrace, stock.getBookValueVec());
	FACTORY.getLog()->LOGFILE(LOG "Book Value for " + stock.getName() + ": " + vecTrace);
	vecTrace.clear();

	vecToString(vecTrace, stock.getTotalDebtVec());
	FACTORY.getLog()->LOGFILE(LOG "Total Debt for " + stock.getName() + ": " + vecTrace);
	vecTrace.clear();
	*/

	return true;
}


bool Services::HTTPSProxySrv::_getFromCashFlowStatement(Stock& stock)
{

	std::string server("financialmodelingprep.com");
	std::string path = "/api/v3/cash-flow-statement/" + stock.getName() + "?period=annual&limit=7&apikey=uPMbx8GNAsEUl3youNkelyZIwSUfdbT2";

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

		if(obj["freeCashFlow"].IsInt64()) {
			stock.getFreeCashFlowVec().push_back(static_cast<double>(obj["freeCashFlow"].GetInt64()));		 		
	  	} else {
	  		std::cout << "Error: not int 64" << '\n';
	  	}

	}


	// Trace
	/*
	std::string fcfVecTrace{};
	
	vecToString(fcfVecTrace, stock.getFreeCashFlowVec());
	FACTORY.getLog()->LOGFILE(LOG "FCF for " + stock.getName() + ": " + fcfVecTrace);
	*/

	return true;
}


// NOTHING
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