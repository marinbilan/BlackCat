#include "MasterSrv.h"
#include "Factory.h"
#include <map>
#include <set>
#include <regex>
#include <chrono>

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
				auto start = std::chrono::high_resolution_clock::now();
				cmdSecondWord(commandLineString);
				auto end = std::chrono::high_resolution_clock::now();

				// Duration in seconds
    			auto total_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    			std::cout << ">>>>>>>> Elapsed time: " << total_seconds << " seconds\n";

    			int hours = total_seconds / 3600;
    			int minutes = (total_seconds % 3600) / 60;
    			int seconds = total_seconds % 60;

			    std::cout << "Elapsed time: "
			              << hours << "h "
			              << minutes << "m "
			              << seconds << "s\n";
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

/*
Position:
cd ~/dev/Investing/agents/video/remotion

npx remotion render StockAnalysis out/kspi_analysis.mp4 --crf=26  // Video
npx remotion still Thumbnail out/kspi_thumbnail.png               // Thumbnail
npx remotion render Short out/cb_analysis.mp4 --crf=26            // Short

*/

	portfolioMap.insert({"Selection", Selection});
    portfolioMap.insert({"My_Portfolio", My_Portfolio});

    portfolioMap.insert({"Demo", Demo});
	portfolioMap.insert({"Reits", Reits});

    // SuperInvestors
	portfolioMap.insert({"Robert_Olstein_Olstein_Capital_Management", Robert_Olstein_Olstein_Capital_Management});  // BIG
	portfolioMap.insert({"David_Katz_Matrix_Asset_Advisors", David_Katz_Matrix_Asset_Advisors});  // BIG 106
	portfolioMap.insert({"Richard_Pzena_1", Richard_Pzena_1});
	portfolioMap.insert({"Richard_Pzena_2", Richard_Pzena_2});
	portfolioMap.insert({"Thomas_Gayner_Markel_Group_1", Thomas_Gayner_Markel_Group_1});
	portfolioMap.insert({"Thomas_Gayner_Markel_Group_2", Thomas_Gayner_Markel_Group_2});
	portfolioMap.insert({"Christopher_Davis_Davis_Advisors_1", Christopher_Davis_Davis_Advisors_1});  // BIG
	portfolioMap.insert({"Christopher_Davis_Davis_Advisors_2", Christopher_Davis_Davis_Advisors_2});
	portfolioMap.insert({"Greenhaven_Associates", Greenhaven_Associates});	
	portfolioMap.insert({"Guy_Spier_Aquamarine_Capital", Guy_Spier_Aquamarine_Capital});
	portfolioMap.insert({"Lindsell_Train", Lindsell_Train});
	portfolioMap.insert({"Yacktman_Asset_Management", Yacktman_Asset_Management});
	portfolioMap.insert({"Torray_Funds", Torray_Funds});
	portfolioMap.insert({"Jensen_Investment_Management", Jensen_Investment_Management});
	portfolioMap.insert({"Tweedy_Browne_Value_Fund", Tweedy_Browne_Value_Fund});
	portfolioMap.insert({"William_Von_Mueffling_Cantillon_Capital_Management", William_Von_Mueffling_Cantillon_Capital_Management});
	portfolioMap.insert({"Harry_Burn_Sound_Shore", Harry_Burn_Sound_Shore});
	portfolioMap.insert({"Kahn_Brothers_Group", Kahn_Brothers_Group});
	portfolioMap.insert({"Josh_Tarasoff_Greenlea_Lane_Capital", Josh_Tarasoff_Greenlea_Lane_Capital});
	portfolioMap.insert({"Robert_Vinall", Robert_Vinall});
	portfolioMap.insert({"Vulcan_Value_Partners", Vulcan_Value_Partners});
	portfolioMap.insert({"Steven_Check_Check_Capital_Management", Steven_Check_Check_Capital_Management});
	portfolioMap.insert({"Mohnish_Pabrai_Pabrai_Investments", Mohnish_Pabrai_Pabrai_Investments});
	portfolioMap.insert({"Thomas_Russo_Gardner_Russo_Quinn", Thomas_Russo_Gardner_Russo_Quinn});
	portfolioMap.insert({"John_Armitage_Egerton_Capital", John_Armitage_Egerton_Capital});
	portfolioMap.insert({"Third_Avenue_Management", Third_Avenue_Management});
	portfolioMap.insert({"Mairs_And_Power_Growth_Fund_1", Mairs_And_Power_Growth_Fund_1});
	portfolioMap.insert({"Mairs_And_Power_Growth_Fund_2", Mairs_And_Power_Growth_Fund_2});
	portfolioMap.insert({"Mairs_And_Power_Growth_Fund_3", Mairs_And_Power_Growth_Fund_3});
	portfolioMap.insert({"First_Pacific_Advisors", First_Pacific_Advisors});
	portfolioMap.insert({"Polen_Capital_Management_1", Polen_Capital_Management_1});
	portfolioMap.insert({"Polen_Capital_Management_2", Polen_Capital_Management_2});
	portfolioMap.insert({"Polen_Capital_Management_3", Polen_Capital_Management_3});
	portfolioMap.insert({"Samantha_McLemore_Patient_Capital_Management", Samantha_McLemore_Patient_Capital_Management});
	portfolioMap.insert({"Francis_Chou_Chou_Associates", Francis_Chou_Chou_Associates});
	portfolioMap.insert({"Dodge_And_Cox_1", Dodge_And_Cox_1});
	portfolioMap.insert({"Dodge_And_Cox_2", Dodge_And_Cox_2});
	portfolioMap.insert({"Dodge_And_Cox_3", Dodge_And_Cox_3});
	portfolioMap.insert({"Wallace_Weitz_Weitz_Large_Cap_Equity_Fund", Wallace_Weitz_Weitz_Large_Cap_Equity_Fund});
	portfolioMap.insert({"Sarah_Ketterer_Causeway_Capital_Management_1", Sarah_Ketterer_Causeway_Capital_Management_1});
	portfolioMap.insert({"Sarah_Ketterer_Causeway_Capital_Management_2", Sarah_Ketterer_Causeway_Capital_Management_2});
	portfolioMap.insert({"Dennis_Hong_ShawSpring_Partners", Dennis_Hong_ShawSpring_Partners});
	portfolioMap.insert({"Seth_Klarman_Baupost_Group", Seth_Klarman_Baupost_Group});
	portfolioMap.insert({"First_Eagle_Investment_Management_1", First_Eagle_Investment_Management_1});  // BIG 420 Stocks
	portfolioMap.insert({"First_Eagle_Investment_Management_2", First_Eagle_Investment_Management_2});
	portfolioMap.insert({"First_Eagle_Investment_Management_3", First_Eagle_Investment_Management_3});
	portfolioMap.insert({"First_Eagle_Investment_Management_4", First_Eagle_Investment_Management_4});
	portfolioMap.insert({"First_Eagle_Investment_Management_5", First_Eagle_Investment_Management_5});
	portfolioMap.insert({"Chuck_Akre_Akre_Capital_Management", Chuck_Akre_Akre_Capital_Management});
	portfolioMap.insert({"David_Abrams_Abrams_Capital_Management", David_Abrams_Abrams_Capital_Management});
	portfolioMap.insert({"John_Rogers_Ariel_Appreciation_Fund_1", John_Rogers_Ariel_Appreciation_Fund_1}); // BIG 106
	portfolioMap.insert({"John_Rogers_Ariel_Appreciation_Fund_2", John_Rogers_Ariel_Appreciation_Fund_2});
	portfolioMap.insert({"Greg_Alexander_Conifer_Management", Greg_Alexander_Conifer_Management});
	portfolioMap.insert({"Ruane_Cunniff_Sequoia_Fund", Ruane_Cunniff_Sequoia_Fund});
	portfolioMap.insert({"Hillman_Value_Fund", Hillman_Value_Fund});
	portfolioMap.insert({"Prem_Watsa_Fairfax_Financial_Holdings", Prem_Watsa_Fairfax_Financial_Holdings});
	portfolioMap.insert({"Terry_Smith_Fundsmith", Terry_Smith_Fundsmith});
	portfolioMap.insert({"AKO_Capital", AKO_Capital});
	portfolioMap.insert({"Pat_Dorsey_Dorsey_Asset_Management", Pat_Dorsey_Dorsey_Asset_Management});
	portfolioMap.insert({"Bill_Miller_Miller_Value_Partners", Bill_Miller_Miller_Value_Partners});
	portfolioMap.insert({"Bryan_Lawrence_Oakcliff_Capital", Bryan_Lawrence_Oakcliff_Capital});
	portfolioMap.insert({"Arnold_Van_Den_Berg_Century_Management_1", Arnold_Van_Den_Berg_Century_Management_1});  // BIG 120
	portfolioMap.insert({"Arnold_Van_Den_Berg_Century_Management_2", Arnold_Van_Den_Berg_Century_Management_2});
	portfolioMap.insert({"Leon_Cooperman", Leon_Cooperman});
	portfolioMap.insert({"Francois_Rochon_Giverny_Capital", Francois_Rochon_Giverny_Capital});
	portfolioMap.insert({"AltaRock_Partners", AltaRock_Partners});
	portfolioMap.insert({"Clifford_Sosin_CAS_Investment_Partners", Clifford_Sosin_CAS_Investment_Partners});
	portfolioMap.insert({"Glenn_Greenberg_Brave_Warrior_Advisors", Glenn_Greenberg_Brave_Warrior_Advisors});
	portfolioMap.insert({"Chase_Coleman_Tiger_Global_Management", Chase_Coleman_Tiger_Global_Management});
	portfolioMap.insert({"Li_Lu_Himalaya_Capital_Management", Li_Lu_Himalaya_Capital_Management});
	portfolioMap.insert({"David_Tepper_Appaloosa_Management", David_Tepper_Appaloosa_Management});
	portfolioMap.insert({"David_Rolfe_Wedgewood_Partners", David_Rolfe_Wedgewood_Partners});
	portfolioMap.insert({"Valley_Forge_Capital_Management", Valley_Forge_Capital_Management});
	portfolioMap.insert({"Alex_Roepers_Atlantic_Investment_Management", Alex_Roepers_Atlantic_Investment_Management});
	portfolioMap.insert({"Stephen_Mandel_Lone_Pine_Capital", Stephen_Mandel_Lone_Pine_Capital});
	portfolioMap.insert({"Warren_Buffett", Warren_Buffett});
	portfolioMap.insert({"Daniel_Loeb_Third_Point", Daniel_Loeb_Third_Point});
	portfolioMap.insert({"Mason_Hawkins_Longleaf_Partners", Mason_Hawkins_Longleaf_Partners});  // DONE
	portfolioMap.insert({"Christopher_Bloomstran_Semper_Augustus", Christopher_Bloomstran_Semper_Augustus});
	portfolioMap.insert({"Henry_Ellenbogen", Henry_Ellenbogen});
	portfolioMap.insert({"Muhlenkamp", Muhlenkamp});
	portfolioMap.insert({"Norbert_Lou_Punch_Card_Management", Norbert_Lou_Punch_Card_Management});
	portfolioMap.insert({"Bill_Ackman_Pershing_Square_Capital_Management", Bill_Ackman_Pershing_Square_Capital_Management});
	portfolioMap.insert({"Bill_Nygren_Oakmark_Select_Fund_1", Bill_Nygren_Oakmark_Select_Fund_1});  // BIG 161
	portfolioMap.insert({"Bill_Nygren_Oakmark_Select_Fund_2", Bill_Nygren_Oakmark_Select_Fund_2});
	portfolioMap.insert({"Bruce_Berkowitz_Fairholme_Capital", Bruce_Berkowitz_Fairholme_Capital});
	portfolioMap.insert({"Carl_Icahn_Icahn_Capital_Management", Carl_Icahn_Icahn_Capital_Management});
	portfolioMap.insert({"David_Einhorn_Greenlight_Capital", David_Einhorn_Greenlight_Capital});
	portfolioMap.insert({"ValueAct_Capital", ValueAct_Capital});
	portfolioMap.insert({"Viking_Global_Investors", Viking_Global_Investors});
	portfolioMap.insert({"Lee_Ainslie_Maverick_Capital_1", Lee_Ainslie_Maverick_Capital_1});  // BIG 177
	portfolioMap.insert({"Lee_Ainslie_Maverick_Capital_2", Lee_Ainslie_Maverick_Capital_2});
	portfolioMap.insert({"Nelson_Peltz_Trian_Fund_Management", Nelson_Peltz_Trian_Fund_Management});
	portfolioMap.insert({"Abrams_Bison_Investments", Abrams_Bison_Investments});
	portfolioMap.insert({"Duan_Yongping", Duan_Yongping});
	portfolioMap.insert({"Howard_Marks_Oaktree_Capital_Management", Howard_Marks_Oaktree_Capital_Management});
	portfolioMap.insert({"Steven_Romick_FPA_Crescent_Fund", Steven_Romick_FPA_Crescent_Fund});
	portfolioMap.insert({"FPA_Queens_Road_Small_Cap_Value_Fund", FPA_Queens_Road_Small_Cap_Value_Fund});		
    portfolioMap.insert({"Tom_Bancroft_Makaira_Partners", Tom_Bancroft_Makaira_Partners});
    portfolioMap.insert({"Kahn_Brothers_Group", Kahn_Brothers_Group});   
    portfolioMap.insert({"Triple_Frond_Partners", Triple_Frond_Partners});  	
	portfolioMap.insert({"Chris_Hohn_TCI_Fund_Management", Chris_Hohn_TCI_Fund_Management});
	portfolioMap.insert({"Glenn_Welling_Engaged_Capital", Glenn_Welling_Engaged_Capital});
	portfolioMap.insert({"Charles_Bobrinskoy_Ariel_Focus_Fund", Charles_Bobrinskoy_Ariel_Focus_Fund});
	portfolioMap.insert({"Meridian_Contrarian_Fund", Meridian_Contrarian_Fund});


	// ==============================================================
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


    // IJH SP400 Mid Cap - https://www.ishares.com/us/products/239763/ishares-core-sp-midcap-etf

    portfolioMap.insert({"IJH_Communication", IJH_Communication});
    portfolioMap.insert({"IJH_ConsumerDiscretionary", IJH_ConsumerDiscretionary});
    portfolioMap.insert({"IJH_ConsumerStaples", IJH_ConsumerStaples});
    portfolioMap.insert({"IJH_Energy", IJH_Energy});
    portfolioMap.insert({"IJH_Financials", IJH_Financials});
    portfolioMap.insert({"IJH_HealthCare", IJH_HealthCare});
    portfolioMap.insert({"IJH_Industrials", IJH_Industrials});
    portfolioMap.insert({"IJH_IT", IJH_IT});
    portfolioMap.insert({"IJH_Materials", IJH_Materials});
    portfolioMap.insert({"IJH_RealEstate", IJH_RealEstate});
    portfolioMap.insert({"IJH_Utilities", IJH_Utilities});


    // IJR SP500 Small Cap - https://www.ishares.com/us/products/239774/ishares-core-sp-smallcap-etf

    portfolioMap.insert({"IJR_Communication", IJR_Communication});
    portfolioMap.insert({"IJR_ConsumerDiscretionary", IJR_ConsumerDiscretionary});
    portfolioMap.insert({"IJR_ConsumerStaples", IJR_ConsumerStaples});
    portfolioMap.insert({"IJR_Energy", IJR_Energy});
    portfolioMap.insert({"IJR_Financials", IJR_Financials});
    portfolioMap.insert({"IJR_HealthCare", IJR_HealthCare});
    portfolioMap.insert({"IJR_Industrials", IJR_Industrials});
    portfolioMap.insert({"IJR_IT", IJR_IT});
    portfolioMap.insert({"IJR_Materials", IJR_Materials});
    portfolioMap.insert({"IJR_RealEstate", IJR_RealEstate});
    portfolioMap.insert({"IJR_Utilities", IJR_Utilities});


    std::map<std::string, std::vector<std::string>>::iterator it;


// Second word
				std::regex regexPattern(R"(^\s*\w+\s+(\w+))");
				std::smatch matchResult;

				// Perform the regex search
    			if (std::regex_search(cmdLine, matchResult, regexPattern)) {
        			std::string secondWord = matchResult[1];  // matchResult[1] contains the second word

					std::vector<std::string> stocksToAnalyze;

					// Check if "all" - collect all unique stocks from all portfolios
					if (secondWord == "all") {
						std::cout << "Analyzing ALL portfolios..." << std::endl;
						std::set<std::string> allStocksSet;
						for (const auto& portfolio : portfolioMap) {
							for (const auto& stock : portfolio.second) {
								allStocksSet.insert(stock);
							}
						}
						stocksToAnalyze.assign(allStocksSet.begin(), allStocksSet.end());
						std::cout << "Total unique stocks: " << stocksToAnalyze.size() << std::endl;
					} else {
						it = portfolioMap.find(secondWord);
						stocksToAnalyze = it->second;
					}

					for(const auto& stockName : stocksToAnalyze) {
        				std::cout << stockName << '\n';
    				}

					// PROCESS DATA FOR EACH INSTANCE (DEVICE)
					m_invDevIfVec[0]->collectData(stocksToAnalyze);
					m_invDevIfVec[0]->calculateData();

					// NEW NEW NEW NEW NEW
					m_invDevIfVec[0]->_new_EvaluateCompanies();

					// PRINT
					m_invDevIfVec[0]->_new_sortCompaniesByTotalScore();
					m_invDevIfVec[0]->_new_printCompaniesByTotalScore();

					m_invDevIfVec[0]->_new_sortCompaniesByTotalScore();
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