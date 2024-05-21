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
	

	bool _getFromSummary(Stock& stock);

	bool _getFromIncomeStatement(Stock& stock);	

	// INCOME STATEMENT
	bool _getRevenueAndEPSPrediction(const std::string& stockTicker,
		std::vector<double>& revenuePredictionVec, 
		std::vector<double>& epsPredictionVec);

	// BALANCE SHEET
	bool _getFromBalanceSheet(Stock& stock);

	// CAHS FLOW STATEMENT
	bool _getFromCashFlowStatement(Stock& stock);


	// ANLYSIS
    bool _getFromAnalysisStatement(const std::string& stockTicker,
		std::vector<double>& EPSEstimates,
		std::vector<double>& GrowthEstimates);


	// Helper Method(s)
	void getDataFromServer(const std::string& server, const std::string& path, std::string& data);


private:
std::string m_dbPath;
std::string m_dbPathWithName;
std::string m_name;

};

} // End of Namespace


