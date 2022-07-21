#include <regex>
#include <fstream>

#include "HTTPSProxySrv.h"
#include "HTTPSClient.h"


Service::HTTPSProxySrv::HTTPSProxySrv(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
}


Service::HTTPSProxySrv::~HTTPSProxySrv()
{
}


void Service::HTTPSProxySrv::preInit()
{
	// Get params from DB for this instance
	std::cout << "HTTPSProxySrv preInit() called!" << '\n';
}


void Service::HTTPSProxySrv::postInit()
{

}


// --== HTTPS Access ==--
// INCOME STATEMENT
bool Service::HTTPSProxySrv::getFromIncomeStatement(const std::string& stockTicker,
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
bool Service::HTTPSProxySrv::getRevenueAndEPSPrediction(const std::string& stockTicker,
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
bool Service::HTTPSProxySrv::getFromBalanceSheet(const std::string& stockTicker,
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
bool Service::HTTPSProxySrv::getFromCashFlowStatement(const std::string& stockTicker,
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