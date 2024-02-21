#include <regex>
#include <fstream>

#include "Factory.h"

#include "HTTPSProxySrv.h"
#include "HTTPSClient.h"
#include "CommonTypes.h"



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
bool Services::HTTPSProxySrv::_getFromSummary(const std::string& stockTicker, std::string& stockName, double& stockPrice, double& PE_Ratio)
{
	// [1] Preparation
	// ----
	// Create client and get Income Statement (HTML) in _HTTPSContent.txt file
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Income Statement
    std::string server("finance.yahoo.com");
    // finance.yahoo.com/quote/AAPL?p=AAPL
    std::string path("/quote/" + stockTicker + "?p=" + stockTicker);

	Networking::HTTPSClient c(io_service, ctx, server, path);
    io_service.run();
    // ----

    // [2] Get: Revenue, Gross Profit Net Income (Common Stackhold)
    // ----
	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;

	std::smatch match;
	std::regex stockPricePattern("regularMarketPrice\" data-trend=\"none\" data-pricehint=\"[0-9]+\" value=\"([0-9]+\\.[0-9]+)");
	std::regex peRatioPattern("data-test=\"PE_RATIO-value\">([0-9]+\\.[0-9]+)");
	std::regex stockNamePattern("<meta charSet=\"utf-8\"\\/><title>(.*)Stock Price");

	while (std::getline(file, lineFromFile)) 
	{
		// Get Stock Name
		if (std::regex_search(lineFromFile, match, stockNamePattern))
		{
			stockName = match[1].str();
		}
		// Get Stock Price
		if (std::regex_search(lineFromFile, match, stockPricePattern))
		{
			// std::string peRatio = match[1].str();
			stockPrice = stod(match[1].str());
			std::cout << "Regular Market Price: " << stockPrice << std::endl;

			// std::cout << "PE Ratio: " << peRatio << std::endl;
		}

		// Get PE Ratio
		if (std::regex_search(lineFromFile, match, peRatioPattern))
		{
			// std::string peRatio = match[1].str();
			PE_Ratio = stod(match[1].str());
			// std::cout << "PE Ratio: " << peRatio << std::endl;
		}
	}



	return true;
}


/*
[INCOME STATEMENT]

*/
bool Services::HTTPSProxySrv::_getFromIncomeStatement(Stock& stock, bool standard)
{
	// [1] Preparation
	// ----
	// Create client and get Income Statement (HTML) in _HTTPSContent.txt file
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Income Statement
    std::string server("finance.yahoo.com");
    std::string path("/quote/" + stock.getName() + "/financials?p=" + stock.getName());

	Networking::HTTPSClient c(io_service, ctx, server, path);
    io_service.run();
    // ----

    // [2] Get: Revenue, Gross Profit Net Income (Common Stackhold)
    // ----
	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;
	std::regex regexRevenueLine;
	std::regex regexGrossProfitLine;
	std::regex regexNetIncomeLine;


	if(standard) {
		regexRevenueLine = "Total Revenue.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
		regexGrossProfitLine = "Gross Profit.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
		regexNetIncomeLine = "Net Income Common Stockholders.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
	} else {
		regexRevenueLine = "Total Revenue.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
		regexGrossProfitLine = "Gross Profit.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
		regexNetIncomeLine = "Net Income Common Stockholders.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
	}
	

	while (std::getline(file, lineFromFile)) 
	{
		// Get Total Revenue
		if (std::regex_search(lineFromFile, match, regexRevenueLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
				if(i % 2 != 0) {  // Only for odd indexes
					std::string totalRevenueStr(match[i].str());
					// Remove "," from data
					totalRevenueStr.erase(remove(totalRevenueStr.begin(), totalRevenueStr.end(), ','), totalRevenueStr.end());

					double num = std::stod(totalRevenueStr);
					// Push back in vector (important - in thousands)
					stock.getRevenueVec().push_back(num);
				}
		    }
		}

		// Get Gross Profit
		if (std::regex_search(lineFromFile, match, regexGrossProfitLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
				if(i % 2 != 0) {  // Only for odd indexes
					std::string grossProfitStr(match[i].str());
					// Remove "," from data
					grossProfitStr.erase(remove(grossProfitStr.begin(), grossProfitStr.end(), ','), grossProfitStr.end());

					double num = stod(grossProfitStr);
					// Push back in vector (important - in thousands)
					stock.getGrossProfitVec().push_back(num);
				}
		    }
		}

		// Get Net Income
		if (std::regex_search(lineFromFile, match, regexNetIncomeLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
				if(i % 2 != 0) {  // Only for odd indexes
					std::string netIncomeStr(match[i].str());
					// Remove "," from data
					netIncomeStr.erase(remove(netIncomeStr.begin(), netIncomeStr.end(), ','), netIncomeStr.end());

					double num = stod(netIncomeStr);
					// Push back in vector (important - in thousands)
					stock.getIncomeVec().push_back(num);
				}
		    }
		}
	}

	// Close file
	file.close();


	// Remove first (TTM) element
	if(standard) {
		stock.getRevenueVec().erase(stock.getRevenueVec().begin());
		stock.getGrossProfitVec().erase(stock.getGrossProfitVec().begin());
		stock.getIncomeVec().erase(stock.getIncomeVec().begin());
	} else {
		// Do nothing
	}	

	// Reverse elems in vec
	std::reverse(stock.getRevenueVec().begin(), stock.getRevenueVec().end());
	std::reverse(stock.getGrossProfitVec().begin(), stock.getGrossProfitVec().end());
	std::reverse(stock.getIncomeVec().begin(), stock.getIncomeVec().end());

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
	// [1] Preparation
	// ----
	// Create client and get Income Statement (HTML) in _HTTPSContent.txt file
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Income Statement- APPLE
    std::string server("finance.yahoo.com");
    // std::string path("/quote/AAPL/balance-sheet?p=AAPL");
    std::string path("/quote/" + stock.getName() + "/balance-sheet?p=" + stock.getName());

	Networking::HTTPSClient c(io_service, ctx, server, path);
    io_service.run();
    // ----

    // [2] Get:
    // ----
	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;
	std::regex regexTotalEquityLine;
	std::regex regexTotalDebtLine;
	std::regex regexSharesNumberLine;


	if(standard) {
		regexTotalEquityLine = "Total Equity Gross Minority Interest.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";	
		regexTotalDebtLine = "Total Debt.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
		regexSharesNumberLine = "Share Issued.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
	} else {
		regexTotalEquityLine = "Total Equity Gross Minority Interest.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";	
		regexTotalDebtLine = "Total Debt.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
		regexSharesNumberLine = "Share Issued.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";

	}


	while (std::getline(file, lineFromFile)) 
	{
		// Get Total Equity
		if (std::regex_search(lineFromFile, match, regexTotalEquityLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
				if(i % 2 != 0) {  // Only for odd indexes
					std::string totalEquityStr(match[i].str());
					// Remove "," from data
					totalEquityStr.erase(remove(totalEquityStr.begin(), totalEquityStr.end(), ','), totalEquityStr.end());

					double num = stod(totalEquityStr);
					// Push back in vector (important - in thousands)
					stock.getBookValueVec().push_back(num);
				}
		    }
		}
		// Get Total Debt
		if (std::regex_search(lineFromFile, match, regexTotalDebtLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
				if(i % 2 != 0) {  // Only for odd indexes
					std::string totalDebtStr(match[i].str());
					// Remove "," from data
					totalDebtStr.erase(remove(totalDebtStr.begin(), totalDebtStr.end(), ','), totalDebtStr.end());

					double num = stod(totalDebtStr);
					// Push back in vector (important - in thousands)
					stock.getTotalDebtVec().push_back(num);
				}
		    }
		}
		// Get Number of Shares
		if (std::regex_search(lineFromFile, match, regexSharesNumberLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
				if(i % 2 != 0) {  // Only for odd indexes
					std::string totalNumberOfSharesStr(match[i].str());
					// Remove "," from data
					totalNumberOfSharesStr.erase(remove(totalNumberOfSharesStr.begin(), totalNumberOfSharesStr.end(), ','), totalNumberOfSharesStr.end());

					double num = stod(totalNumberOfSharesStr);
					// Push back in vector (important - in thousands)
					stock.getShareIssuedVec().push_back(num);
				}
		    }
		}
	}

	// Close file
	file.close();


	// DO NOT NEED TO Remove first element TTM

	// Reverse elems in vec
	std::reverse(stock.getBookValueVec().begin(), stock.getBookValueVec().end());
	std::reverse(stock.getTotalDebtVec().begin(), stock.getTotalDebtVec().end());
	std::reverse(stock.getShareIssuedVec().begin(), stock.getShareIssuedVec().end());
	
	// Trace

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

	return true;
}


// CASH FLOW STATEMENT
bool Services::HTTPSProxySrv::_getFromCashFlowStatement(Stock& stock, bool standard)
{
	// [1] Preparation
	// ----
	// Create client and get Income Statement (HTML) in _HTTPSContent.txt file
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Income Statement
    std::string server("finance.yahoo.com");
    std::string path("/quote/" + stock.getName() + "/cash-flow?p=" + stock.getName());

	FACTORY.getLog()->LOGFILE(LOG "Creating Cash Flow HTTPS client for " + stock.getName());
	

	Networking::HTTPSClient c(io_service, ctx, server, path);
    io_service.run();
    // ----

    // [2] Get Data:
    // ----
	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;
	std::regex regexFreeCashFlowLine;


	if(standard) {
		regexFreeCashFlowLine = "Free Cash Flow.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
	} else {
		regexFreeCashFlowLine = "Free Cash Flow.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
	}

	
	while (std::getline(file, lineFromFile)) 
	{
		// Get Free Cash Flow
		if (std::regex_search(lineFromFile, match, regexFreeCashFlowLine))
		{	
			for (size_t i = 1; i < match.size(); ++i)
		    {
				if(i % 2 != 0) {  // Only for odd indexes
					std::string freeCashFlowStr(match[i].str());

					// Remove "," from data
					freeCashFlowStr.erase(remove(freeCashFlowStr.begin(), freeCashFlowStr.end(), ','), freeCashFlowStr.end());
					double num = stod(freeCashFlowStr);
					// Push back in vector (important - in thousands)
					stock.getFreeCashFlowVec().push_back(num);
				}
				
		    }
		}
	}

	// Close file
	file.close();

	// Check if we get on standard way
	if(!stock.getFreeCashFlowVec().size()) return false;


	// Remove first element TTM
	if(standard) {
		stock.getFreeCashFlowVec().erase(stock.getFreeCashFlowVec().begin());
	} else {
		// Do nothing
	}
	
	// Reverse elems in vec
	std::reverse(stock.getFreeCashFlowVec().begin(), stock.getFreeCashFlowVec().end());
	
	std::string fcfVecTrace{};
	
	vecToString(fcfVecTrace, stock.getFreeCashFlowVec());
	FACTORY.getLog()->LOGFILE(LOG "FCF for " + stock.getName() + ": " + fcfVecTrace);
	

	return true;
}


// ANALYSIS
bool Services::HTTPSProxySrv::_getFromAnalysisStatement(const std::string& stockTicker,
		std::vector<double>& EPSEstimates,
		std::vector<double>& GrowthEstimates)
{
	// [1] Preparation
	// ----
	// Create client and get Income Statement (HTML) in _HTTPSContent.txt file
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Income Statement- APPLE
    std::string server("finance.yahoo.com");
    std::string path("/quote/" + stockTicker + "/cash-flow?p=" + stockTicker);

	Networking::HTTPSClient c(io_service, ctx, server, path);
    io_service.run();
    // ----

    // [2] Get:
    // ----
	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;

	// std::regex regexFreeCashFlowLine("Free Cash Flow.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");


	// Close file
	file.close();

	return true;
}

