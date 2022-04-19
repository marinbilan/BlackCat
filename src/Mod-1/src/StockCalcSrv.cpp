#include "StockCalcSrv.h"


Service::StockCalcSrv::StockCalcSrv(const std::string& dbPath, const std::string& name) : 
	m_dbPath(dbPath),
	m_name(name),
	m_dbPathWithName(dbPath + name + "_")
{
}


Service::StockCalcSrv::~StockCalcSrv()
{
	std::cout << "Delete ~StockCalcSrv()" << '\n';
}


void Service::StockCalcSrv::preInit()
{
	// Get params from DB for this instance
	std::cout << "StockCalcSrv preInit() called!" << '\n';
}


void Service::StockCalcSrv::postInit()
{
	std::cout << "StockCalcSrv postInit() called!" << '\n';
}


void Service::StockCalcSrv::calcDCF()
{
	std::cout << "---- calcDCF ----" << '\n';
	/*
	STEP 1.1 : Gather data from Yahoo Finance
	*/
	// gatherStockData();

	/*
	STEP 1.2 : Calculate FCF / NetIncome foreach year
	*/
	// calcFCFdivByNetIncomeForEachYear();

	/*
	STEP 1.3 : Project Revenue Growth in percentage
	*/
	// projectPercentageRevenueGrowth();

	/*
	STEP 1.4 : Net Income margines (NetIncome / Revenue) 
	*/
	// calcNetIncomeMargines();

	/*
	STEP 1.5 : Calculate FCF for next N years (Default 4 years)
	*/
	// calcFCFProjectionForNextNYears();
}


// PART: 1
void Service::StockCalcSrv::gatherStockData()
{
	std::cout << "---- gatherStockData ----" << '\n';
    // [ STEP 1 - Free Cash Flow projection ]
    /*
    Free Cash Flow (FCF) should be in line with prfitability (Net Income from Counting Operations)
    */
    // Net income from Counting Operations (From latest to newest year)
    // ==== ====
    // INPUT 1 (From Yahoo! Finance):
    m_NetIncomeVec = {53'394'000, 45'687'000, 48'351'000, 59'531'000};  

    // Predict next four years from Revenue projections and project it to Free Cash Flow for next (N = 4) years
    // Free Cash Flow
    // INPUT 2 (From Yahoo! Finance)
    m_FCFVec = {70'019'000, 53'497'000, 51'774'000, 64'121'000};
   
    // ----
    // Revenue projections for next two years [2019 (257.31 B), 2020 (269.68 B)]
    // Note that last two years are projections from Yahoo Finance - Analysis page
    //                                             2015         2016         2017         2018         2019         2020    // Next two years (2019, 2020) from Yahoo Finance
    // INPUT 3 (From Yahoo! Finance) - Add two projected years (2019, 2020)
    m_TotalRevenueVec = { 233'715'000, 215'639'000, 229'234'000, 265'595'000, 257'310'000, 269'680'000 }; // Predict next two years from avg revenue growth
    // Analyst's projected revenue growth:                       -8%           6%          16%          -3%           5%
   
    // From here - we will project two more revenue years [2021, 2022] 
    // Output of calculation is: FCFVecProjection - vector of Free Cash Flow of next 4 years
   
    // ==== ====
    // [ STEP 2 - WAAC Projection ]
   
    // INPUT 4: Interest Expanse (From Income Statement)
    m_interestExpanse = 3'240'000;
    // INPUT 5: Interest Expanse (From Balance Sheet)
    m_totalDebt = 102'519'000;
   
    // INPUT 6: - Pretax Income (Income Statement)
    m_pretaxIncome = 72'902'000;
   
    // INPUT 7: - Tax Provision (Income Tax Expense) (Income Statement)
    m_taxProvision = 13'372'000;
   
    // Yahoo Finance (Rf) = 2.32 % (0.0232)
    // INPUT 8: - Xahoo! Finance > Markets > US Treasury Bond Rates > Treasury Yield 10 Years (Rf)
    m_Rf = 0.0232;
   
    // INPUT 9: - B (beta for the stock) = 0.89 (Yahoo! Finance)
    m_Beta = 0.89;
   
    // INPUT 10: - Rm - expected return of the market (around 10 % - SP500 return avg)
    m_Rm = 0.10;
   
    // INPUT 11: - MarketCap
    m_marketCap = 826'629'000;
   
    // INPUT 12: - Shares outstanding
    // Number of shares from K-10 (Annual report) - Important to remove last three digits!
    m_SharesOutstanding = 4'601'075;
   
    // INPUT 13: - PERPETUAL GROWTH RATE is growth of world economy
    // The growth that FREE CASH FLOW is going to grow forever
    m_perpetualGrowth = 0.025;
}


double Service::StockCalcSrv::calcFCFdivByNetIncomeForEachYear()
{
    std::cout << " ---- Calculate FCF / NetIncome foreach year ---- " << '\n';

    auto itNI  = m_NetIncomeVec.begin();
    auto itFCF = m_FCFVec.begin();
   
    std::vector<double> FCFdivByNetIncome;
    double FCFNIRatio = 0.0;
   
    // Check that both vectors are the same size - return error if not
    while(itNI != m_NetIncomeVec.end())
    {
        FCFNIRatio = *itFCF / *itNI;
        FCFdivByNetIncome.push_back(FCFNIRatio);
        std::cout << "FCF = " << *itFCF << " " << "Net Income = " << *itNI << " .... FCFNIRatio: " << FCFNIRatio << '\n';
       
        itNI++;
        itFCF++;
    }
   
    // Calculate minimum value in FCF div NI ratio vector - That is most conservative value
    auto minFCFNIRatio = std::min_element(FCFdivByNetIncome.begin(), FCFdivByNetIncome.end());
    m_minFCFRatio = *minFCFNIRatio;

    std::cout << "Minimum FCF to NetIncome ratio: " << m_minFCFRatio << '\n';
    return m_minFCFRatio;
}


double Service::StockCalcSrv::projectPercentageRevenueGrowth()
{
	std::cout << '\n' << " ---- Project Revenue Growth---- " << '\n';
    /*
    Let's project Revenue - Same concept can be apply for Net Income
    1. Yahoo Finance - Analysis - Find next two years (2019, 2020)
    2. Calculate percentage growth
    */
    double previousValue = 0;
    double delta = 0;
    double deltaPercentage = 0;
   
    std::vector<double> vecOfRevenueDeltasPercentage;
    for (auto it = m_TotalRevenueVec.begin() + 1; it != m_TotalRevenueVec.end(); ++it)
    {
        // Calculate diff between current revenue year and last year
        previousValue = *(it - 1);
        delta = *it - previousValue;
        // To percentage
        deltaPercentage = delta / *it;
       
        vecOfRevenueDeltasPercentage.push_back(deltaPercentage);
       
        std::cout << "Current year:" << *it << " Previous year: " << previousValue << " .... Delta: " << delta << " Delta percentage: " << deltaPercentage << '\n';
    }
   
    // Calculate revenut average growth
    m_avgRevenueGrowth = std::reduce(vecOfRevenueDeltasPercentage.begin(), vecOfRevenueDeltasPercentage.end(), 0.0) / vecOfRevenueDeltasPercentage.size();
    std::cout << "Average revenue growth: " << m_avgRevenueGrowth << '\n';

    return m_avgRevenueGrowth;
}


void Service::StockCalcSrv::projectRevenueNextNYears()
{
   	std::cout << '\n' << "---- Project N more revenue years with calculated revenue (percentage) growth ----" << '\n';
    enum YEARS
    {
        ZERO_YEAR,
        ONE_YEAR,
        TWO_YEARS,  // <---- [ Project two years ]
        THREE_YEARS,
        FOUR_YEARS
    };
   
    double lastRevenueYear = *(m_TotalRevenueVec.end() - 1);
    double nextRevenueYear = 0;
   
    // Push back projected values in revenue vector
    for(int i = 0; i < TWO_YEARS; ++i)
    {
        nextRevenueYear = lastRevenueYear + lastRevenueYear * m_avgRevenueGrowth;
       
        std::cout << "NextRevenueYear: " << nextRevenueYear << '\n';
        m_TotalRevenueVec.push_back(nextRevenueYear);
       
        lastRevenueYear = *(m_TotalRevenueVec.end() - 1);
    }
}


void Service::StockCalcSrv::calcNetIncomeMargines()
{
    // At this point we have 4 years revenue estimates (2 years analyst + 2 years predicted from past ratio)
    // Now when we have revenue estimates we need to convert it in Net Income estimates (From which we will predict FCF next 4 years)
    std::cout << '\n' << "---- Net Income margines (NetIncome / Revenue) ----" << '\n';
    auto itTR_St4 = m_TotalRevenueVec.begin();
    auto itNI_St4 = m_NetIncomeVec.begin();
   
    std::vector<double> vecOfNetIncomeMargines;
    double tempRatioNetIncMargines = 0.0;
    while(itNI_St4 != m_NetIncomeVec.end())
    {
        tempRatioNetIncMargines = *itNI_St4 / *itTR_St4;
        vecOfNetIncomeMargines.push_back(tempRatioNetIncMargines);
        std::cout << *itNI_St4 << " " << *itTR_St4 << " tempRatioNetIncMargines: " << tempRatioNetIncMargines << '\n';
       
        itTR_St4++;
        itNI_St4++;
    }
   
    // Calculate average net income margines (for not predicted years)
    double avgNetIncomeMargines = std::reduce(vecOfNetIncomeMargines.begin(), vecOfNetIncomeMargines.end(), 0.0) / vecOfNetIncomeMargines.size();
    std::cout << "AvgNetIncomeMargines: " << avgNetIncomeMargines << '\n';
   
    // Add 4 more years in NetIncomeVec by multiply revenue x net margines (Net Income / Revenue)
    for(auto revIter = m_TotalRevenueVec.begin() + m_NetIncomeVec.size(); revIter != m_TotalRevenueVec.end(); ++revIter)
    {
        double NetIncomeRest = *revIter * avgNetIncomeMargines;
        m_NetIncomeVec.push_back(NetIncomeRest);
    }
}


void Service::StockCalcSrv::calcFCFProjectionForNextNYears()
{
    // Now similar as above calculation - We take calculated Net Income and multiply it with NetFCFRest
    std::cout << '\n' << "---- Net Income to Free Cash Flow ----" << '\n';

    for(auto ni_Iter = m_NetIncomeVec.begin() + m_FCFVec.size(); ni_Iter != m_NetIncomeVec.end(); ++ni_Iter)
    {
        double NetFCFRest = *ni_Iter * m_minFCFRatio;
        m_FCFVec.push_back(NetFCFRest);
        m_FCFVecProjection.push_back(NetFCFRest);
        // std::cout << "ni_Iter: " << *ni_Iter << " NetFCFRest: " << NetFCFRest << '\n';
    }
   
  
    std::cout << '\n'<< "Free Cash Flow Projection for next four years:" << '\n';
    for(auto& s : m_FCFVecProjection)
    {
        std::cout << s << '\n';
    }
}


// PART: 2
void Service::StockCalcSrv::calcRequiredRateOfReturn()
{
    std::cout << '\n' << "---- Calculate Required Rate of Return ----" << '\n';
    // REQUIRED RATE OF RETURN
    /*
    - wd - (weight of debt)
    - rd -  
    - t  - Effective Tax Rate
    - we - (weight of equity)
    - re -
   
    WAAC = wd * rd * (1-t) + we * re
    */
    double rd = m_interestExpanse / m_totalDebt;
    std::cout << "rd = " << rd << '\n';
   
    // (1 - t)
    // t - Calculated Effective Tax Rate

    double calculatedEffectiveTaxRate = m_taxProvision / m_pretaxIncome;
    std::cout << "calculatedEffectiveTaxRate = " << calculatedEffectiveTaxRate << '\n';
   
    // Cost of Debt = rd * (1 - t)
    // Calculated Effective Tax Rate - Check details in 10-K
    double costOfDebt = rd * ( 1 - calculatedEffectiveTaxRate );
    std::cout << "costOfDebt = " << costOfDebt << '\n' << '\n';
   
   
    // Cost of equity (re)
    // re = Rf + B(Rm - Rf)
    // Important: This value is in (%)
    double re = m_Rf + m_Beta * (m_Rm - m_Rf);
    std::cout << "re = " << re << '\n';
   
    // Total Amount of Capital
    double totalAmountOfCapital = m_totalDebt + m_marketCap;
   
    double wd = m_totalDebt / totalAmountOfCapital;
    std::cout << "wd = " << wd << '\n';
   
    double we = 1 - wd;
    std::cout << "we = " << we << '\n';
   
    m_WAAC = wd * costOfDebt + we * re;
    std::cout << "WAAC = " << m_WAAC << '\n' << '\n';
}


void Service::StockCalcSrv::setRequiredRateOfReturn(double rateOfRetrunWAAC)
{
	m_WAAC = rateOfRetrunWAAC;
}


void Service::StockCalcSrv::calcStockFairIntrinsicValue()
{
    // Now we need to calculate present value of company
    // 1st Year - 2019
    // 2nd Year - 2020
    // ...
   
    // Terminal Value:
    // g - perpetual growth (~2,5 %)
    // r - wanted return (WAAC)
    // TerminalValue (V0) = FCFE0 x (1 + g) / (r - g)
    /*
    We take last year of prediction and we grow it ->
    */
    std::cout << "Last FCF value: " << *(m_FCFVecProjection.end() - 1) << '\n';
    double lastFCFProjectedVal = *(m_FCFVecProjection.end() - 1);
   
    // r - WAAC - required reate of return
    double TerminalValue = lastFCFProjectedVal * (1 + m_perpetualGrowth) / (m_WAAC - m_perpetualGrowth);
    // Important - this is in thousands!! Because last predicted FCF value (4th year) is in thousands!
    std::cout << "TerminalValue: " << TerminalValue << '\n' << '\n';
   
   
    // Place TerminalValue in FCF projection vector
    m_FCFVecProjection.push_back(TerminalValue);
   
    for(auto s : m_FCFVecProjection)
    {
        std::cout << "FCF val: " << s << '\n';
    }
   
    std::cout << '\n';
   
    // TODO: Use std::accumulate
    double sum = 0;
    for(auto it = m_FCFVecProjection.begin(); it != m_FCFVecProjection.end(); ++it)
    {
        int index = std::distance(m_FCFVecProjection.begin() - 1, it);
        if(it == m_FCFVecProjection.end() - 1) index = index - 1;
       
        // 1st - Calculate denominator
        double denominator = pow((1 + m_WAAC), index);
       
        // 2nd - Calculate ratio
        double ratioFCF = *it / denominator;
        // std::cout << "Ratio: " << ratioFCF << '\n';
       
        sum += ratioFCF;
    }
   
    std::cout << "Today company value: " << sum << '\n' << '\n';
   
    m_FairValueOfEquity = sum / m_SharesOutstanding;
    std::cout << ">>>> STOCK FAIR INTRINSIC VALUE: " << m_FairValueOfEquity << '\n';
}



// ---- Set vectors for Unit test ----
void Service::StockCalcSrv::setVectors(const std::vector<double>& netIncomeVec,
	const std::vector<double>& fCFVec,
	const std::vector<double>& totalRevenueVec)
{
	m_NetIncomeVec = netIncomeVec;
	m_FCFVec = fCFVec;
	m_TotalRevenueVec = totalRevenueVec;
}


// Set vectors for Unit test
void Service::StockCalcSrv::setParamsForRequiredRateOfReturn(double interestExpanse,
	double totalDebt,
	double pretaxIncome,
	double taxProvision,
	double Rf,
	double Beta,
	double Rm,
	double marketCap,
	long   SharesOutstanding,
	double perpetualGrowth) 
{
	m_totalDebt = totalDebt;
	m_pretaxIncome = pretaxIncome;
	m_taxProvision = taxProvision;
	m_Rf = Rf;
	m_Beta = Beta;
	m_Rm = Rm;
	m_marketCap = marketCap;
	m_SharesOutstanding = SharesOutstanding;
	m_perpetualGrowth = perpetualGrowth;
};