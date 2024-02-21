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
	

	bool _getFromSummary(const std::string& stockTicker, std::string& stockName, double& stockPrice, double& PE_Ratio);

	bool _getFromIncomeStatement(Stock& stock, bool standard);	

	// INCOME STATEMENT
	bool _getRevenueAndEPSPrediction(const std::string& stockTicker,
		std::vector<double>& revenuePredictionVec, 
		std::vector<double>& epsPredictionVec);

	// BALANCE SHEET
	bool _getFromBalanceSheet(Stock& stock, bool standard);

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


