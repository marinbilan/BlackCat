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

	// BALANCE SHEET
	virtual void sortStocksByYearsToReturnDebt() = 0;
	virtual void sortStocksByDebtPerSharePrice() = 0;
	virtual void sortStocksByPriceToBookValue() = 0;
	virtual void sortStocksBySharesIssuedGrowth() = 0;
	virtual void sortStocksByFinalScore() = 0;



	// INCOME STATEMENT
	virtual void sortStocksByPERatio() = 0;
	virtual void sortStocksByGrossProfit() = 0;
	virtual void sortStocksByAvrGrowth() = 0;
	virtual void sortStocksByFinalIncomeStatementScore() = 0;

	virtual void calculateTotalScore() = 0;
	virtual void sortStocksByBalanceSheetAndIncomeStatementScore() = 0;

	virtual void sortStocksByZeroGrowthIntrinsicValue() = 0;

	// PRINT
	virtual void printStocksByYearsToReturnDebt() = 0;
	virtual void printStocksByDebtPerSharePercentage() = 0;
	virtual void printStocksByPriceToBookValue() = 0;
	virtual void printStocksBySharesIssuedGrowth() = 0;
	virtual void printStocksByFinalBalanceSheetScore() = 0;

	virtual void printStocksByPERatio() = 0;
	virtual void printStocksByGrossProfit() = 0;
	virtual void printStocksByAvgGrowth() = 0;
	virtual void printStocksByFinalIncomeStatementScore() = 0;

	virtual void printStocksByBalanceAndIncomeStatement() = 0;

	virtual void printStocksByIntrinsicValue() = 0;
	// ---- POSTPROCESS POSTPROCESS POSTPROCESS----

private:
};

} // End of namespace