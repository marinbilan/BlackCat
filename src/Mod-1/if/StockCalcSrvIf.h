#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>

namespace Service
{

class StockCalcSrvIf
{
public:
	virtual ~StockCalcSrvIf()
	{
		std::cout << "Delete ~StockCalcSrvIf()" << '\n';
	}

	virtual const std::string& getName()
	{
		return "Err: Called from interface";
	}

	virtual void preInit() {};
	virtual void postInit() {};

	// Start DCF calculation
	virtual void gatherStockData() {};
	virtual void calcDCF() {};


	// Private - API for Unit test
	virtual double calcFCFdivByNetIncomeForEachYear() {};
	virtual double projectPercentageRevenueGrowth() {};
	virtual void projectRevenueNextNYears() {};
	virtual void calcNetIncomeMargines() {};
	virtual void calcFCFProjectionForNextNYears() {};
	// Default required rate of return
	virtual void calcRequiredRateOfReturn() {};
	// Set wanted required rate of return
	virtual void setRequiredRateOfReturn(double rateOfRetrunWAAC) {};

	virtual void calcStockFairIntrinsicValue() {};

	// Set vectors for Unit test
	virtual void setVectors(const std::vector<double>& netIncomeVec,
		const std::vector<double>& fCFVec,
		const std::vector<double>& totalRevenueVec) {};

	// Set vectors for Unit test
	virtual void setParamsForRequiredRateOfReturn(double interestExpanse,
		double totalDebt,
		double pretaxIncome,
		double taxProvision,
		double Rf,
		double Beta,
		double Rm,
		double marketCap,
		long   SharesOutstanding,
		double perpetualGrowth) {};

private:
};

} // End of namespace