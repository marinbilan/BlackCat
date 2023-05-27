#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <vector>


namespace Services
{

class HTTPSProxySrvIf
{
public:
	~HTTPSProxySrvIf()
	{
	}

	virtual const std::string& getName()
	{
		return "";
	}

	virtual void preInit() {};

	virtual void postInit() {};

    virtual bool _getFromIncomeStatement(const std::string& stockTicker,
		std::vector<double>& revenueVec,
		std::vector<double>& grossProfitVec,
		std::vector<double>& netIncomeVec) = 0;

    virtual bool _getFromBalanceSheet(const std::string& stockTicker,
		std::vector<double>& bookValueVec,
		std::vector<double>& totalDebtVec,
		std::vector<double>& shareIssuedVec) = 0;

    virtual bool _getFromCashFlowStatement(const std::string& stockTicker,
		std::vector<double>& cashFlowVec) = 0;

    virtual bool _getFromAnalysisStatement(const std::string& stockTicker,
		std::vector<double>& EPSEstimates,
		std::vector<double>& GrowthEstimates) = 0;

	// ==== LINEAR REGRESSION ====
	virtual bool calcLinearRegressCoeffs(const std::vector<double>& x, 
							     const std::vector<double>& y,
								 double& a, 
								 double& b) = 0;
private:
};

} // End of namespace