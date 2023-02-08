#pragma once
#include "StockCalcSrvIf.h"


namespace Service
{

class StockCalcSrv : public StockCalcSrvIf
{
public:
	StockCalcSrv(const std::string& dbPath, const std::string& name);
	~StockCalcSrv();

	const std::string& getName()
	{
		return m_name;
	}

	void preInit();
	void postInit();

	// Start for Discount Cash Flow calculation
	void gatherStockData();
	void calcDCF();

	// Private stuff
	// Part 1
	double calcFCFdivByNetIncomeForEachYear();
	double projectPercentageRevenueGrowth();
	void projectRevenueNextNYears();
	void calcNetIncomeMargines();
	void calcFCFProjectionForNextNYears();

	// Part 2
	// Default required rate of return
	void calcRequiredRateOfReturn();
	// Set wanted required rate of return
	void setRequiredRateOfReturn(double rateOfRetrunWAAC);
	// INTRINSIC VALUE
	void calcStockFairIntrinsicValue();

	// //// ////
	// Set vectors for Unit test
	void setVectors(const std::vector<double>& netIncomeVec,
		const std::vector<double>& fCFVec,
		const std::vector<double>& totalRevenueVec);

	// Set vectors for Unit test
	void setParamsForRequiredRateOfReturn(double interestExpanse,
		double totalDebt,
		double pretaxIncome,
		double taxProvision,
		double Rf,
		double Beta,
		double Rm,
		double marketCap,
		long   SharesOutstanding,
		double perpetualGrowth);

	// NEW
	// ==== LINEAR REGRESSION ====
	bool calcLinearRegressCoeffs(const std::vector<double>& x, 
							     const std::vector<double>& y,
								 double& a, 
								 double& b);
private:
std::string m_dbPath;
std::string m_dbPathWithName;
std::string m_name;

// DCF related stuff
std::vector<double> m_NetIncomeVec;
std::vector<double> m_FCFVec;
std::vector<double> m_TotalRevenueVec;

double m_interestExpanse;
double m_totalDebt;
double m_pretaxIncome;
double m_taxProvision;
double m_Rf;
double m_Beta;
double m_Rm;
double m_marketCap;
long   m_SharesOutstanding;
double m_perpetualGrowth;

// To calculate
double m_minFCFRatio;
double m_avgRevenueGrowth;

std::vector<double> m_FCFVecProjection;
double m_WAAC;
double m_FairValueOfEquity;
};

}