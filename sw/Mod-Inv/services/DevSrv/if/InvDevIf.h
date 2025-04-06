#pragma once
#include <string>
#include <vector>


namespace Services
{

class InvDevIf
{
public:
	virtual ~InvDevIf()
	{
	}

	virtual const std::string& getName() = 0;

	virtual void preInit() = 0;
	virtual void postInit() = 0;

	// PROCEDURE
	virtual void collectData(const std::vector<std::string>& portfolio) = 0;
	virtual void calculateData() = 0;
	virtual void storeData() = 0;

	// NEW NEW NEW NEW POSTPROCESS
	virtual void _new_EvaluateCompanies() = 0;

	virtual void _new_sortCompaniesByTotalScore() = 0;
	virtual void _new_printCompaniesByTotalScore() = 0;

	virtual void _new_sortCompaniesByIntrinsicValue() = 0;
	virtual void _new_printCompaniesByIntrinsicValue() = 0;






	// ---- POSTPROCESS POSTPROCESS POSTPROCESS----
	virtual void sortStocksByNetProfitMargin() = 0;
	virtual void sortStocksByNetProfitMarginForPrint() = 0;
	virtual void printStocksByNetProfitMargin() = 0;

	virtual void sortStocksByYearsToReturnDebt() = 0;
	virtual void sortStocksByYearsToReturnDebtForPrint() = 0;
	virtual void printStocksByYearsToReturnDebt() = 0;

	virtual void sortStocksByReturnOnEquityRatio() = 0;

	virtual void sortStocksByPERatio() = 0;
	virtual void sortStocksByPriceToBookValue() = 0;

	//
	// FINAL VALUE SCORE
	virtual void sortStocksByTotalScore() = 0;
	virtual void printStocksByTotalScore() = 0;

	// DCF INTRINSIC VALUE
	virtual void sortStocksByIntrinsicValue() = 0;
	virtual void printStocksByIntrinsicValue() = 0;


private:
};

} // End of namespace