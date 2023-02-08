#pragma once
#include "HTTPSProxySrvIf.h"


namespace Service
{

class HTTPSProxySrv : public HTTPSProxySrvIf
{
public:
	HTTPSProxySrv(const std::string& dbPath, const std::string& name);
	~HTTPSProxySrv();

	const std::string& getName()
	{
		return m_name;
	}

	void preInit();
	void postInit();


	// --== HTTPS Access ==--
	// INCOME STATEMENT
	bool getFromIncomeStatement(const std::string& stockTicker,
		std::vector<double>& revenueVec, 
		std::vector<double>& grossProfitVec,
		std::vector<double>& netIncomeVec);

	// REVENUE AND EPS PREDICTION
	bool getRevenueAndEPSPrediction(const std::string& stockTicker,
		std::vector<double>& revenuePredictionVec, 
		std::vector<double>& epsPredictionVec);

	// BALANCE SHEET
	bool getFromBalanceSheet(const std::string& stockTicker,
		std::vector<double>& bookValueVec, // Assets - Liabilities
		std::vector<double>& totalDebtVec,
		std::vector<double>& shareIssuedVec);

	// CAHS FLOW STATEMENT
	bool getFromCashFlowStatement(const std::string& stockTicker,
		std::vector<double>& cashFlowVec,
		double& stockPrice);


private:
std::string m_dbPath;
std::string m_dbPathWithName;
std::string m_name;

};

} // End of Namespace


