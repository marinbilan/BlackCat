#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "InvDev0.h"


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


	virtual bool _getFromSummary(const std::string& stockTicker, double& stockPrice, double& PE_Ratio) = 0;

    virtual bool _getFromIncomeStatement(const std::string& stockTicker,
		std::vector<double>& revenueVec,
		std::vector<double>& grossProfitVec,
		std::vector<double>& netIncomeVec,
		bool standard) = 0;

    virtual bool _getFromBalanceSheet(const std::string& stockTicker,
		std::vector<double>& bookValueVec,
		std::vector<double>& totalDebtVec,
		std::vector<double>& shareIssuedVec,
		bool standard) = 0;

    virtual bool _getFromCashFlowStatement(Stock& stock, bool standard) = 0;

    virtual bool _getFromAnalysisStatement(const std::string& stockTicker,
		std::vector<double>& EPSEstimates,
		std::vector<double>& GrowthEstimates) = 0;


private:
};

} // End of namespace