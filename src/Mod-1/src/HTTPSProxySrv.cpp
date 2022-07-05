#include <regex>
#include <fstream>

#include "HTTPSProxySrv.h"
#include "Factory.h"

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


void Service::HTTPSProxySrv::testFactory()
{
	Common::Factory::getInstance().testFactoryPrint();
}


void Service::HTTPSProxySrv::setFactoryImplPtr(Common::FactoryIf& factoryPtr)
{
	m_factoryImplSingleton = &factoryPtr;
}


void Service::HTTPSProxySrv::testFactoryImpl()
{
	m_factoryImplSingleton->doSomething();
}


void Service::HTTPSProxySrv::setService(ServiceIf* srvIfPtr)
{
	// Call some method via ServiceIf
	srvIfPtr->preInit();
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
			revenueVec.push_back(std::stod(match.str(1)));
			revenueVec.push_back(std::stod(match.str(2)));
			revenueVec.push_back(std::stod(match.str(3)));
			revenueVec.push_back(std::stod(match.str(4)));

			// Get Cost Of Revenue			
			if (std::regex_search(lineFromFile, match, regexLineGrossProfit))
			{
				grossProfitVec.push_back(std::stod(match.str(1)));
				grossProfitVec.push_back(std::stod(match.str(2)));
				grossProfitVec.push_back(std::stod(match.str(3)));
				grossProfitVec.push_back(std::stod(match.str(4)));
			}

			// Get Net Income
			if (std::regex_search(lineFromFile, match, regexLineNetIncome))
			{
				netIncomeVec.push_back(std::stod(match.str(1)));
				netIncomeVec.push_back(std::stod(match.str(2)));
				netIncomeVec.push_back(std::stod(match.str(3)));
				netIncomeVec.push_back(std::stod(match.str(4)));
			}
		}
	}

	// Close file
	file.close();

	return true;
}