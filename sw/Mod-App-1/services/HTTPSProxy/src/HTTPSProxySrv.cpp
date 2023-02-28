#include <regex>
#include <fstream>

#include "HTTPSProxySrv.h"
#include "HTTPSClient.h"


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
	// Get params from DB for this instance
	std::cout << "HTTPSProxySrv preInit() called!" << '\n';
}


void Services::HTTPSProxySrv::postInit()
{

}


/*
[INCOME STATEMENT]

	1] Total Revenue [Exists]
	2] Gross Profit [Exists] - Revenue - Const of Revenue
	3] Net Income Common Stockhold [Exists] - should be consistent
	----

	Gross Margin [%] = Gross Profit / Revenue
	Net Margin [%] = Net Income / Revenue
*/
bool Services::HTTPSProxySrv::_getFromIncomeStatement(const std::string& stockTicker,
	std::vector<double>& revenueVec,
	std::vector<double>& grossProfitVec,
	std::vector<double>& netIncomeVec)
{
	// [1] Preparation
	// ----
	// Create client and get Income Statement (HTML) in _HTTPSContent.txt file
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Income Statement- APPLE
    std::string server("finance.yahoo.com");
    std::string path("/quote/" + stockTicker + "/financials?p=" + stockTicker);

	Networking::HTTPSClient c(io_service, ctx, server, path);
    io_service.run();
    // ----

    // [2] Get: Revenue, Gross Profit Net Income Common Stackhold
    // ----
	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;

	std::regex regexRevenueLine("Total Revenue.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");
	std::regex regexGrossProfitLine("Gross Profit.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");
	std::regex regexNetIncomeLine("Net Income Common Stockholders.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");

	//
	while (std::getline(file, lineFromFile)) 
	{
		// Get Total Revenue
		if (std::regex_search(lineFromFile, match, regexRevenueLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string totalRevenueStr(match[i].str());
			    // Remove "," from data
			    totalRevenueStr.erase(remove(totalRevenueStr.begin(), totalRevenueStr.end(), ','), totalRevenueStr.end());

			    double num = std::stod(totalRevenueStr);
			    // Push back in vector (important - in thousands)
			    if(num != 0) revenueVec.push_back(num);
		    }
		}

		// Get Gross Profit
		if (std::regex_search(lineFromFile, match, regexGrossProfitLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string grossProfitStr(match[i].str());
			    // Remove "," from data
			    grossProfitStr.erase(remove(grossProfitStr.begin(), grossProfitStr.end(), ','), grossProfitStr.end());

			    double num = stod(grossProfitStr);
			    // Push back in vector (important - in thousands)
			    if(num != 0) grossProfitVec.push_back(num);
		    }
		}

		// Get Net Income
		if (std::regex_search(lineFromFile, match, regexNetIncomeLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string netIncomeStr(match[i].str());
			    // Remove "," from data
			    netIncomeStr.erase(remove(netIncomeStr.begin(), netIncomeStr.end(), ','), netIncomeStr.end());

			    double num = stod(netIncomeStr);
			    // Push back in vector (important - in thousands)
			    if(num != 0) netIncomeVec.push_back(num);
		    }
		}
	}

	// Close file
	file.close();
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
	std::vector<double>& shareIssuedVec)
{
	// [1] Preparation
	// ----
	// Create client and get Income Statement (HTML) in _HTTPSContent.txt file
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Income Statement- APPLE
    std::string server("finance.yahoo.com");
    std::string path("/quote/AAPL/balance-sheet?p=AAPL");

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

	std::regex regexTotalEquityLine("Total Equity Gross Minority Interest.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");
	std::regex regexTotalDebtLine("Total Debt.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");
	std::regex regexSharesNumberLine("Share Issued.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");

	//
	while (std::getline(file, lineFromFile)) 
	{
		// Get Total Equity
		if (std::regex_search(lineFromFile, match, regexTotalEquityLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string totalEquityStr(match[i].str());
			    // Remove "," from data
			    totalEquityStr.erase(remove(totalEquityStr.begin(), totalEquityStr.end(), ','), totalEquityStr.end());

			    double num = stod(totalEquityStr);
			    // Push back in vector (important - in thousands)
			    if(num != 0) bookValueVec.push_back(num);
		    }
		}
		// Get Total Debt
		if (std::regex_search(lineFromFile, match, regexTotalDebtLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string totalDebtStr(match[i].str());
			    // Remove "," from data
			    totalDebtStr.erase(remove(totalDebtStr.begin(), totalDebtStr.end(), ','), totalDebtStr.end());

			    double num = stod(totalDebtStr);
			    // Push back in vector (important - in thousands)
			    if(num != 0) totalDebtVec.push_back(num);
		    }
		}
		// Get Number of Shares
		if (std::regex_search(lineFromFile, match, regexSharesNumberLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string totalNumberOfSharesStr(match[i].str());
			    // Remove "," from data
			    totalNumberOfSharesStr.erase(remove(totalNumberOfSharesStr.begin(), totalNumberOfSharesStr.end(), ','), totalNumberOfSharesStr.end());

			    double num = stod(totalNumberOfSharesStr);
			    // Push back in vector (important - in thousands)
			    if(i % 2 != 0) shareIssuedVec.push_back(num);
		    }
		}
	}

	// Close file
	file.close();

	return true;
}


// CASH FLOW STATEMENT
bool Services::HTTPSProxySrv::_getFromCashFlowStatement(const std::string& stockTicker,
	std::vector<double>& cashFlowVec)
{
	// [1] Preparation
	// ----
	// Create client and get Income Statement (HTML) in _HTTPSContent.txt file
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Income Statement- APPLE
    std::string server("finance.yahoo.com");
    std::string path("/quote/AAPL/cash-flow?p=AAPL");

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

	std::regex regexFreeCashFlowLine("Free Cash Flow.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");

	//
	while (std::getline(file, lineFromFile)) 
	{
		// Get Free Cash Flow
		if (std::regex_search(lineFromFile, match, regexFreeCashFlowLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string freeCashFlowStr(match[i].str());
			    // Remove "," from data
			    freeCashFlowStr.erase(remove(freeCashFlowStr.begin(), freeCashFlowStr.end(), ','), freeCashFlowStr.end());

			    double num = stod(freeCashFlowStr);
			    // Push back in vector (important - in thousands)
			    if(num != 0) cashFlowVec.push_back(num);
		    }
		}
	}

	// Close file
	file.close();

	return true;
}














// OLD: REMOVE
// --== HTTPS Access ==--
// INCOME STATEMENT
bool Services::HTTPSProxySrv::getFromIncomeStatement(const std::string& stockTicker,
	std::vector<double>& revenueVec,
	std::vector<double>& grossProfitVec,
	std::vector<double>& netIncomeVec)
{
	// 1] --== CREATE CLIENT AND THE FILL FILE "_HTTPSContent.txt" WITH HTML INFO ==--

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Income Statement
    // /quote/AAPL/financials?p=AAPL
    std::string server("finance.yahoo.com");
    std::string path("/quote/" + stockTicker + "/financials?p=" + stockTicker);

    // Create Temp Client
    Networking::HTTPSClient HTTPSClient(io_service, ctx, server, path);
    
    // Run HTTPS Client
    io_service.run();

    // 2] --== OPEN CREATED FILE "_HTTPSContent.txt" AND PARSE FROM INCOME STATEMENT ==--
	std::string _fileName("_HTTPSContent.txt");
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;

	// .*?  - Capture everything until
	std::regex regexLineRevenue("\"annualTotalRevenue\".*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+)");
	std::regex regexLineGrossProfit("\"annualGrossProfit\".*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+)");
	std::regex regexLineNetIncome("\"annualNetIncomeContinuousOperations\".*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+)");

	if (!file.is_open()) 
	{
		std::cout << "[ERROR] Unable to open file!" << '\n';
		return false;
	}

	while (std::getline(file, lineFromFile)) 
	{
		std::cout << "Reading line ..." << '\n';
		// Do regex stuff
		// Get Revenue
		if (std::regex_search(lineFromFile, match, regexLineRevenue))
		{
			for (std::smatch::iterator it = match.begin() + 1; it != match.end(); ++it) 
			{
				revenueVec.push_back(std::stod(*it));
			}

			// Get Cost Of Revenue			
			if (std::regex_search(lineFromFile, match, regexLineGrossProfit))
			{
				for (std::smatch::iterator it = match.begin() + 1; it != match.end(); ++it) 
				{
					grossProfitVec.push_back(std::stod(*it));
				}
			}

			// Get Net Income
			if (std::regex_search(lineFromFile, match, regexLineNetIncome))
			{
				for (std::smatch::iterator it = match.begin() + 1; it != match.end(); ++it) 
				{
					netIncomeVec.push_back(std::stod(*it));
				}
			}
		}
	}

	// Close file
	file.close();

	return true;
}


// FUTURE REVENUE AND EPS STATEMENT
bool Services::HTTPSProxySrv::getRevenueAndEPSPrediction(const std::string& stockTicker,
		std::vector<double>& revenuePredictionVec, 
		std::vector<double>& epsPredictionVec)
{
	// 1] --== CREATE CLIENT AND THE FILL FILE "_HTTPSContent.txt" WITH HTML INFO ==--

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Analysis
    // /quote/AAPL/analysis?p=AAPL
    std::string server("finance.yahoo.com");
    std::string path("/quote/" + stockTicker + "/analysis?p=" + stockTicker);

    // Create Temp Client
    Networking::HTTPSClient HTTPSClient(io_service, ctx, server, path);
    
    // Run HTTPS Client
    io_service.run();

    // 2] --== OPEN CREATED FILE "_HTTPSContent.txt" AND PARSE FROM INCOME STATEMENT ==--
	std::string _fileName("_HTTPSContent.txt");
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;

	// Analysis
	std::regex regexLineRevenueEstimate("\"revenueEstimate\".*?raw\":(\\d+).*?\"revenueEstimate\".*?raw\":(\\d+).*?\"revenueEstimate\".*?raw\":(\\d+).*?\"revenueEstimate\".*?raw\":(\\d+)");
	std::regex regexLineEarningsEstimate("\"earningsEstimate\".*?raw\":(\\d+).*?\"earningsEstimate\".*?raw\":(\\d+).*?\"earningsEstimate\".*?raw\":(\\d+)(\\.\\d+).*?\"earningsEstimate\".*?raw\":(\\d+)(\\.\\d+)");


	if (!file.is_open()) 
	{
		std::cout << "[ERROR] Unable to open file!" << '\n';
		return false;
	}

	while (std::getline(file, lineFromFile)) 
	{
		// Get Revenue Prediction
		if (std::regex_search(lineFromFile, match, regexLineRevenueEstimate))
		{
			revenuePredictionVec.push_back(std::stod(match.str(3))); // Current year revenue
			revenuePredictionVec.push_back(std::stod(match.str(4))); // Next year revenue

			// Get EPS Prediction
			if (std::regex_search(lineFromFile, match, regexLineEarningsEstimate))
			{
				epsPredictionVec.push_back(std::stod(match.str(3) + match.str(4)));
				epsPredictionVec.push_back(std::stod(match.str(5) + match.str(6)));
			}
		}
	}

	// Close file
	file.close();

	return true;
}


// BALANCE SHEET
bool Services::HTTPSProxySrv::getFromBalanceSheet(const std::string& stockTicker,
	std::vector<double>& bookValueVec,
	std::vector<double>& totalDebtVec,
	std::vector<double>& shareIssuedVec)
{
	// 1] --== CREATE CLIENT AND THE FILL FILE "_HTTPSContent.txt" WITH HTML INFO ==--

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Balance Sheet
    // /quote/AAPL/financials?p=AAPL
    std::string server("finance.yahoo.com");
    std::string path("/quote/" + stockTicker + "/balance-sheet?p=" + stockTicker);

    // Create Temp Client
    Networking::HTTPSClient HTTPSClient(io_service, ctx, server, path);
    
    // Run HTTPS Client
    io_service.run();

    // 2] --== OPEN CREATED FILE "_HTTPSContent.txt" AND PARSE FROM INCOME STATEMENT ==--
	std::string _fileName("_HTTPSContent.txt");
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;

	// .*?  - Capture everything until
	std::regex regexLineTotalEquityGrossMinorityInterest("\"annualTotalEquityGrossMinorityInterest\".*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+)");
	std::regex regexLineTotalDebt("\"annualTotalDebt\".*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+)");
	std::regex regexLineShareIssued("\"annualShareIssued\".*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+)");

	if (!file.is_open()) 
	{
		std::cout << "[ERROR] Unable to open file!" << '\n';
		return false;
	}

	while (std::getline(file, lineFromFile)) 
	{
		// Do regex stuff
		// Get Total Debt
		if (std::regex_search(lineFromFile, match, regexLineTotalEquityGrossMinorityInterest))
		{
			for (std::smatch::iterator it = match.begin() + 1; it != match.end(); ++it) 
			{
				bookValueVec.push_back(std::stod(*it));
			}

			// Get Cost Of Revenue			
			if (std::regex_search(lineFromFile, match, regexLineTotalDebt))
			{
				for (std::smatch::iterator it = match.begin() + 1; it != match.end(); ++it) 
				{
					totalDebtVec.push_back(std::stod(*it));
				}
			}

			// Get Net Income
			if (std::regex_search(lineFromFile, match, regexLineShareIssued))
			{
				for (std::smatch::iterator it = match.begin() + 1; it != match.end(); ++it) 
				{
					shareIssuedVec.push_back(std::stod(*it));
				}
			}
		}
	}

	// Close file
	file.close();

	return true;
}


// CASH FLOW STATEMENT
bool Services::HTTPSProxySrv::getFromCashFlowStatement(const std::string& stockTicker,
	std::vector<double>& cashFlowVec,
	double& stockPrice)
{
	// 1] --== CREATE CLIENT AND THE FILL FILE "_HTTPSContent.txt" WITH HTML INFO ==--

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Cash Flow
    // /quote/AAPL/cash-flow?p=AAPL
    std::string server("finance.yahoo.com");
    std::string path("/quote/" + stockTicker + "/cash-flow?p=" + stockTicker);

    // Create Temp Client
    Networking::HTTPSClient HTTPSClient(io_service, ctx, server, path);
    
    // Run HTTPS Client
    io_service.run();

    // 2] --== OPEN CREATED FILE "_HTTPSContent.txt" AND PARSE FROM INCOME STATEMENT ==--
	std::string _fileName("_HTTPSContent.txt");
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;

	// Free Cash Flow
	std::regex regexLineCashFlow("\"annualFreeCashFlow\".*?raw\":(\\d+).*?raw\":(\\d+).*?raw\":(\\d+).*?raw\":(\\d+)");
	std::regex regexLineStockPrice("FIN_TICKER_PRICE&quot;:&quot;(\\d*\\.?\\d+)");

	if (!file.is_open()) 
	{
		std::cout << "[ERROR] Unable to open file!" << '\n';
		return false;
	}

	while (std::getline(file, lineFromFile)) 
	{
		// Do regex stuff
		// Get Total Debt
		if (std::regex_search(lineFromFile, match, regexLineCashFlow))
		{
			// Get Free Cash Flow Vec
			for (std::smatch::iterator it = match.begin() + 1; it != match.end(); ++it) 
			{
				cashFlowVec.push_back(std::stod(*it));
			}

			// Get Stock Price
			if (std::regex_search(lineFromFile, match, regexLineStockPrice))
			{
				std::cout << "match.str(1): " << std::stod(match.str(1)) << '\n';
				stockPrice = std::stod(match.str(1));
			}
		}
	}

	// Close file
	file.close();

	return true;
}