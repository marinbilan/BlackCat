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

    portfolioMap.insert({"My_Portfolio", My_Portfolio});
    portfolioMap.insert({"Demo", Demo});
    portfolioMap.insert({"Top_Companies", Top_Companies});

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
    portfolioMap.insert({"Tweedy_Browne_Value_Fund", Tweedy_Browne_Value_Fund});
    portfolioMap.insert({"Wallace_Weitz_Weitz_Large_Cap_Equity_Fund", Wallace_Weitz_Weitz_Large_Cap_Equity_Fund});
    portfolioMap.insert({"Lindsell_Train", Lindsell_Train});
    // [8.8.24]
    portfolioMap.insert({"Yacktman_Asset_Management", Yacktman_Asset_Management});
    portfolioMap.insert({"Thomas_Gayner_Markel_Group", Thomas_Gayner_Markel_Group});
    portfolioMap.insert({"William_Von_Mueffling_Cantillon_Capital_Management", William_Von_Mueffling_Cantillon_Capital_Management});
    portfolioMap.insert({"Torray_Funds", Torray_Funds});
    portfolioMap.insert({"John_Rogers_Ariel_Appreciation_Fund", John_Rogers_Ariel_Appreciation_Fund});
    portfolioMap.insert({"Charles_Bobrinskoy_Ariel_Focus_Fund", Charles_Bobrinskoy_Ariel_Focus_Fund});
    portfolioMap.insert({"Robert_Vinall", Robert_Vinall});

    portfolioMap.insert({"David_Abrams_Abrams_Capital_Management", David_Abrams_Abrams_Capital_Management});
    portfolioMap.insert({"Polen_Capital_Management", Polen_Capital_Management});
    portfolioMap.insert({"First_Eagle_Investment_Management", First_Eagle_Investment_Management});
    portfolioMap.insert({"AKO_Capital", AKO_Capital});
    portfolioMap.insert({"John_Armitage_Egerton_Capital", John_Armitage_Egerton_Capital});

    portfolioMap.insert({"Harry_Burn_Sound_Shore", Harry_Burn_Sound_Shore});
    portfolioMap.insert({"Thomas_Russo_Gardner_Russo_Quinn", Thomas_Russo_Gardner_Russo_Quinn});
    portfolioMap.insert({"Christopher_Davis_Davis_Advisors", Christopher_Davis_Davis_Advisors});
    portfolioMap.insert({"Third_Avenue_Management", Third_Avenue_Management});
    portfolioMap.insert({"Chuck_Akre_Akre_Capital_Management", Chuck_Akre_Akre_Capital_Management});

	portfolioMap.insert({"David_Rolfe_Wedgewood_Partners", David_Rolfe_Wedgewood_Partners});
	portfolioMap.insert({"Bill_Miller_Miller_Value_Partners", Bill_Miller_Miller_Value_Partners});
	portfolioMap.insert({"Seth_Klarman_Baupost_Group", Seth_Klarman_Baupost_Group});

	portfolioMap.insert({"Dennis_Hong_ShawSpring_Partners", Dennis_Hong_ShawSpring_Partners});
	portfolioMap.insert({"Howard_Marks_Oaktree_Capital_Management", Howard_Marks_Oaktree_Capital_Management});

	portfolioMap.insert({"Greg_Alexander_Conifer_Management", Greg_Alexander_Conifer_Management});
	portfolioMap.insert({"Prem_Watsa_Fairfax_Financial_Holdings", Prem_Watsa_Fairfax_Financial_Holdings});
	portfolioMap.insert({"Francis_Chou_Chou_Associates", Francis_Chou_Chou_Associates});

	portfolioMap.insert({"Terry_Smith_Fundsmith", Terry_Smith_Fundsmith});
	portfolioMap.insert({"Pat_Dorsey_Dorsey_Asset_Management", Pat_Dorsey_Dorsey_Asset_Management});
	portfolioMap.insert({"Clifford_Sosin_CAS_Investment_Partners", Clifford_Sosin_CAS_Investment_Partners});
	portfolioMap.insert({"Leon_Cooperman", Leon_Cooperman});

	portfolioMap.insert({"Chris_Hohn_TCI_Fund_Management", Chris_Hohn_TCI_Fund_Management});
	portfolioMap.insert({"Samantha_McLemore_Patient_Capital_Management", Samantha_McLemore_Patient_Capital_Management});

	portfolioMap.insert({"Bryan_Lawrence_Oakcliff_Capital", Bryan_Lawrence_Oakcliff_Capital});
	portfolioMap.insert({"Josh_Tarasoff_Greenlea_Lane_Capital", Josh_Tarasoff_Greenlea_Lane_Capital});
	portfolioMap.insert({"Triple_Frond_Partners", Triple_Frond_Partners});
	portfolioMap.insert({"Francois_Rochon_Giverny_Capital", Francois_Rochon_Giverny_Capital});

	portfolioMap.insert({"Kahn_Brothers_Group", Kahn_Brothers_Group});
	portfolioMap.insert({"AltaRock_Partners", AltaRock_Partners});
	portfolioMap.insert({"Bill_Ackman_Pershing_Square_Capital_Management", Bill_Ackman_Pershing_Square_Capital_Management});
	portfolioMap.insert({"David_Tepper_Appaloosa_Management", David_Tepper_Appaloosa_Management});

	// [8.9.24]
	portfolioMap.insert({"Glenn_Welling_Engaged_Capital", Glenn_Welling_Engaged_Capital});
	portfolioMap.insert({"Glenn_Greenberg_Brave_Warrior_Advisors", Glenn_Greenberg_Brave_Warrior_Advisors});
	portfolioMap.insert({"Bruce_Berkowitz_Fairholme_Capital", Bruce_Berkowitz_Fairholme_Capital});
	portfolioMap.insert({"David_Einhorn_Greenlight_Capital", David_Einhorn_Greenlight_Capital});
	portfolioMap.insert({"Nelson_Peltz_Trian_Fund_Management", Nelson_Peltz_Trian_Fund_Management});
	portfolioMap.insert({"Norbert_Lou_Punch_Card_Management", Norbert_Lou_Punch_Card_Management});
	portfolioMap.insert({"Carl_Icahn_Icahn_Capital_Management", Carl_Icahn_Icahn_Capital_Management});
	portfolioMap.insert({"Daniel_Loeb_Third_Point", Daniel_Loeb_Third_Point});
	portfolioMap.insert({"Lee_Ainslie_Maverick_Capital", Lee_Ainslie_Maverick_Capital});
	portfolioMap.insert({"Christopher_Bloomstran_Semper_Augustus", Christopher_Bloomstran_Semper_Augustus});
	portfolioMap.insert({"Alex_Roepers_Atlantic_Investment_Management", Alex_Roepers_Atlantic_Investment_Management});
	portfolioMap.insert({"ValueAct_Capital", ValueAct_Capital});
	portfolioMap.insert({"Stephen_Mandel_Lone_Pine_Capital", Stephen_Mandel_Lone_Pine_Capital});
	portfolioMap.insert({"Valley_Forge_Capital_Management", Valley_Forge_Capital_Management});
	portfolioMap.insert({"Viking_Global_Investors", Viking_Global_Investors});
	portfolioMap.insert({"Tom_Bancroft_Makaira_Partners", Tom_Bancroft_Makaira_Partners});
	portfolioMap.insert({"Ruane_Cunniff_Sequoia_Fund", Ruane_Cunniff_Sequoia_Fund});
	portfolioMap.insert({"Mason_Hawkins_Longleaf_Partners", Mason_Hawkins_Longleaf_Partners});
	portfolioMap.insert({"Bill_Nygren_Oakmark_Select_Fund", Bill_Nygren_Oakmark_Select_Fund});
	portfolioMap.insert({"Meridian_Contrarian_Fund", Meridian_Contrarian_Fund});
	portfolioMap.insert({"Warren_Buffett", Warren_Buffett});
	portfolioMap.insert({"Chase_Coleman_Tiger_Global_Management", Chase_Coleman_Tiger_Global_Management});
	portfolioMap.insert({"Michael_Burry_Scion_Asset_Management", Michael_Burry_Scion_Asset_Management});


	// Small-Cap ISP6 ($300 Million to $2 Billion): Over 1,000 stocks
    portfolioMap.insert({"ISP6_Communication", ISP6_Communication});
    portfolioMap.insert({"ISP6_ConsumerDiscretionary", ISP6_ConsumerDiscretionary});
    portfolioMap.insert({"ISP6_ConsumerStaples", ISP6_ConsumerStaples});
    portfolioMap.insert({"ISP6_Energy", ISP6_Energy});
    portfolioMap.insert({"ISP6_Financials", ISP6_Financials});
    portfolioMap.insert({"ISP6_HealthCare", ISP6_HealthCare});
    portfolioMap.insert({"ISP6_Industrials", ISP6_Industrials});
    portfolioMap.insert({"ISP6_IT", ISP6_IT});
    portfolioMap.insert({"ISP6_Materials", ISP6_Materials});
    portfolioMap.insert({"ISP6_RealEstate", ISP6_RealEstate});
    portfolioMap.insert({"ISP6_Utilities", ISP6_Utilities});


    // Micro-Cap iShares IWC ($50 Million to $300 Million): Over 1,500 stocks
    portfolioMap.insert({"IWC_IT", IWC_IT});  // 166 Companies


    /*
    portfolioMap.insert({"Bill_Nygren_Oakmark_Select_Fund", Bill_Nygren_Oakmark_Select_Fund});
    portfolioMap.insert({"Ruane_Cunniff_Sequoia_Fund", Ruane_Cunniff_Sequoia_Fund});
    portfolioMap.insert({"Meridian_Contrarian_Fund", Meridian_Contrarian_Fund});
    portfolioMap.insert({"Charles_Bobrinskoy_Ariel_Focus_Fund", Charles_Bobrinskoy_Ariel_Focus_Fund});
    portfolioMap.insert({"Glenn_Greenberg_Brave_Warrior_Advisors", Glenn_Greenberg_Brave_Warrior_Advisors});
    portfolioMap.insert({"Alex_Roepers_Atlantic_Investment_Management", Alex_Roepers_Atlantic_Investment_Management});
    portfolioMap.insert({"Nelson_Peltz_Trian_Fund_Management", Nelson_Peltz_Trian_Fund_Management});
    portfolioMap.insert({"Valley_Forge_Capital_Management", Valley_Forge_Capital_Management});
    portfolioMap.insert({"ValueAct_Capital", ValueAct_Capital});
    portfolioMap.insert({"Bill_Melinda_Gates_Foundation_Trust", Bill_Melinda_Gates_Foundation_Trust});
    portfolioMap.insert({"Bill_Ackman_Pershing_Square_Capital_Management", Bill_Ackman_Pershing_Square_Capital_Management});
    portfolioMap.insert({"Lee_Ainslie_Maverick_Capital", Lee_Ainslie_Maverick_Capital});
	portfolioMap.insert({"Carl_Icahn_Icahn_Capital_Management", Carl_Icahn_Icahn_Capital_Management});
    portfolioMap.insert({"Daniel_Loeb_Third_Point", Daniel_Loeb_Third_Point});
    portfolioMap.insert({"Stephen_Mandel_Lone_Pine_Capital", Stephen_Mandel_Lone_Pine_Capital});
    portfolioMap.insert({"Mason_Hawkins_Longleaf_Partners", Mason_Hawkins_Longleaf_Partners});
    portfolioMap.insert({"David_Rolfe_Wedgewood_Partners", David_Rolfe_Wedgewood_Partners});
    portfolioMap.insert({"David_Einhorn_Greenlight_Capital", David_Einhorn_Greenlight_Capital});
    portfolioMap.insert({"John_Rogers_Ariel_Appreciation_Fund", John_Rogers_Ariel_Appreciation_Fund});
	*/




   
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

					// NEW NEW NEW NEW NEW 
					m_invDevIfVec[0]->_new_EvaluateCompanies();

					// PRINT
					m_invDevIfVec[0]->_new_sortCompaniesByTotalScore();
					m_invDevIfVec[0]->_new_printCompaniesByTotalScore();

					m_invDevIfVec[0]->_new_sortCompaniesByIntrinsicValue();
					m_invDevIfVec[0]->_new_printCompaniesByIntrinsicValue();
					// NEW NEW NEW NEW NEW 


					
					/*
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
					*/

    			} else {
        			std::cout << "Unknown Portfolio!" << std::endl;
    			}
}