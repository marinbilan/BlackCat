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
		static std::string empty;
		return empty;
	}

	virtual void preInit() {};

	virtual void postInit() {};

	virtual void _new_GetDataFromServer(Company& company) = 0;







	virtual bool _getFromSummary(Stock& stock) = 0;

	virtual bool _getRatios(Stock& stock) = 0;

	virtual bool _getDCF(Stock& stock) = 0;

    virtual bool _getFromIncomeStatement(Stock& stock) = 0;

    virtual bool _getFromBalanceSheet(Stock& stock) = 0;

    virtual bool _getFromCashFlowStatement(Stock& stock) = 0;

    virtual bool _getFromAnalysisStatement(const std::string& stockTicker,
		std::vector<double>& EPSEstimates,
		std::vector<double>& GrowthEstimates) = 0;

private:
};

} // End of namespace