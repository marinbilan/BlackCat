#include <math.h>

#include "InvDev0.h"

#include "HTTPSProxySrvIf.h"
#include "HTTPSProxySrv.h"

#include "Factory.h"





// NEW NEW NEW NEW 

const std::string& Services::Company::getCompanyTicker() const
{
	return m_companyTicker;
}


void Services::Company::setSummary(const std::string& companyName,
		double stockPrice,
		int64_t marketCap,
		double eps,
		double pe,
		int64_t numOfSharesOutstanding)
{
	m_companyName = companyName;
	m_stockPrice = stockPrice;
	m_marketCap = marketCap;
	m_eps = eps;
	m_pe = pe;
	m_numOfSharesOutstanding = numOfSharesOutstanding;
}


// INCOME STATEMENT
void Services::Company::setIncomeStatement(const Data& revenue,
	const Data& netIncomeRatio,
	const Data& netIncome)
{
	m_revenueVec.push_back(revenue);
	m_netIncomeRatioVec.push_back(netIncomeRatio);
	m_netIncomeVec.push_back(netIncome);
}


void Services::Company::setIncomeStatementQuartal(const Data& revenueQuartal,
	const Data& netIncomeRatioQuartal,
	const Data& netIncomeQuartal)
{
	m_revenueQuartalVec.push_back(revenueQuartal);
	m_netIncomeRatioQuartalVec.push_back(netIncomeRatioQuartal);
	m_netIncomeQuartalVec.push_back(netIncomeQuartal);
}


std::vector<Services::Data>& Services::Company::getRevenueVec()
{
	return m_revenueVec;
}


std::vector<Services::Data>& Services::Company::getRevenueQuartalVec()
{
}


std::vector<Services::Data>& Services::Company::getNetIncomeRatioVec()
{
	return m_netIncomeRatioVec;
}


std::vector<Services::Data>& Services::Company::getNetIncomeVec()
{
	return m_netIncomeVec;
}


// BALANCE SHEET
void Services::Company::setBalanceSheet(const Data& cashAndCashEquivalents,
	const Data& totalStockholdersEquity,
	const Data& totalDebt)
{
	m_cashAndCashEquivalentsVec.push_back(cashAndCashEquivalents);
	m_totalStockholdersEquityVec.push_back(totalStockholdersEquity);
	m_totalDebtVec.push_back(totalDebt);
}


void Services::Company::setBalanceSheetQuartal(const Data& cashAndCashEquivalentsQuartal,
	const Data& totalStockholdersEquityQuartal,
	const Data& totalDebtQuartal)
{
	m_cashAndCashEquivalentsQuartalVec.push_back(cashAndCashEquivalentsQuartal);
	m_totalStockholdersEquityQuartalVec.push_back(totalStockholdersEquityQuartal);
	m_totalDebtQuartalVec.push_back(totalDebtQuartal);	
}


std::vector<Services::Data>& Services::Company::getCashAndCashEqVec()
{
	return m_cashAndCashEquivalentsVec;
}


std::vector<Services::Data>& Services::Company::getStockholdersEquityVec()
{
	return m_totalStockholdersEquityVec;
}


std::vector<Services::Data>& Services::Company::getTotalDebtVec()
{
	return m_totalDebtVec;
}


// CASH FLOW STATEMENT
void Services::Company::setCashFlowStatement(const Data& freeCashFlow)
{
	m_freeCashFlowVec.push_back(freeCashFlow);
}


void Services::Company::setCashFlowStatementQuartal(const Data& freeCashFlowQuartal)
{
	std::cout << "---- Store FCF Q Period: " << freeCashFlowQuartal.m_period << " Value: " << freeCashFlowQuartal.m_value << '\n';
	m_freeCashFlowQuartalVec.push_back(freeCashFlowQuartal);
}


std::vector<Services::Data>& Services::Company::getFreeCashFlowVec()
{
	return m_freeCashFlowVec;
}





void Services::Company::setRatios(const double& currentRatio, const double& netProfitMargin, const double& returnOnEquity,
		const double& priceToBookRatio, const double& priceEarningsRatio, const double& priceFairValue, const double& dividendYield) 
{
	m_currentRatio = currentRatio;
	m_netProfitMargin = netProfitMargin;
	m_returnOnEquity = returnOnEquity;
	m_priceToBookRatio = priceToBookRatio;
	m_priceEarningsRatio = priceEarningsRatio;
	m_priceFairValue = priceFairValue;
	m_dividendYield = dividendYield;
}


void Services::Company::normalizeValues() 
{
	// INCOME STATEMENT
	// Revenue
	std::transform(m_revenueVec.begin(), m_revenueVec.end(), m_revenueVec.begin(), [&](Data& data) 
		{ 
			data.m_value /= static_cast<double>(m_numOfSharesOutstanding);
			return data;
		});

	// Revenue Quartal

	// Net Income 

	// Net Income Quartal
	std::transform(m_netIncomeQuartalVec.begin(), m_netIncomeQuartalVec.end(), m_netIncomeQuartalVec.begin(), [&](Data& data) 
		{ 
			data.m_value /= static_cast<double>(m_numOfSharesOutstanding);
			return data;
		});



	// BALANCE SHEET
	// 
	std::transform(m_cashAndCashEquivalentsVec.begin(), m_cashAndCashEquivalentsVec.end(), m_cashAndCashEquivalentsVec.begin(), [&](Data& data) 
		{ 
			data.m_value /= static_cast<double>(m_numOfSharesOutstanding);
			return data;
		});

	// 
	std::transform(m_cashAndCashEquivalentsQuartalVec.begin(), m_cashAndCashEquivalentsQuartalVec.end(), m_cashAndCashEquivalentsQuartalVec.begin(), [&](Data& data) 
		{ 
			data.m_value /= static_cast<double>(m_numOfSharesOutstanding);
			return data;
		});

	std::transform(m_totalDebtVec.begin(), m_totalDebtVec.end(), m_totalDebtVec.begin(), [&](Data& data) 
		{ 
			data.m_value /= static_cast<double>(m_numOfSharesOutstanding);
			return data;
		});

	// 
	std::transform(m_totalDebtQuartalVec.begin(), m_totalDebtQuartalVec.end(), m_totalDebtQuartalVec.begin(), [&](Data& data) 
		{ 
			data.m_value /= static_cast<double>(m_numOfSharesOutstanding);
			return data;
		});

	std::transform(m_totalStockholdersEquityVec.begin(), m_totalStockholdersEquityVec.end(), m_totalStockholdersEquityVec.begin(), [&](Data& data) 
		{ 
			data.m_value /= static_cast<double>(m_numOfSharesOutstanding);
			return data;
		});

	// 
	std::transform(m_totalStockholdersEquityQuartalVec.begin(), m_totalStockholdersEquityQuartalVec.end(), m_totalStockholdersEquityQuartalVec.begin(), [&](Data& data) 
		{ 
			data.m_value /= static_cast<double>(m_numOfSharesOutstanding);
			return data;
		});



	// CASH FLOW STATEMENT

	// Free Cash Flow (Year)
	std::transform(m_freeCashFlowVec.begin(), m_freeCashFlowVec.end(), m_freeCashFlowVec.begin(), [&](Data& data) 
		{ 
			data.m_value /= static_cast<double>(m_numOfSharesOutstanding);
			return data;
		});

	// Free Cash Flow (Quartal)
	std::transform(m_freeCashFlowQuartalVec.begin(), m_freeCashFlowQuartalVec.end(), m_freeCashFlowQuartalVec.begin(), [&](Data& data) 
		{ 
			data.m_value /= static_cast<double>(m_numOfSharesOutstanding);
			return data;
		});
}


void Services::Company::reverseVectors() 
{
	// INCOME STATEMENT
	std::reverse(m_revenueVec.begin(), m_revenueVec.end());


	// BALANCE SHEET

	// Cash and Equivalence
	std::reverse(m_cashAndCashEquivalentsVec.begin(), m_cashAndCashEquivalentsVec.end());
	std::reverse(m_cashAndCashEquivalentsQuartalVec.begin(), m_cashAndCashEquivalentsQuartalVec.end());

	// Total Debt
	std::reverse(m_totalDebtVec.begin(), m_totalDebtVec.end());
	std::reverse(m_totalDebtQuartalVec.begin(), m_totalDebtQuartalVec.end());

	// Shareholders Equity
	std::reverse(m_totalStockholdersEquityVec.begin(), m_totalStockholdersEquityVec.end());
	std::reverse(m_totalStockholdersEquityQuartalVec.begin(), m_totalStockholdersEquityQuartalVec.end());


	// CASH FLOW STATEMENT
	// FCF
	std::reverse(m_freeCashFlowVec.begin(), m_freeCashFlowVec.end());
	// FCF Quartal
	std::reverse(m_freeCashFlowQuartalVec.begin(), m_freeCashFlowQuartalVec.end());
}


void Services::Company::setCalculatedData(
		double revL,
		double revH,
		double revAvg,
		double revCAGR,

		double netIncRatioL,
		double netIncRatioH,
		double netIncRatioAvg,
		double netIncRatioCAGR,

		double netIncL,
		double netIncH,
		double netIncAvg,
		double netIncCAGR,

		double cashL,
		double cashH,
		double cashAvg,
		double cashCAGR,

		double shEqL,
		double shEqH,
		double shEqAvg,
		double shEqCAGR,

		double totDebtL,
		double totDebtH,
		double totDebtAvg,
		double totDebtCAGR,

		double fcfL,
		double fcfH,
		double fcfAvg,
		double fcfCAGR) 
{
	m_revL = revL;
	m_revH = revH;
	m_revAvg = revAvg;
	m_revCAGR = revCAGR;

	std::cout << "Set value: " << m_revL << " " << m_revH << " " << m_revAvg << " " << m_revCAGR << '\n';

	m_netIncRatioL = netIncRatioL;
	m_netIncRatioH = netIncRatioH;
	m_netIncRatioAvg = netIncRatioAvg;
	m_netIncRatioCAGR = netIncCAGR;

	m_netIncL = netIncL;
	m_netIncH = netIncH;
	m_netIncAvg = netIncAvg;
	m_netIncCAGR = netIncCAGR;

	m_cashL = cashL;
	m_cashH = cashH;
	m_cashAvg = cashAvg;
	m_cashCAGR = cashCAGR;

	m_shEqL = shEqL;
	m_shEqH = shEqH;
	m_shEqAvg = shEqAvg;
	m_shEqCAGR = shEqCAGR;

	m_totDebtL = totDebtL;
	m_totDebtH = totDebtH;
	m_totDebtAvg = totDebtAvg;
	m_totDebtCAGR = totDebtCAGR;

	m_fcfL = fcfL;
	m_fcfH = fcfH;
	m_fcfAvg = fcfAvg;
	m_fcfCAGR = fcfCAGR;	
}


void Services::Company::setCalculatedValueParams(int PE_Mark, int PB_Mark, int ROE_Mark, int NetMargin_Mark, int DebtToEquity_Mark, int CurrentRatio_Mark, int YrsToRetDebtFCF_Mark,
		int TotalMark, double DebtToEquity_calc, double YrsToRetDebtFCF_calc)
{
	m_PE_Mark = PE_Mark;
	m_PB_Mark = PB_Mark;
	m_ROE_Mark = ROE_Mark;
	m_NetMargin_Mark = NetMargin_Mark;
	m_DebtToEquity_Mark = DebtToEquity_Mark;
	m_CurrentRatio_Mark = CurrentRatio_Mark;
	m_YrsToRetDebtFCF_Mark = YrsToRetDebtFCF_Mark;

	m_TotalMark = TotalMark;

	m_DebtToEquity_calc = DebtToEquity_calc;
	m_YrsToRetDebtFCF_calc = YrsToRetDebtFCF_calc;
}


void Services::Company::setDCFCalculatedValues(double peGrowthRate, double peGrowthPrice, double peGrowthError,
		double zeroGrowthRate, double zeroGrowthPrice, double zeroGrowthRateError, double grahmPrice)
{
	m_peGrowthRate = peGrowthRate;
	m_peGrowthPrice = peGrowthPrice;
	m_peGrowthError = peGrowthError;

	m_zeroGrowthRate = zeroGrowthRate;
	m_zeroGrowthPrice = zeroGrowthPrice;
	m_zeroGrowthRateError = zeroGrowthRateError;

	m_grahmPrice = grahmPrice;

}


void Services::Company::printCompanyInfo()
{
	std::cout << "" << '\n';
	std::cout << "" << '\n';
	std::cout << "|||||||||||" << '\n';
	std::cout << "[ SUMMARY ]" << '\n';
	std::cout << "[" << m_companyTicker << "] " << m_companyName << '\n';
	std::cout << "Price: " << m_stockPrice << " $ [Calc: " << "... $]" << " [Market Cap: " << m_marketCap << " $]" << '\n';
	std::cout << "PE: " << m_pe << '\n';
	std::cout << "EPS: " << m_eps << '\n';
	std::cout << "Shares Outstanding: " << m_numOfSharesOutstanding << '\n' << '\n';

	std::cout << "P/E:             [" << m_PE_Mark << "] "<< m_pe << '\n';
	std::cout << "P/B:             [" << m_PB_Mark << "] "<< m_priceToBookRatio << '\n';

	std::cout << "RoE:             [" << m_ROE_Mark << "] "<< m_returnOnEquity << '\n';
	std::cout << "Net Margin:      [" << m_NetMargin_Mark << "] "<< m_netProfitMargin << '\n';
	std::cout << "D/E:             [" << m_DebtToEquity_Mark << "] "<< m_DebtToEquity_calc << '\n';

	std::cout << "Current Ratio:   [" << m_CurrentRatio_Mark << "] "<< m_currentRatio << '\n';
	std::cout << "YrsToRetDebtFCF: [" << m_YrsToRetDebtFCF_Mark << "] "<< m_YrsToRetDebtFCF_calc << '\n';
	std::cout << "TOTAL MARK: " << m_TotalMark << '\n';

	std::cout << '\n';

	std::cout << "Revenue [ ";
	for(auto s : m_revenueVec) 
	{
		// std::cout << s.m_period << " " << s.m_value;
		std::cout << s.m_value << " ";
	}
	std::cout << "] [Lin: " << m_revH << " Avg: " << m_revAvg << " CAGR: " << m_revCAGR << "]" << '\n';

	/*
	for(auto s : m_revenueQuartalVec) 
	{
		std::cout << s.m_period << " " << s.m_value << '\n';
	}
	*/

	std::cout << "Net Income Ratio [ ";
	for(auto s : m_netIncomeRatioVec) 
	{
		// std::cout << s.m_period << " " << s.m_value;
		std::cout << s.m_value << " ";
	}
	std::cout << "] [Lin: " << m_netIncRatioH << " Avg: " << m_netIncRatioAvg << " CAGR: " << m_netIncRatioCAGR << "]" << '\n';

	/*
	std::cout << " - Net Income Ratio -" << '\n';
	for(auto s : m_netIncomeRatioVec) 
	{
		std::cout << s.m_period << " " << s.m_value << '\n';
	}

	for(auto s : m_netIncomeRatioQuartalVec) 
	{
		std::cout << s.m_period << " " << s.m_value << '\n';
	}
	*/

	std::cout << "Net Income [ ";
	for(auto s : m_netIncomeVec) 
	{
		std::cout << s.m_value << " ";
	}
	std::cout << "] [Lin: " << m_netIncH << " Avg: " << m_netIncAvg << " CAGR: " << m_netIncCAGR << "]" << '\n';
	

	// ---- BALANCE SHEET ----

	// Cash
	std::cout << '\n';

	std::cout << "[Cash and Eq]" << '\n';
	for(auto s : m_cashAndCashEquivalentsVec) 
	{
		std::cout << " [" << s.m_period << "] " << s.m_value;
	}
	std::cout << " ........ [Lin: " << m_totDebtH << " Avg: " << m_totDebtAvg << " CAGR: " << m_cashCAGR << "]" << '\n';

	int lastFour = std::min(4, (int)m_cashAndCashEquivalentsQuartalVec.size()); // Print last four values

	for(auto it = m_cashAndCashEquivalentsQuartalVec.rbegin(); it != m_cashAndCashEquivalentsQuartalVec.rbegin() + lastFour; ++it) 
	{
		std::cout << " [" << (*it).m_period << "] " << (*it).m_value;
	}


	// Debt
	std::cout << '\n';

	std::cout << "[Total Debt]" << '\n';
	for(auto s : m_totalDebtVec) 
	{
		std::cout << " [" << s.m_period << "] " << s.m_value;
	}
	std::cout << " ........ [Lin: " << m_totDebtH << " Avg: " << m_totDebtAvg << " CAGR: " << m_totDebtCAGR << "]" << '\n';

	lastFour = std::min(4, (int)m_totalDebtQuartalVec.size()); // Print last four values

	for(auto it = m_totalDebtQuartalVec.rbegin(); it != m_totalDebtQuartalVec.rbegin() + lastFour; ++it) 
	{
		std::cout << " [" << (*it).m_period << "] " << (*it).m_value;
	}


	// Shareholder Equity
	std::cout << '\n';

	std::cout << "[Shareholder's Equity]" << '\n';
	for(auto s : m_totalStockholdersEquityVec) 
	{
		std::cout << " [" << s.m_period << "] " << s.m_value;
	}
	std::cout << " ........ [Lin: " << m_shEqH << " Avg: " << m_shEqAvg << " CAGR: " << m_shEqCAGR << "]" << '\n';

	lastFour = std::min(4, (int)m_totalStockholdersEquityQuartalVec.size()); // Print last four values

	for(auto it = m_totalStockholdersEquityQuartalVec.rbegin(); it != m_totalStockholdersEquityQuartalVec.rbegin() + lastFour; ++it) 
	{
		std::cout << " [" << (*it).m_period << "] " << (*it).m_value;
	}



	// ---- CASH FLOW STATEMENT ----
	std::cout << '\n';

	// Free Cash Flow
	std::cout << "FCF:    ";
	for(auto s : m_freeCashFlowVec) 
	{
		std::cout << " [" << s.m_period << "] " << s.m_value;
	}
	std::cout << " ........ [Lin: " << m_fcfH << " Avg: " << m_fcfAvg << " CAGR: " << m_fcfCAGR << "]" << '\n';

	std::cout << "FCF Q:  ";	
	lastFour = std::min(4, (int)m_freeCashFlowQuartalVec.size()); // Print last four values

	for(auto it = m_freeCashFlowQuartalVec.rbegin(); it != m_freeCashFlowQuartalVec.rbegin() + lastFour; ++it) 
	{
		std::cout << " [" << (*it).m_period << "] " << (*it).m_value;
	}

	//
	std::cout << '\n';
	std::cout << '\n';
	std::cout << "FCF = " << m_fcfH << " $" << '\n';
	std::cout << "[DCF PE Gr      = " << m_peGrowthPrice << " $]" << " [PE Gr rate  = " << m_peGrowthRate << "]" << " [DCF Error = " << m_peGrowthError << "]" << '\n';
	std::cout << "[DCF 0  Gr      = " << m_zeroGrowthPrice << " $] " << "[DCF Error = " << m_zeroGrowthRateError << "]" << '\n';
	std::cout << "[Graham         = " << m_grahmPrice << " $]" << '\n';
	std::cout << "-----------------" << '\n';
	std::cout << "[Price          = " << m_stockPrice << " $]" << '\n';
	
	std::cout << "|||||||||||" << '\n';
	std::cout << '\n';
	std::cout << '\n';
}

// NEW NEW NEW NEW 








Services::InvDev::InvDev(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
	std::string trace = "[MB][MasterSrv][InvDev] Services::InvDev constructor";
	Common::Factory::Factory::getInstance().getClientServerSrv()->TRACE(trace);

	trace = "[MB][MasterSrv][InvDev] Services::InvDev name: " + m_name;
	Common::Factory::Factory::getInstance().getClientServerSrv()->TRACE(trace);
}


Services::InvDev::~InvDev()
{
	std::cout << "Services::InvDev destructor called!" << '\n';
}


const std::string& Services::InvDev::getName()
{

	return m_name;
}


void Services::InvDev::preInit()
{

}


void Services::InvDev::postInit()
{

}


void Services::InvDev::collectData(const std::vector<std::string>& portfolio)
{
	std::cout << "[MB] Services::InvDev collectData ..." << '\n';

	// Clean up vector before new analysis
	m_stocksVec.clear();
	m_companyVec.clear();

	// foreach stock ...

	for(const auto& stockName : portfolio)
	{
		Stock stock(stockName);


		// Create HTTPSProxy via Factory and get from Container
		// FACTORY.getLog()->LOGFILE(LOG "Create HTTPSProxy via Factory and get from Container");
		std::shared_ptr<Services::HTTPSProxySrvIf> objHTTPSProxy = std::make_shared<Services::HTTPSProxySrv>("Test", "Test");



		// NEW NEW NEW NEW NEW NEW 
		
		Company company(stockName);
		objHTTPSProxy->_new_GetDataFromServer(company);

		// Check that data exists - Check this in details
		if(company.getRevenueVec().size() > 0) 
		{
			company.reverseVectors();

			// company.printCompanyInfo();
			company.normalizeValues();
			// company.printCompanyInfo();

			m_companyVec.push_back(company);			
		}
		
		// NEW NEW NEW NEW NEW NEW


		objHTTPSProxy->_getFromSummary(stock);
		objHTTPSProxy->_getRatios(stock);
		objHTTPSProxy->_getDCF(stock);

		objHTTPSProxy->_getFromIncomeStatement(stock);
		objHTTPSProxy->_getFromBalanceSheet(stock);
		objHTTPSProxy->_getFromCashFlowStatement(stock);


		// Prepare vectors
		stock.reverseVectors();


		if(stock.getRevenueVec().size() &&
		stock.getGrossProfitVec().size() &&
		stock.getIncomeVec().size() &&
		stock.getBookValueVec().size() &&
		stock.getTotalDebtVec().size() &&
		stock.getShareIssuedVec().size() &&
		stock.getFreeCashFlowVec().size()) {
			FACTORY.getLog()->LOGFILE(LOG "Store " + stock.getName());

			m_stocksVec.push_back(stock);
		}

	}
}


void Services::InvDev::calculateData()
{
	// Foreach stock calculate data
	for(auto& s : m_stocksVec)
	{
		calculateGrowth(s);
	}

	std::cout << "---------------------------------------CALC DATA -------------------------------------------------------" << '\n';
	// NEW NEW NEW NEW NEW NEW NEW NEW
	
	for(auto& s : m_companyVec)
	{
		// Check issue on Lindsal Train - cored dump
		std::cout << "---------------------------------------CALC COMPANY -------------------------------------------------------" << '\n';
		std::cout << "Stock: " << s.getCompanyTicker() << '\n';
		_new_calculateData(s);
		_new_calculateValueParams(s);
		_new_calculatePrice(s);

		s.printCompanyInfo();
	}
	
	// NEW NEW NEW NEW NEW NEW NEW NEW


}


void Services::InvDev::_new_calculateData(Company& company)
{
	// ---- INCOME STATEMENT ----
	// REVENUE
	double revL = 0.0;
	double revH = 0.0;
	double revAvg = 0.0;
	double revCAGR = 0.0;

	_new_calcParameters(company.getRevenueVec(), revL, revH, revAvg, revCAGR);

	// NET INCOME RATIO (Net Profit Margin)
	double netIncRatioL = 0.0;
	double netIncRatioH = 0.0;
	double netIncRatioAvg = 0.0;
	double netIncRatioCAGR = 0.0;
	_new_calcParameters(company.getNetIncomeRatioVec(), netIncRatioL, netIncRatioH, netIncRatioAvg, netIncRatioCAGR);

	// NET INCOME
	double netIncL = 0.0;
	double netIncH = 0.0;
	double netIncAvg = 0.0;
	double netIncCAGR = 0.0;
	_new_calcParameters(company.getNetIncomeVec(), netIncL, netIncH, netIncAvg, netIncCAGR);

	// ---- BALANCE SHEET ----

	// CASH AND CASH EQUIVALENTS
	double cashL = 0.0;
	double cashH = 0.0;
	double cashAvg = 0.0;
	double cashCAGR = 0.0;
	_new_calcParameters(company.getCashAndCashEqVec(), cashL, cashH, cashAvg, cashCAGR);

	// TOTAL SHAREHOLDERS EQUITY
	double shEqL = 0.0;
	double shEqH = 0.0;
	double shEqAvg = 0.0;
	double shEqCAGR = 0.0;
	_new_calcParameters(company.getStockholdersEquityVec(), shEqL, shEqH, shEqAvg, shEqCAGR);

	// TOTAL DEBT
	double totDebtL = 0.0;
	double totDebtH = 0.0;
	double totDebtAvg = 0.0;
	double totDebtCAGR = 0.0;
	_new_calcParameters(company.getTotalDebtVec(), totDebtL, totDebtH, totDebtAvg, totDebtCAGR);

	// ---- CASH FLOW STATEMENT ----

	// FREE CASH FLOW
	double fcfL = 0.0;
	double fcfH = 0.0;
	double fcfAvg = 0.0;
	double fcfCAGR = 0.0;
	_new_calcParameters(company.getFreeCashFlowVec(), fcfL, fcfH, fcfAvg, fcfCAGR);


	company.setCalculatedData(
		revL, revH, revAvg, revCAGR,
		netIncRatioL, netIncRatioH, netIncRatioAvg, netIncRatioCAGR,
		netIncL, netIncH, netIncAvg, netIncCAGR,
		cashL, cashH, cashAvg, cashCAGR,
		shEqL, shEqH, shEqAvg, shEqCAGR,
		totDebtL, totDebtH, totDebtAvg, totDebtCAGR,
		fcfL, fcfH, fcfAvg, fcfCAGR
		);

}


void Services::InvDev::_new_calcParameters(std::vector<Data>& dataVec, double& lowVal, double& highVal, double& avgValue, double& CAGR)
{
	double a = 0.0;
	double b = 0.0;

	// [1] Calc Linear Regression High Low values
	_new_calcLinearRegressCoeffs(dataVec, a, b);
	_new_calcLinearValues(dataVec, a, b, lowVal, highVal);

	// [2] Avg value
    double totValue = std::accumulate(dataVec.begin(), dataVec.end(), 0.0, 
    	[](double sum, const Data& d) {
         	return sum + d.m_value;
        });
    avgValue = totValue / dataVec.size();

	// [3] Last value - For print, already have (last vector value)

    // [4] Growth k value
    CAGR = _new_CAGR(dataVec, lowVal, highVal);
}


/*
	LINEAR FUNCTION: y = a + b * x
	std::vector<double> y = {265595, 260174, 274515, 365817};
	std::vector<double> x = {1,      2,      3,      4};

    double year5 = a + b * 5;  //  5th year
    double year6 = a + b * 6;  //  6th year
*/
void Services::InvDev::_new_calcLinearRegressCoeffs(const std::vector<Data>& dataVec, double& a, double& b)
{
	std::vector<double> rangeYrs(dataVec.size());
	std::iota(rangeYrs.begin(), rangeYrs.end(), 1); // 1, 2, 3, 4 ...

    double sumX = 0;
    double sumX2 = 0; 
    double sumY = 0; 
    double sumXY = 0;

    for(int i = 0; i <= rangeYrs.size(); i++)
    {
        sumX =  sumX + rangeYrs[i];
        sumX2 = sumX2 + rangeYrs[i] * rangeYrs[i];

        sumY =  sumY + dataVec[i].m_value;
        sumXY = sumXY + rangeYrs[i] * dataVec[i].m_value;
    }

    int n = rangeYrs.size(); // Number of points

    b = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    a = (sumY - b * sumX) / n;
}


void Services::InvDev::_new_calcLinearValues(const std::vector<Data>& dataVec, double& a, double& b, double& lowValue, double& highValue)
{
	/*
	[ val0, val1, val2, ... valN ]
	[ 1     2     3     ... n]
	*/
	lowValue  = a + b * 1;
	highValue = a + b * static_cast<double>(dataVec.size());
}

static 	std::map<double, int> PEranges = { 
		{10.0, 5},    // Range: [0, 10)   gives mark 5 
		{20.0, 4},    // Range: [10, 20)  gives mark 4 
		{30.0, 3},    // Range: [20, 30)  gives mark 3  
		{40.0, 2},    // Range: [30, 40)  gives mark 2  
		{10000.0, 1}, // Range: [40, inf) gives mark 1  
	};

void Services::InvDev::_new_calculateValueParams(Company& company)
{
	std::cout << "Calculate value parameters ..." << '\n';

	/*
	Valuation Metrics
	- PE - (last 12 months) - This value is ok (Ratio API)
	- PB - (last 12 months) - Check value because this value is for last year

	Profitable Metrics
	- ROE - Net Income / Sh Equity - (last 12 months)
	- Net Margin                   - (last 12 months)

	Financial Health Metrics
	- D/E = Total Debt / Sh Equity - (last 12 months)
	- Current Ratio = Current Assets / Current Liabilities
	- Years to return debt with FCF
	*/


	// [ Valuation Metrics ]
	// PE
	auto it0 = PEranges.upper_bound(company.m_pe); 
	int PE_Mark = 0;
	if (it0 != PEranges.end()) 
	{ 
		std::cout << "PE Mark: " << it0->second << "\n";
		PE_Mark = it0->second; 
	} else 
	{ 
		std::cout << "Value out of defined ranges. Set mark to 0.\n"; 
	}

	// PB
	std::map<double, int> PBranges = { 
		{1.0, 5},     // Range: [0, 1)  gives mark 5 - Undervalued or Distressed - The stock trades below its book value, meaning the market values it less than its net assets
		{2.0, 4},     // Range: [1, 2)  gives mark 4 - Fairly Valued - Typically seen as a reasonable valuation range
		{3.0, 3},     // Range: [2, 3)  gives mark 3 - Growth or Overvaluation? - Investors are willing to pay a premium over the company's book value
		{7.0, 2},     // Range: [3, 7)  gives mark 2
		{10000.0, 1}, // Range:[7, inf) gives mark 1 
	};  

	auto it1 = PBranges.upper_bound(company.m_priceToBookRatio); 
	int PB_Mark = 0;
	std::cout << " m_priceToBookRatio: " << company.m_priceToBookRatio << "\n";
	if (it1 != PBranges.end()) 
	{ 
		std::cout << " PBMark: " << it1->second << "\n"; 
		PB_Mark = it1->second;
	} else 
	{ 
		std::cout << "Value out of defined ranges. Set mark to 0.\n"; 
	}




	// Profitable Metrics
	// ROE
	std::map<double, int> ROEranges = { 
		{0.1, 1}, // Range: [0, 0.1)    gives mark 1 - Could indicate poor management, high debt, or an industry with low margins
		{0.2, 2}, // Range: [0.1, 0.2)  gives mark 2 - Many solid, well-managed companies fall in this range
		{0.3, 3}, // Range: [0.2, 0.3)  gives mark 3 - Often seen in quality companies with competitive advantages (e.g., brand strength, pricing power) 
		{1,   5},  // Range: [0.3, inf) gives mark 5 - Could indicate an exceptional business (e.g., asset-light, strong pricing power)
	}; 	

	auto it2 = ROEranges.upper_bound(company.m_returnOnEquity); 
	int ROE_Mark = 0;
	std::cout << " m_returnOnEquity: " << company.m_returnOnEquity << "\n";
	if (it2 != ROEranges.end()) 
	{ 
		std::cout << " ROEMark: " << it2->second << "\n"; 
		ROE_Mark = it2->second;
	} else 
	{ 
		std::cout << "Value out of defined ranges. Set mark to 0.\n"; 
	}

	// Net Margin 
	std::map<double, int> NetMarginranges = { 
		{0.05, 1}, // Range: [0, 0.05)     gives mark 1 - Thin profitability—common in industries with high costs or intense competition (retail, airlines, grocery stores)
		{0.15, 2}, // Range: [0.05, 0.15)  gives mark 2 - Healthy and sustainable profitability for most businesses (financials, industrials, and consumer goods)
		{0.30, 3}, // Range: [0.15, 0.30)  gives mark 3 - Indicates strong pricing power, cost efficiency, or a high-value product/service (tech, pharmaceuticals, software, luxury brands)
		{1,   5},  // Range: [0.3, inf)    gives mark 5 - Software, biotech, and monopolistic businesses with minimal production costs (Can signal a competitive moat,)
	};

	auto it3 = NetMarginranges.upper_bound(company.m_netProfitMargin); 
	int NetMargin_Mark = 0;
	std::cout << " m_netProfitMargin: " << company.m_netProfitMargin << "\n";
	if (it3 != NetMarginranges.end()) 
	{ 
		std::cout << " ROEMark: " << it3->second << "\n"; 
		NetMargin_Mark = it3->second;
	} else 
	{ 
		std::cout << "Value out of defined ranges. Set mark to 0.\n"; 
	}




	// Financial Health Metrics
	// D/E = Total Debt / Sh Equity
	double lastYearDebt = company.m_totalDebtVec.back().m_value;
	double lastYearShEquity = company.m_totalStockholdersEquityVec.back().m_value;
	double DebtToEquity = lastYearDebt / lastYearShEquity;

	std::map<double, int> DEranges = { 
		{0.3, 5},     // Range: [0, 0.3)   gives mark 5 - Very low debt	Conservative, strong balance sheet ✅
		{0.6, 4},     // Range: [0.3, 0.6) gives mark 4 - Moderate debt	Generally acceptable for value investing 👍
		{1.0, 3},     // Range: [0.6, 1.0) gives mark 3 - Higher debt	Requires further scrutiny
		{2.0, 2},     // Range: [1.0, 2.0) gives mark 2 - Acceptable only for capital-intensive industries (e.g., utilities, telecom)
		{10000.0, 1}, // Range: [2.0, inf) gives mark 1 - Highly leveraged	High financial risk, avoid unless justified
	};

	auto it4 = DEranges.upper_bound(DebtToEquity); 
	int DebtToEquity_Mark = 0;
	std::cout << " DebtToEquity: " << DebtToEquity << "\n";
	if (it4 != DEranges.end()) 
	{ 
		std::cout << " DEMark: " << it4->second << "\n"; 
		DebtToEquity_Mark = it4->second;
	} else 
	{ 
		std::cout << "Value out of defined ranges. Set mark to 0.\n"; 
	}

	// Current Ratio = Current Assets / Current Liabilities
	std::map<double, int> CurrentRatioranges = { 
		{1.0, 1},   // Range: [0, 1.00)    gives mark 1 - Possible liquidity issues - High risk, needs further analysis
		{1.5, 2},   // Range: [1.00, 1.50) gives mark 2 - Barely covering liabilities - Watch cash flow closely
		{2.5, 5},   // Range: [1.5, 2.5)   gives mark 5 - Healthy financial position - ✅ Ideal range for value investing
		{10000, 4}, // Range: [2.5, inf)   gives mark 4 - Excess assets or inefficiency - Might not be using capital effectivelys
	};

	auto it5 = CurrentRatioranges.upper_bound(company.m_currentRatio); 
	int CurrentRatio_Mark = 0;
	std::cout << " CurrentRatio: " << company.m_currentRatio << "\n";
	if (it5 != CurrentRatioranges.end()) 
	{ 
		std::cout << " CurrentRatioMark: " << it5->second << "\n";
		CurrentRatio_Mark = it5->second; 
	} else 
	{ 
		std::cout << "Value out of defined ranges. Set mark to 0.\n"; 
	}


	// Years to return debt with FCF
	int n = company.m_freeCashFlowQuartalVec.size();

	for(auto s : company.m_freeCashFlowQuartalVec) 
	{
		std::cout << "---- CALC FCF Q Period: " << s.m_period << " Value: " << s.m_value << '\n';
	}

	int lastFourStart = std::max(0, n - 4);  // Ensure we don't go out of bounds

	double sumLastFour = std::accumulate(
	        company.m_freeCashFlowQuartalVec.begin() + lastFourStart, 
	        company.m_freeCashFlowQuartalVec.end(), 
	        0.0, 
	        [](double sum, const Data& d) { 
	        	std::cout << " ----> Q: " << d.m_value << '\n';
	        	return sum + d.m_value; }
	    );

	std::cout << "----> lastYearDebt: " << lastYearDebt << '\n';
	double YrsToReturnDebtWithFCF = lastYearDebt / sumLastFour; // lastYearDebt already get above

	std::map<double, int> YRStoRetDbtranges = { 
		{1.0, 5},     // Very strong financial position, debt can be cleared in a year.
		{3.0, 4},     // Manageable debt level, reasonable for most industries.
		{5.0, 3},     // Debt is significant, requires strong and stable FCF.
		{10.0, 2},    // Company might struggle to repay debt without major growth.
		{10000.0, 1}, // Excessive debt, FCF is too low—potential red flag.
	};

	auto it6 = YRStoRetDbtranges.upper_bound(YrsToReturnDebtWithFCF); 
	int YrsToRetDebtFCF_Mark = 0;
	std::cout << " YrsToReturnDebtWithFCF: " << YrsToReturnDebtWithFCF << "\n";
	if (it6 != YRStoRetDbtranges.end()) 
	{ 
		YrsToRetDebtFCF_Mark = it6->second;
	} else 
	{ 
		std::cout << "Value out of defined ranges. Set mark to 0.\n"; 
	}

	int totalMark = PE_Mark + PB_Mark + ROE_Mark + NetMargin_Mark + DebtToEquity_Mark + CurrentRatio_Mark + YrsToRetDebtFCF_Mark;

	// Set Marks
	company.setCalculatedValueParams(PE_Mark, PB_Mark, ROE_Mark, NetMargin_Mark, DebtToEquity_Mark, CurrentRatio_Mark, YrsToRetDebtFCF_Mark, totalMark, DebtToEquity, YrsToReturnDebtWithFCF);


}



/*
To compute the growth rate in percentage for a set of values, we assume the growth follows a consistent rate (e.g., exponential or linear). 
In this case, we’ll calculate the compound annual growth rate (CAGR) since we have a set of sequential values over time.

Formula:

x = 1 / (n - 1)
CAGR = (Last Value / First Value)^x - 1 

Last Value is the final value in the dataset
First Value is the initial value in the dataset.
n is the number of values in the dataset.

Ex:
First Value: 15.7937
Last Value:  52.4095
Number of values (𝑛): 7

Calc:
CAGR ≈ 0.2119 or 21.19 %

*/
double Services::InvDev::_new_CAGR(std::vector<Data>& vec, const double& first_value, const double& last_value)
{

	int n = vec.size();

	double CAGR = pow(last_value / first_value, 1.0 / (n - 1)) - 1;

	return CAGR;
}


	// Graham Formula
	/*
	EPS = Earnings per share (TTM)
	g = Expected growth rate (5-10 years)
	8.5 = P/E ratio for a no-growth company
	4.4 = Average risk-free yield (historical AAA corporate bond yield when Graham wrote this)
	Y = Current AAA corporate bond yield (to adjust for interest rates)

	IV = [ EPS×(8.5+2g)×4.4 ] / Y
 
	[ META CASE ]

	EPS (TTM) = $14.87 (as of latest report)
	Growth rate (g) = 15% (expected)
	Current AAA bond yield (Y) = 5.0%

	Intrinsic Value = [ 14.87×(8.5+2×15)×4.4 ] / 5.0 = 503.7 $
	*/
void Services::InvDev::_new_calculatePrice(Company& company)
{
	// If PE ratio growth is higher than 0.095 (9.5%) limit company growth to 0.095% (9.5%) 
	double peGrowthError = 0.0;
	double peGrowthRate = 1 / company.m_pe;
	double peGrowthPrice = calculateDCF(peGrowthRate, company.m_fcfH, peGrowthError);

	//
	double zeroGrowthError = 0.0;
	double zeroGrowthRate = 0.0;
	double zeroGrowthPrice = calculateDCF(zeroGrowthRate, company.m_fcfH, zeroGrowthError);


	// Graham Formula
	double referenceYield = 4.4; // Historical AAA bond yield used by Graham
	double bondYield = 5.0; // Current AAA corporate bond yield (%)
	double peGrowthRatePercentage = peGrowthRate * 100;
	double grahamPrice = (company.m_eps * (8.5 + 2 * peGrowthRatePercentage) * referenceYield) / bondYield;

	company.setDCFCalculatedValues(peGrowthRate, peGrowthPrice, peGrowthError, zeroGrowthRate, zeroGrowthPrice, zeroGrowthError, grahamPrice);
}
// NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW NEW 












void Services::InvDev::calculateGrowth(Stock& stock)
{
	// ---- [INCOME STATEMENT] ----
	//
	// [1] Revenue k (Growth)
	double a;
	double b;
	// [1] Revenue k (Growth)
	calcLinearRegressCoeffs(stock.getRevenueVec(), a, b);
	double revenueGrowth = calculateK(a, b, stock.getRevenueVec());

	// TODO: Gross Margin k

	// [2] Net Income k (Growth)
	calcLinearRegressCoeffs(stock.getIncomeVec(), a, b);
	double netIncomeGrowth = calculateK(a, b, stock.getIncomeVec());

	// ---- [FREE CASH FLOW STATEMENT] ----
	//
	// [3] FCF k (Growth)
	calcLinearRegressCoeffs(stock.getFreeCashFlowVec(), a, b);

	// Calculate FCF at 4th year (for DCF analysis)
	double FCF4thYear = a + b * stock.getFreeCashFlowVec().size();
	double FCF4thYearPerShare = FCF4thYear / stock.getShareIssuedVec().back();

	double FCFGrowth = calculateK(a, b, stock.getFreeCashFlowVec());

	// [4] Average (Growth)
	double avgGrowth = (revenueGrowth + netIncomeGrowth + FCFGrowth) / 3;
	FACTORY.getLog()->LOGFILE(LOG "AvgGrowth " + stock.getName() + ": " + std::to_string(avgGrowth));



	// [] Gross Profit Margin Ratio
	stock.m_grossProfitRatio = stock.getGrossProfitVec().back() / stock.getRevenueVec().back();

	// Calc revenue slope value
	calcLinearRegressCoeffs(stock.getRevenueVec(), a, b);
	double revenueSlopeVal = a + b * stock.getRevenueVec().size();

	calcLinearRegressCoeffs(stock.getGrossProfitVec(), a, b);
	double grossProfitSlopeVal = a + b * stock.getRevenueVec().size();

	stock.m_grossProfitRatioSlopeVal = grossProfitSlopeVal / revenueSlopeVal;



	// ----------------
	// [5] Yahoo PE Ratio
	double peRatio = stock.getPERatio();

	// [6] Growth from current point
	double peGrowth = 1 / peRatio;

	// [7] Calculated PE ratio
	// Market Cap (in thousands) = Stock Price * Num of Stocks
	double marketCap = stock.getStockPrice() * stock.getShareIssuedVec().back();
	// Average Net Income (in thousands) last N years
	double avgNetIncome = std::accumulate(stock.getIncomeVec().begin(), stock.getIncomeVec().end(), 0.0) / stock.getIncomeVec().size();	

	// If avg net income is less then zero 
	// double calculatedPE = marketCap / avgNetIncome;
	double calculatedPE = (avgNetIncome > 0.0) ? marketCap / avgNetIncome : 0.1;

	// [8] Calculate average FCF (per share)
	double avgFCF = std::accumulate(stock.getFreeCashFlowVec().begin(), stock.getFreeCashFlowVec().end(), 0.0) / stock.getFreeCashFlowVec().size();
	// avgFCF = (avgFCF > 0.0) ? avgFCF : 0.01;

	double avgFCFPerShare = avgFCF / stock.getShareIssuedVec().back();

	//
	stock.m_FCFToPriceRatio = avgFCFPerShare / stock.getStockPrice();


	// ---- DCF ----
	// Get this value for particular industry
	double desiredReturn = 0.15;

	// ----
	double upperGrowthError = 0.0;
	double DCFUpperValue = calculateDCF(avgGrowth, avgFCFPerShare, upperGrowthError);
	// ----	
	// If PE ratio growth is higher than 0.095 (9.5%) limit company growth to 0.095% (9.5%) 
	double upperPEGrowthError = 0.0;
	double upperPEGrowth = (peGrowth >= 0.095) ? 0.095 : peGrowth;
	double DCFPEValue = calculateDCF(upperPEGrowth, avgFCFPerShare, upperPEGrowthError);
	// ----
	double zeroGrowthError = 0.0;
	double zeroGrowth = 0.0;
	double DCFzeroValue = calculateDCF(zeroGrowth, avgFCFPerShare, zeroGrowthError);
	// ----


	stock.setIncomeAndFCFStatements(
		revenueGrowth, 
		netIncomeGrowth, 
		FCFGrowth, 
		avgGrowth, 
		peGrowth, 
		calculatedPE, 
		avgFCFPerShare, 
		//
		desiredReturn, 
		// 
		DCFUpperValue, 
		upperGrowthError,
		DCFPEValue,
		upperPEGrowthError, 
		DCFzeroValue,
		zeroGrowthError,
		marketCap);


	// [BALANCE SHEET]
	//
	// [10] Calculate Book value (Equity) k
	calcLinearRegressCoeffs(stock.getBookValueVec(), a, b);
	double BookValueGrowth = calculateK(a, b, stock.getBookValueVec());

	// [11] P/B - (Market Cap)/(Book Value) - All values in thousands
	// TODO: Check this if book value is negative
	double lastYearBookVal = stock.getBookValueVec().back();
	double priceToBookVal = marketCap / lastYearBookVal;


	// [12] Total Debt per Shate - (Total (Last) Debt / Avrg FCF)
	double totalDebtPerShare = stock.getTotalDebtVec().back() / stock.getShareIssuedVec().back();

	// [13]
	double totalDebtPerSharePercentage = totalDebtPerShare / stock.getStockPrice();


	// [14] Years to Return Debt - (Total Debt / Avg FCF)
	double lastYearTotalDebt = stock.getTotalDebtVec().back();
	// If FCF is negative set 0.01
	avgFCF = (avgFCF > 0.0) ? avgFCF : 0.01;
	double yearsToReturnDebt = lastYearTotalDebt / avgFCF;

	// [15] Issued Shares k
	calcLinearRegressCoeffs(stock.getShareIssuedVec(), a, b);
	double sharesIssuedGrowth = calculateK(a, b, stock.getShareIssuedVec());


	// [16] Calculate Intrinsic Value
	double val = stock.getStockPrice() - stock.getDCFZeroGr();
	stock.setIntrValueZeroGrDiff(val);

	val = stock.getStockPrice() - stock.getDCFPEAvg();
	stock.setIntrValuePEGrDiff(val);

	val = stock.getStockPrice() - stock.getDCFCompanyGrowth();
	stock.setIntrValueCompanyGrDiff(val);


	stock.setBalanceSheet(
		BookValueGrowth, 
		priceToBookVal, 
		totalDebtPerShare,
		totalDebtPerSharePercentage, 
		yearsToReturnDebt, 
		sharesIssuedGrowth);

	stock.calcVecsPerShare();

	stock.printStock();	
}


void Services::InvDev::storeData()
{
	std::cout << "[MB] Services::InvDev storeData ..." << '\n';

}



// ---- POSTPROCESS POSTPROCESS POSTPROCESS----
// ---- POSTPROCESS POSTPROCESS POSTPROCESS----
// ---- POSTPROCESS POSTPROCESS POSTPROCESS----
void Services::InvDev::sortStocksByNetProfitMargin() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getNetProfitRatioAPI() < rhs.getNetProfitRatioAPI(); 
		});

	float totalScoreFloat = 1.4;
	
	for(auto& s : m_stocksVec) {
		s.m_totalScoreFloat += totalScoreFloat;

		totalScoreFloat += 1.4;
	}
}


void Services::InvDev::sortStocksByNetProfitMarginForPrint() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getNetProfitRatioAPI() > rhs.getNetProfitRatioAPI(); 
		});
}


void Services::InvDev::printStocksByNetProfitMargin() {
	// Find longest stock name
	std::string maxLengthStr = m_stocksVec.front().getFullName();

	for(auto s : m_stocksVec) {
		if(maxLengthStr.length() < s.getFullName().length()) maxLengthStr = s.getFullName();
	}

	std::cout << "____________________" << '\n';
	std::cout << "[ NET PROFIT RATIO ]" << '\n';
	for(auto s : m_stocksVec) {
		s.printStocksByNetProfit(maxLengthStr.length());
	}
	std::cout << '\n';
}

// ----

void Services::InvDev::sortStocksByYearsToReturnDebt() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getYearsToPayDebt() > rhs.getYearsToPayDebt(); 
		});

	float totalScoreFloat = 1.3;

	for(auto& s : m_stocksVec) {
		s.m_totalScoreFloat += totalScoreFloat;

		totalScoreFloat += 1.3;
	}
}


void Services::InvDev::sortStocksByYearsToReturnDebtForPrint() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getYearsToPayDebt() < rhs.getYearsToPayDebt(); 
		});
}


void Services::InvDev::printStocksByYearsToReturnDebt() {
	// Find longest stock name
	std::string maxLengthStr = m_stocksVec.front().getFullName();

	for(auto s : m_stocksVec) {
		if(maxLengthStr.length() < s.getFullName().length()) maxLengthStr = s.getFullName();
	}

	std::cout << "________________________" << '\n';
	std::cout << "[ YEARS TO RETURN DEBT ]" << '\n';
	for(auto s : m_stocksVec) {
		s.printYearsToReturnDebt(maxLengthStr.length());
	}
	std::cout << '\n';
}

// ----

void Services::InvDev::sortStocksByReturnOnEquityRatio() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getReturnOnEquityAPI() < rhs.getReturnOnEquityAPI(); 
		});

	float totalScoreFloat = 1.2;

	
	for(auto& s : m_stocksVec) {
		s.m_totalScoreFloat += totalScoreFloat;

		totalScoreFloat += 1.2;
	}
}

// ----

void Services::InvDev::sortStocksByPERatio() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getPERatio() > rhs.getPERatio(); 
		});

	float totalScoreFloat = 1.1;

	
	for(auto& s : m_stocksVec) {
		s.m_totalScoreFloat += totalScoreFloat;

		totalScoreFloat += 1.1;
	}
}

// ----

void Services::InvDev::sortStocksByPriceToBookValue() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
			return lhs.getPriceToBookRatioAPI() > rhs.getPriceToBookRatioAPI(); 
		});

	float totalScoreFloat = 1.0;

	for(auto& s : m_stocksVec) {
		s.m_totalScoreFloat += totalScoreFloat;

		totalScoreFloat += 1.0;
	}
}



//
// FINAL VALUE SCORE
void Services::InvDev::sortStocksByTotalScore() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
		return lhs.m_totalScoreFloat > rhs.m_totalScoreFloat;
	});

}


void Services::InvDev::printStocksByTotalScore() {
	// Find longest stock name
	std::string maxLengthStr = m_stocksVec.front().getFullName();

	for(auto s : m_stocksVec) {
		if(maxLengthStr.length() < s.getFullName().length()) maxLengthStr = s.getFullName();
	}

	std::cout << "_______________" << '\n';
	std::cout << "[ TOTAL SCORE ]" << '\n';
	for(auto s : m_stocksVec) {
		s.printStocksByFinalIncomeStatementScr(maxLengthStr.length());
	}
	std::cout << '\n';
}


// DCF INTRINSIC VALUE
void Services::InvDev::sortStocksByIntrinsicValue() {

	std::sort(std::begin(m_stocksVec), std::end(m_stocksVec), [](Stock& lhs, Stock& rhs) { 
		return lhs.getIntrValueZeroGrDiff() < rhs.getIntrValueZeroGrDiff(); 
	});

}


void Services::InvDev::printStocksByIntrinsicValue() {

	// Find longest stock name
	std::string maxLengthStr = m_stocksVec.front().getFullName();

	for(auto s : m_stocksVec) {
		if(maxLengthStr.length() < s.getFullName().length()) maxLengthStr = s.getFullName();
	}

	std::cout << "______________________________" << '\n';
	std::cout << "[ SHARES DCF INTRINSIC VALUE ]" << '\n';

	size_t diff0 = maxLengthStr.size() + 4;
	std::string str0(diff0, ' ');
	std::string str1(2, ' ');
	std::cout << str0 << "[Stock]" << str1 << "[Total Score]" << str1 <<"[Price]" << str1 << 
		"[0 Gr DCF]" << '\n';

	for(auto s : m_stocksVec) {
		s.printStockByIntrinsicValueGr(maxLengthStr.length());
	}
}




// ---- HELPER ----
// ---- HELPER ----
bool Services::InvDev::calcLinearRegressCoeffs(const std::vector<double>& y, double& a, double& b)
{
	std::vector<double> rangeYrs(y.size());
	std::iota(rangeYrs.begin(), rangeYrs.end(), 1); // 1, 2, 3, 4 ...

    double sumX = 0;
    double sumX2 = 0; 
    double sumY = 0; 
    double sumXY = 0;

    // Calculate required sums
    for(int i = 0; i <= rangeYrs.size(); i++)
    {
        sumX =  sumX + rangeYrs[i];
        sumX2 = sumX2 + rangeYrs[i] * rangeYrs[i];
        sumY =  sumY + y[i];
        sumXY = sumXY + rangeYrs[i] * y[i];
    }

    // Calculating a and b coeff
    int n = rangeYrs.size(); // Number of points

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







double Services::InvDev::_new_calculateK(const double& a, const double& b, std::vector<Data>& vec) 
{
	double nextYearVal = a + b * (vec.size() + 1);
	double nextNextYearVal = a + b * (vec.size() + 2);

	// Percentage growth (k)
	double growth = nextNextYearVal / nextYearVal - 1;

	// Special cases
	if(nextYearVal < 0.0 && nextNextYearVal < 0.0) {
		growth = -growth;
	} else if (nextYearVal < 0.0 && nextNextYearVal > 0.0) {
		growth = -growth;
	} else if (nextYearVal < 0.0 && nextNextYearVal < 0.0 && nextYearVal < nextNextYearVal) {
		growth = -growth;
	}
	

	return growth;
}


double Services::InvDev::calculateK(const double& a, const double& b, std::vector<double>& vec) {

	double nextYearVal = a + b * (vec.size() + 1);
	double nextNextYearVal = a + b * (vec.size() + 2);

	// Percentage growth (k)
	double growth = nextNextYearVal / nextYearVal - 1;

	// Special cases
	if(nextYearVal < 0.0 && nextNextYearVal < 0.0) {
		growth = -growth;
	} else if (nextYearVal < 0.0 && nextNextYearVal > 0.0) {
		growth = -growth;
	} else if (nextYearVal < 0.0 && nextNextYearVal < 0.0 && nextYearVal < nextNextYearVal) {
		growth = -growth;
	}
	

	return growth;
}


double Services::InvDev::calculateDCF(const double& incrRate, const double& FCFPerS, double& error)
{
	double incRate = incrRate;
	double FCFPS = FCFPerS;

	// How much money (percentage) to make	
	double interestRate = 0.15;
	double numerator = 1 + interestRate;

    double previousSum = 0.0;
	double nextSum = 0.0;

	double DCFError = 0.0;

	for (int i = 1; i <= 100; ++i)
	{
		double summand = FCFPS / pow(numerator, i);
	    
	    // Calculate next FCFPS
	    FCFPS = FCFPS + incRate * FCFPS;
	    
		previousSum = nextSum;
		nextSum = nextSum + summand;

		DCFError = nextSum - previousSum;

		if (DCFError < 0.05)
		{
		    break;
		}
	}

	error = DCFError;
	return nextSum;
}
