#pragma once
#include "HTTPSProxySrvIf.h"


namespace Services
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
	bool _getFromCashFlowStatement(const std::string& stockTicker,
		std::vector<double>& cashFlowVec, bool standard);

	// ANLYSIS
    bool _getFromAnalysisStatement(const std::string& stockTicker,
		std::vector<double>& EPSEstimates,
		std::vector<double>& GrowthEstimates);

	// ==== LINEAR REGRESSION ====
	bool calcLinearRegressCoeffs(const std::vector<double>& x, 
                                    const std::vector<double>& y,
                                    double& a, 
                                    double& b);

private:
std::string m_dbPath;
std::string m_dbPathWithName;
std::string m_name;

};

} // End of Namespace


