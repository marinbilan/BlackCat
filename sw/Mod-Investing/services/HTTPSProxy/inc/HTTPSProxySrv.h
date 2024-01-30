#pragma once
#include "HTTPSProxySrvIf.h"


namespace Services
{

class HTTPSProxySrv : public HTTPSProxySrvIf
{
public:
	HTTPSProxySrv(const std::string& dbPath, const std::string& name);
	~HTTPSProxySrv();

	const std::string& getName() const
	{
		return m_name;
	}

	void preInit();
	void postInit();
	

	bool _getFromSummary(const std::string& stockTicker, double& stockPrice, double& PE_Ratio);

	bool _getFromIncomeStatement(const std::string& stockTicker,
		std::vector<double>& revenueVec, 
		std::vector<double>& grossProfitVec,
		std::vector<double>& netIncomeVec, bool standard);	

	// INCOME STATEMENT
	bool _getRevenueAndEPSPrediction(const std::string& stockTicker,
		std::vector<double>& revenuePredictionVec, 
		std::vector<double>& epsPredictionVec);

	// BALANCE SHEET
	bool _getFromBalanceSheet(const std::string& stockTicker,
		std::vector<double>& bookValueVec, // Assets - Liabilities
		std::vector<double>& totalDebtVec,
		std::vector<double>& shareIssuedVec, bool standard);

	// CAHS FLOW STATEMENT
	bool _getFromCashFlowStatement(Stock& stock, bool standard);


	// ANLYSIS
    bool _getFromAnalysisStatement(const std::string& stockTicker,
		std::vector<double>& EPSEstimates,
		std::vector<double>& GrowthEstimates);


private:
std::string m_dbPath;
std::string m_dbPathWithName;
std::string m_name;

};

} // End of Namespace


