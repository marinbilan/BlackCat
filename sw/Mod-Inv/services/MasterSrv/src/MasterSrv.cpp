#include "MasterSrv.h"
#include "Factory.h"
#include <map>
#include <regex>

#include "SuperInvestors.h"



Services::MasterSrv::MasterSrv(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "\\")
{
	std::string trace = "[MB][MasterSrv][InvDev] Services::MasterSrv constructor";
	Common::Factory::Factory::getInstance().getClientServerSrv()->TRACE(trace);

	trace = "[MB][MasterSrv][InvDev] Services::MasterSrv name: " + m_name;
	Common::Factory::Factory::getInstance().getClientServerSrv()->TRACE(trace);
}


Services::MasterSrv::~MasterSrv()
{
	std::cout << "Services::MasterSrv destructor called!" << '\n';
}


const std::string& Services::MasterSrv::getName()
{
	// std::cout << "Services::MasterSrv preInit() called!" << '\n';

	return m_name;
}


void Services::MasterSrv::preInit()
{
	std::cout << "[MB][MasterSrv][InvDev] Services::MasterSrv preInit()" << '\n';

	m_objectsManager = Common::Factory::Factory::getInstance().getObjectsManager();
	m_invDevIfVec = m_objectsManager->getInvDevIfVec();

	// for(auto s : m_invDevIfVec) {}

    // cmd
    std::string commandLineString;
	std::smatch match;

	do
	{
		std::cout << "$ ";
		// Waiting for command
		std::getline(std::cin, commandLineString);

		std::regex oneWordLine("(\\w+)");
		// First word match
		if (std::regex_search(commandLineString, match, oneWordLine))
		{
			if (!match.str(1).compare("analyze"))
			{
				cmdSecondWord(commandLineString);

			}
			if (!match.str(1).compare("help"))
			{
				//cmdHelp();
			}
		}

	} while (commandLineString != "exit");
}


void Services::MasterSrv::postInit()
{

}


void Services::MasterSrv::cmdSecondWord(const std::string& cmdLine) 
{
	std::map<std::string, std::vector<std::string>> portfolioMap;

    portfolioMap.insert({"Test_Portfolio_0", Test_Portfolio_0});
    portfolioMap.insert({"Hillman_Value_Fund", Hillman_Value_Fund});
    portfolioMap.insert({"Robert_Olstein_Olstein_Capital_Management", Robert_Olstein_Olstein_Capital_Management});
    portfolioMap.insert({"Guy_Spier_Aquamarine_Capital", Guy_Spier_Aquamarine_Capital});
    portfolioMap.insert({"FPA_Queens_Road_Small_Cap_Value_Fund", FPA_Queens_Road_Small_Cap_Value_Fund});
    portfolioMap.insert({"Steven_Romick_FPA_Crescent_Fund", Steven_Romick_FPA_Crescent_Fund});
    portfolioMap.insert({"Greenhaven_Associates", Greenhaven_Associates});
    portfolioMap.insert({"Richard_Pzena_Hancock_Classic_Value", Richard_Pzena_Hancock_Classic_Value});
    portfolioMap.insert({"David_Katz_Matrix_Asset_Advisors", David_Katz_Matrix_Asset_Advisors});
    portfolioMap.insert({"Dodge_And_Cox", Dodge_And_Cox});
    portfolioMap.insert({"Mairs_And_Power_Growth_Fund", Mairs_And_Power_Growth_Fund});

     
   
    std::map<std::string, std::vector<std::string>>::iterator it;


// Second word 
				std::regex regexPattern(R"(^\s*\w+\s+(\w+))");
				std::smatch matchResult;

				// Perform the regex search
    			if (std::regex_search(cmdLine, matchResult, regexPattern)) {		
        			std::string secondWord = matchResult[1];  // matchResult[1] contains the second word

					it = portfolioMap.find(secondWord);

					for(const auto& stockName : it->second) {
        				std::cout << stockName << '\n';    
    				}

					// PROCESS DATA FOR EACH INSTANCE (DEVICE)
					m_invDevIfVec[0]->collectData(it->second);
					m_invDevIfVec[0]->calculateData();
					m_invDevIfVec[0]->storeData();  // In DB

					// POSTPROCESS DATA FOR EACH INSTANCE (DEVICE)
					m_invDevIfVec[0]->sortStocksByNetProfitMargin();
					m_invDevIfVec[0]->sortStocksByNetProfitMarginForPrint();
					m_invDevIfVec[0]->printStocksByNetProfitMargin();

					m_invDevIfVec[0]->sortStocksByYearsToReturnDebt();
					m_invDevIfVec[0]->sortStocksByYearsToReturnDebtForPrint();
					m_invDevIfVec[0]->printStocksByYearsToReturnDebt();

					// Calculate PE and FCF, but not print
					m_invDevIfVec[0]->sortStocksByReturnOnEquityRatio();
					m_invDevIfVec[0]->sortStocksByPERatio();
					m_invDevIfVec[0]->sortStocksByPriceToBookValue();

					//
					// FINAL VALUE SCORE
					m_invDevIfVec[0]->sortStocksByTotalScore();
					m_invDevIfVec[0]->printStocksByTotalScore();

					// DCF INTRINSIC VALUE
					m_invDevIfVec[0]->sortStocksByIntrinsicValue();
					m_invDevIfVec[0]->printStocksByIntrinsicValue();

    			} else {
        			std::cout << "Unknown Portfolio!" << std::endl;
    			}
}