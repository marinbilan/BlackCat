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
	virtual void sortStocksByAvgFCFPerShare() = 0;

	// BALANCE SHEET
	virtual void sortStocksByYearsToReturnDebt() = 0;
	virtual void sortStocksByDebtPerSharePrice() = 0;
	virtual void sortStocksByPriceToBookValue() = 0;
	virtual void sortStocksBySharesIssuedGrowth() = 0;
	virtual void sortStocksByFinalScore() = 0;

	// INCOME STATEMENT
	virtual void sortStocksByPERatio() = 0;

	// PRINT
	virtual void printStocksByYearsToReturnDebt() = 0;
	virtual void printStocksByDebtPerSharePercentage() = 0;
	virtual void printStocksByPriceToBookValue() = 0;
	virtual void printStocksBySharesIssuedGrowth() = 0;
	virtual void printStocksByFinalScore() = 0;

	virtual void printStocksByPERatio() = 0;
	// ---- POSTPROCESS POSTPROCESS POSTPROCESS----

private:
};

} // End of namespace