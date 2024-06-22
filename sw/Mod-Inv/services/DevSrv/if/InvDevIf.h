#pragma once
#include <string>



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
	virtual void collectData() = 0;
	virtual void calculateData() = 0;
	virtual void storeData() = 0;


	// ---- POSTPROCESS POSTPROCESS POSTPROCESS----
	virtual void sortStocksByGrossProfit() = 0;
	virtual void sortStocksByGrossProfitForPrint() = 0;
	virtual void printStocksByGrossProfit() = 0;

	virtual void sortStocksByYearsToReturnDebt() = 0;
	virtual void sortStocksByYearsToReturnDebtForPrint() = 0;
	virtual void printStocksByYearsToReturnDebt() = 0;

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