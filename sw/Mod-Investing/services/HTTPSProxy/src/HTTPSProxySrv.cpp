#include <regex>
#include <fstream>

#include "HTTPSProxySrv.h"
#include "HTTPSClient.h"



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
bool Services::HTTPSProxySrv::_getFromSummary(const std::string& stockTicker, double& stockPrice, double& PE_Ratio)
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

	while (std::getline(file, lineFromFile)) 
	{
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
	1] Total Revenue [Exists]
	2] Gross Profit [Exists] = Revenue - Cost of Revenue
	3] Net Income (Common Stockhold) [Exists] -- should be consistent
	----
	Gross Margin [%] = Gross Profit / Revenue
	Net Margin [%] = Net Income / Revenue
*/
bool Services::HTTPSProxySrv::_getFromIncomeStatement(const std::string& stockTicker,
	std::vector<double>& revenueVec,
	std::vector<double>& grossProfitVec,
	std::vector<double>& netIncomeVec, bool standard)
{
	// [1] Preparation
	// ----
	// Create client and get Income Statement (HTML) in _HTTPSContent.txt file
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Income Statement
    std::string server("finance.yahoo.com");
    std::string path("/quote/" + stockTicker + "/financials?p=" + stockTicker);

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

	// bool doStandard = true;

	if(standard) {
		regexRevenueLine = "Total Revenue.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
		regexGrossProfitLine = "Gross Profit.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
		regexNetIncomeLine = "Net Income Common Stockholders.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
	} else {
		regexRevenueLine = "Total Revenue.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
		regexGrossProfitLine = "Gross Profit.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
		regexNetIncomeLine = "Net Income Common Stockholders.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
	}
	//
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
					revenueVec.push_back(num);
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
					grossProfitVec.push_back(num);
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
					netIncomeVec.push_back(num);
				}
		    }
		}
	}

	// Close file
	file.close();


	// Remove first (TTM) element
	if(standard) {
		revenueVec.erase(revenueVec.begin());
		grossProfitVec.erase(grossProfitVec.begin());
		netIncomeVec.erase(netIncomeVec.begin());
	} else {
		// Do nothing
	}	

	// Reverse elems in vec
	std::reverse(revenueVec.begin(), revenueVec.end());
	std::reverse(grossProfitVec.begin(), grossProfitVec.end());
	std::reverse(netIncomeVec.begin(), netIncomeVec.end());


	std::cout << " ---- REVENUE ----" << '\n';
	for(auto s : revenueVec) {
		std::cout << s << " ";
	}
	std::cout << '\n';

	std::cout << " ---- GROSS PROFIT ----" << '\n';
	for(auto s : grossProfitVec) {
		std::cout << s << " ";
	}
	std::cout << '\n';

	std::cout << " ---- NET INCOME ----" << '\n';
	for(auto s : netIncomeVec) {
		std::cout << s << " ";
	}
	std::cout << '\n';


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
	4] Retained Earnings - Added to or widraw (If company invest - Growth)
	5] Total Equity Gross Minority Interest [Equity]

	To measure how efficient company use this reinvestment (Retained Earnings) we calc:
	Return on Equity [%] = Net Income / Total Equity

	----
	Return on Net Tangible assets - More sofisticated method of how efficient
	company use retained earnings (Book Esseys of Warren Buffett)
	----

	Look to bussiness with no Long Term Debt
	6] Total Debt
	If company can pay whole Total Debt with 4 years Earnings, that is good position
*/
bool Services::HTTPSProxySrv::_getFromBalanceSheet(const std::string& stockTicker,
	std::vector<double>& bookValueVec,
	std::vector<double>& totalDebtVec,
	std::vector<double>& shareIssuedVec, bool standard)
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
    std::string path("/quote/" + stockTicker + "/balance-sheet?p=" + stockTicker);

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

	// bool doStandard = true;

	if(standard) {
		regexTotalEquityLine = "Total Equity Gross Minority Interest.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";	
		regexTotalDebtLine = "Total Debt.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
		regexSharesNumberLine = "Share Issued.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
	} else {
		regexTotalEquityLine = "Total Equity Gross Minority Interest.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";	
		regexTotalDebtLine = "Total Debt.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
		regexSharesNumberLine = "Share Issued.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";

	}
	//
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
					bookValueVec.push_back(num);
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
					totalDebtVec.push_back(num);
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
					shareIssuedVec.push_back(num);
				}
		    }
		}
	}

	// Close file
	file.close();


	// DO NOT NEED TO Remove first element TTM

	// Reverse elems in vec
	std::reverse(bookValueVec.begin(), bookValueVec.end());
	std::reverse(totalDebtVec.begin(), totalDebtVec.end());
	std::reverse(shareIssuedVec.begin(), shareIssuedVec.end());
	
	
	std::cout << " ---- BOOK VALUE ----" << '\n';
	for(auto s : bookValueVec) {
		std::cout << s << " ";
	}
	std::cout << '\n';

	std::cout << " ---- DEBT ----" << '\n';
	for(auto s : totalDebtVec) {
		std::cout << s << " ";
	}
	std::cout << '\n';

	std::cout << " ---- SHARED ISSUE ----" << '\n';
	for(auto s : shareIssuedVec) {
		std::cout << s << " ";
	}
	std::cout << '\n';
	

	return true;
}


// CASH FLOW STATEMENT
bool Services::HTTPSProxySrv::_getFromCashFlowStatement(const std::string& stockTicker,
	std::vector<double>& cashFlowVec, bool standard)
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
	std::regex regexFreeCashFlowLine;


	// bool doStandard = true;

	if(standard) {
		regexFreeCashFlowLine = "Free Cash Flow.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
	} else {
		regexFreeCashFlowLine = "Free Cash Flow.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)";
	}

	//
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
					cashFlowVec.push_back(num);
				}
				
		    }
		}
	}

	// Close file
	file.close();

	// Check if we get on standard way
	if(!cashFlowVec.size()) return false;


	// Remove first element TTM
	if(standard) {
		cashFlowVec.erase(cashFlowVec.begin());
	} else {
		// Do nothing
	}
	
	// Reverse elems in vec
	std::reverse(cashFlowVec.begin(), cashFlowVec.end());


	std::cout << " ---- FREE CASH FLOW ----" << '\n';
	for(auto s : cashFlowVec) {
		std::cout << s << " ";
	}
	std::cout << '\n';


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


// ==== LINEAR REGRESSION ====
bool Services::HTTPSProxySrv::calcLinearRegressCoeffs(const std::vector<double>& x, 
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