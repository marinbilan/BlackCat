#include "MasterSrv.h"
#include "Factory.h"



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

	for(auto s : m_invDevIfVec)
	{
		// PROCESS DATA FOR EACH INSTANCE (DEVICE)
		s->collectData();
		s->calculateData();
		s->storeData();  // In DB


		// POSTPROCESS DATA FOR EACH INSTANCE (DEVICE)
		s->sortStocksByGrossProfit();
		s->sortStocksByGrossProfitForPrint();
		s->printStocksByGrossProfit();

		s->sortStocksByYearsToReturnDebt();
		s->printStocksByYearsToReturnDebt();

		s->sortStocksByPERatio();
		s->printStocksByPERatio();

		s->sortStocksByPriceToBookValue();
		s->printStocksByPriceToBookValue();

		//
		// FINAL VALUE SCORE
		s->sortStocksByFinalIncomeStatementScore();
		s->printStocksByFinalIncomeStatementScore();


		// DCF INTRINSIC VALUE
		s->sortStocksByZeroGrowthIntrinsicValue();
		s->printStocksByIntrinsicValue();






		// s->calculateTotalScore();



		// s->sortStocksByDebtPerSharePrice();
		// s->printStocksByDebtPerSharePercentage();

		// s->sortStocksBySharesIssuedGrowth();
		// s->printStocksBySharesIssuedGrowth();

		// Final Balane Sheet Score
		// s->sortStocksByFinalScore();
		// s->printStocksByFinalBalanceSheetScore();

		// ----------------
		// INCOME STATEMENT
		
		// s->sortStocksByAvrGrowth();
		// s->printStocksByAvgGrowth();

		

		
		
		// s->sortStocksByBalanceSheetAndIncomeStatementScore();
		// s->printStocksByBalanceAndIncomeStatement();

		
		
	}
}


void Services::MasterSrv::postInit()
{

}