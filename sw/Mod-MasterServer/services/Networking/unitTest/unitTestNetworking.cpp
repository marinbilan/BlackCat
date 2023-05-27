#include <gmock/gmock.h>
#include <memory>
#include <map>
#include <regex>
#include <fstream>
// #include <vector>
// Time measurement
#include <chrono>
#include <unistd.h>
#include <math.h>

#include "HTTPSProxySrv.h"

// Networking
#include "NetworkingIf.h"
#include "HTTPSClient.h"

// using ::testing::AtLeast;

// TEST FIXTURE //
class NetworkingUnitTest : public ::testing::Test
{
public:
// Networking::ClientIf* m_httpsClient;

	NetworkingUnitTest()
	{
		// m_httpsClient = new Networking::HTTPSClient("test", "test");
	}

	~NetworkingUnitTest()
	{
		// delete m_myClass;
	}
};


/*
[INCOME STATEMENT]

	1] Total Revenue [Exists]
	2] Gross Profit [Exists] - Revenue - Const of Revenue
	3] Net Income Common Stockhold [Exists] - should be consistent
	----

	Gross Margin [%] = Gross Profit / Revenue
	Net Margin [%] = Net Income / Revenue
*/
TEST_F(NetworkingUnitTest, DISABLED_getFromIncomeStatementTest)
{
	// [1] Preparation
	// ----
	// Create client and get Income Statement (HTML) in _HTTPSContent.txt file
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Income Statement- APPLE
    std::string server("finance.yahoo.com");
    std::string path("/quote/AAPL/financials?p=AAPL");

	Networking::HTTPSClient c(io_service, ctx, server, path);
    io_service.run();
    // ----


    // [2] Get: Revenue, Gross Profit Net Income Common Stackhold
    // ----
	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;

	std::regex regexRevenueLine("Total Revenue.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");
	std::regex regexGrossProfitLine("Gross Profit.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");
	std::regex regexNetIncomeLine("Net Income Common Stockholders.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");

	//
	while (std::getline(file, lineFromFile)) 
	{
		// Get Total Revenue
		if (std::regex_search(lineFromFile, match, regexRevenueLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string totalRevenueStr(match[i].str());
			    // Remove "," from data
			    totalRevenueStr.erase(remove(totalRevenueStr.begin(), totalRevenueStr.end(), ','), totalRevenueStr.end());

			    double num = stod(totalRevenueStr);
			    std::cout << "Total Revenue: " << num << '\n';
			    // Push back in vector
		    }
		}
		// Get Gross Profit
		if (std::regex_search(lineFromFile, match, regexGrossProfitLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string grossProfitStr(match[i].str());
			    // Remove "," from data
			    grossProfitStr.erase(remove(grossProfitStr.begin(), grossProfitStr.end(), ','), grossProfitStr.end());

			    double num = stod(grossProfitStr);
			    std::cout << "Gross Profit: " << num << '\n';
			    // Push back in vector
		    }
		}
		// Get Net Income
		if (std::regex_search(lineFromFile, match, regexNetIncomeLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string netIncomeStr(match[i].str());
			    // Remove "," from data
			    netIncomeStr.erase(remove(netIncomeStr.begin(), netIncomeStr.end(), ','), netIncomeStr.end());

			    double num = stod(netIncomeStr);
			    std::cout << "Net Income: " << num << '\n';
			    // Push back in vector
			    if(num != 0) {};
		    }
		}
	}

	// Close file
	file.close();
}


/*
[ BALANCE SHEET ]

	4] Retained Earnings - Added to or widraw (If company invest - Growth)
	5] Total Equity Gross Minority Interest [Equity]

	To measure how efficient company use this reinvestment (Retained Earnings) we calc:
	Return on Equity [%] = Net Income / Total Equity

	----
	Return on Net Tangible assets - More sofisticated method of how efficient
	company use retained earnings (Book Esseys of Warren Buffett)
	----

	Look to bussiness with no Long Term Debt
	6] Total Debt
	If company can pay whole Total Debt with 4 years Earnings, that is good position
*/
TEST_F(NetworkingUnitTest, DISABLED_getFromBalanceSheetTest)
{
	// [1] Preparation
	// ----
	// Create client and get Income Statement (HTML) in _HTTPSContent.txt file
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Income Statement- APPLE
    std::string server("finance.yahoo.com");
    std::string path("/quote/AAPL/balance-sheet?p=AAPL");

	Networking::HTTPSClient c(io_service, ctx, server, path);
    io_service.run();
    // ----


    // [2] Get:
    // ----
	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;

	std::regex regexTotalEquityLine("Total Equity Gross Minority Interest.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");
	std::regex regexTotalDebtLine("Total Debt.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");
	std::regex regexSharesNumberLine("Share Issued.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");

	//
	while (std::getline(file, lineFromFile)) 
	{
		// Get Total Equity
		if (std::regex_search(lineFromFile, match, regexTotalEquityLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string totalEquityStr(match[i].str());
			    // Remove "," from data
			    totalEquityStr.erase(remove(totalEquityStr.begin(), totalEquityStr.end(), ','), totalEquityStr.end());

			    double num = stod(totalEquityStr);
			    std::cout << "Total Equity: " << num << '\n';
			    // Push back in vector
		    }
		}
		// Get Total Debt
		if (std::regex_search(lineFromFile, match, regexTotalDebtLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string totalDebtStr(match[i].str());
			    // Remove "," from data
			    totalDebtStr.erase(remove(totalDebtStr.begin(), totalDebtStr.end(), ','), totalDebtStr.end());

			    double num = stod(totalDebtStr);
			    std::cout << "Total Debt: " << num << '\n';
			    // Push back in vector
		    }
		}
		// Get Number of Shares
		if (std::regex_search(lineFromFile, match, regexSharesNumberLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string totalNumberOfSharesStr(match[i].str());
			    // Remove "," from data
			    totalNumberOfSharesStr.erase(remove(totalNumberOfSharesStr.begin(), totalNumberOfSharesStr.end(), ','), totalNumberOfSharesStr.end());

			    double num = stod(totalNumberOfSharesStr);
			    std::cout << "Shares Number: " << num << '\n';
			    // Push back in vector
		    }
		}
	}

	// Close file
	file.close();
}



TEST_F(NetworkingUnitTest, DISABLED_getFromCashFlowTest)
{
	// [1] Preparation
	// ----
	// Create client and get Income Statement (HTML) in _HTTPSContent.txt file
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Income Statement- APPLE
    std::string server("finance.yahoo.com");
    std::string path("/quote/AAPL/cash-flow?p=AAPL");

	Networking::HTTPSClient c(io_service, ctx, server, path);
    io_service.run();
    // ----


    // [2] Get:
    // ----
	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;

	std::regex regexFreeCashFlowLine("Free Cash Flow.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");

	//
	while (std::getline(file, lineFromFile)) 
	{
		// Get Free Cash Flow
		if (std::regex_search(lineFromFile, match, regexFreeCashFlowLine))
		{
			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string freeCashFlowStr(match[i].str());
			    // Remove "," from data
			    freeCashFlowStr.erase(remove(freeCashFlowStr.begin(), freeCashFlowStr.end(), ','), freeCashFlowStr.end());

			    double num = stod(freeCashFlowStr);
			    std::cout << "Free Cash Flow: " << num << '\n';
			    // Push back in vector
		    }
		}
	}

	// Close file
	file.close();
}



TEST_F(NetworkingUnitTest, DISABLED_DCF_Test)
{
  double previous_sum = 0.0;
  double sum = 0.0;
  double FCFPS = 8.973;

  double interest_rate = 0.13;
  double num = 1 + interest_rate;


  for (int i = 1; i <= 100; ++i)
  {
    double member = FCFPS / pow(num, i);

    previous_sum = sum;
    sum = sum + member;

    if ((sum - previous_sum) < 0.05)
    {
      std::cout << "xxxx INTRINSIC VALUE: " << sum << '\n';
      // break;
    }
    std::cout << "Mem: " << member << " Sum: " << sum << " Diff: " << sum - previous_sum << '\n';
    // std::cout << "----" << '\n';
  }

   /*
  int year = 1;
  do
  {
    double member = FCFPS / pow(num, year);

    previous_sum = sum;
    sum += member;

    std::cout << "Mem: " << member << " Sum: " << sum << " Diff: " << sum - previous_sum << '\n';

    year++;
  } while ((sum - previous_sum) > 0.05);

  std::cout << "INTRINSIC VALUE: " << sum << '\n';
  */

  // assert(z != 0);
   
}


/*
TEST_F(NetworkingUnitTest, DISABLED_firstMyClassTest)
{
	// HTTPS (SSL) Clinet example
	// https://github.com/alexandruc/SimpleHttpsClient/blob/master/https_client.cpp

	// Socket programming using asio
	// https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Net Income
    // /quote/AAPL/financials?p=AAPL
    // Cash Flow
    // /quote/AAPL/cash-flow?p=AAPL

    // std::cout << "Usage: https_client <server> <path>\n";
    std::string server("finance.yahoo.com");
    // std::string path("/quote/AAPL?p=AAPL&.tsrc=fin-srch");
    // std::string path("/quote/AAPL/cash-flow?p=AAPL");
    std::string path("/quote/AAPL/financials?p=AAPL");

    // client c(io_service, ctx, server, path);
    Networking::HTTPSClient c(io_service, ctx, server, path);
    io_service.run();
}


TEST_F(NetworkingUnitTest, DISABLED_testGetRevenue)
{
	// String to parse	
	std::string lineToRegex = "undefined\" title=\"Total Revenue\"><button aria-label=\"Total Revenue\" class=\"P(0) M(0) Va(m) Bd(0) Fz(s) Mend(2px) tgglBtn\"><svg class=\"H(16px) Fill($primaryColor) Stk($primaryColor) tgglBtn:h_Fill($linkColor) tgglBtn : h_Stk($linkColor) Cur(p)\" width=\"16\" style=\"stroke - width : 0; vertical - align:bottom\" height=\"16\" viewBox=\"0 0 48 48\" data-icon=\"caret - right\"><path d=\"M33.447 24.102L20.72 11.375c - .78 - .78 - 2.048 - .78 - 2.828 0 - .78.78 - .78 2.047 0 2.828l9.9 9.9 - 9.9 9.9c - .78.78 - .78 2.047 0 2.827.78.78 2.047.78 2.828 0l12.727 - 12.728z\"></path></svg></button><span class=\"Va(m)\">Total Revenue</span></div><div class=\"W(3px) Pos(a) Start(100 % ) T(0) H(100 % ) Bg($pfColumnFakeShadowGradient) Pe(n) Pend(5px)\"></div></div><div class=\"Ta(c) Py(6px) Bxz(bb) BdB Bdc($seperatorColor) Miw(120px) Miw(100px)--pnclg Bgc($lv1BgColor) fi - row : h_Bgc($hoverBgColor) D(tbc)\" data-test=\"fin - col\"><span>394,328,000</span></div><div class=\"Ta(c) Py(6px) Bxz(bb) BdB Bdc($seperatorColor) Miw(120px) Miw(100px)--pnclg D(tbc)\" data-test=\"fin - col\"><span>394,328,000</span></div><div class=\"Ta(c) Py(6px) Bxz(bb) BdB Bdc($seperatorColor) Miw(120px) Miw(100px)--pnclg Bgc($lv1BgColor) fi - row : h_Bgc($hoverBgColor) D(tbc)\" data-test=\"fin - col\"><span>365,817,000</span></div><div class=\"Ta(c) Py(6px) Bxz(bb) BdB Bdc($seperatorColor) Miw(120px) Miw(100px)--pnclg D(tbc)\" data-test=\"fin - col\"><span>274,515,000</span></div><div class=\"Ta(c) Py(6px) Bxz(bb) BdB Bdc($seperatorColor) Miw(120px) Miw(100px)--pnclg Bgc($lv1BgColor) fi - row : h_Bgc($hoverBgColor) D(tbc)\" data-test=\"fin - col\"><span>260,174,000</span></div></div><div></div></div><div class=\"\"";

	std::smatch match;
    // std::regex regexLine("[0-9]+(,[0-9]+)*");
    std::regex regexLine("Total Revenue.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");

    std::regex_search(lineToRegex, match, regexLine);

    std::cout << "----" << '\n';

    for (size_t i = 1; i < match.size(); ++i)
    {
	    std::string revenueStr(match[i].str());
	    revenueStr.erase(remove(revenueStr.begin(), revenueStr.end(), ','), revenueStr.end());

	    double num = stod(revenueStr);
	    std::cout << "Num: " << num << '\n';
    }
}


TEST_F(NetworkingUnitTest, DISABLED_regexTest)
{
	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;
	// std::regex regexLine("\"revenue\":.\"raw\":(\\d+)");
	// std::regex regexLine("\"annualFreeCashFlow\".*?raw\":(\\d+).*?raw\":(\\d+).*?raw\":(\\d+).*?raw\":(\\d+)");
	std::regex regexLine("Net Income Common Stockholders.*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*).*?<span>([-]?[0-9]+(,[0-9]+)*)");


	if (file.is_open()) 
	{
		std::cout << "OK, File is open!" << '\n';
	}

	while (std::getline(file, lineFromFile)) 
	{
		// std::cout << "Line: " << lineFromFile.c_str() << '\n';

		// Do regex stuff
		if (std::regex_search(lineFromFile, match, regexLine))
		{
			std::cout << "Regex match!" << '\n';
			// wantedString = match.str(1); // match.str(0) - Whole matched string
			// std::cout << "match.str(0): " << match.str(0) << '\n';
			
			std::cout << "match.str(1): " << match.str(1) << '\n';
			std::cout << "match.str(2): " << match.str(2) << '\n';
			std::cout << "match.str(3): " << match.str(3) << '\n';
			std::cout << "match.str(4): " << match.str(4) << '\n';
			

			for (size_t i = 1; i < match.size(); ++i)
		    {
			    std::string revenueStr(match[i].str());
			    revenueStr.erase(remove(revenueStr.begin(), revenueStr.end(), ','), revenueStr.end());

			    double num = stod(revenueStr);
			    std::cout << "Num: " << num << '\n';
		    }
		}
	}

	// Close file
	file.close();	
}



TEST_F(NetworkingUnitTest, DISABLED_testReadHTTMLContent)
{
	// Open and read file line by line (Should be only one line)
	std::string line;
  	std::ifstream myfile("_HTTPSContent.txt");

	if (myfile.is_open())
    {
	   while (getline (myfile,line))
	   {
	   	 std::cout << "Count reading file" << '\n';
	     std::cout << line << '\n';
	   }

	   myfile.close();
	}
}


TEST_F(NetworkingUnitTest, DISABLED_testWriteInFile)
{
	std::string _fileName("TestFile.txt");

	// Create file object and open file
	std::ofstream _file0(_fileName);

	// Write to the file
	_file0 << " Files can be tricky, but it is fun enough2!\n";
	_file0 << " Next paraf!\n";
	_file0 << " Add more text in the same line\n";
	_file0 << " Third addition";

	// Close the file
	_file0.close();

	// _Open file
	_file0.open(_fileName, std::ios::app);
	_file0 << "This should go to the end of file\n";

	_file0.close();

	// Create file object and open file
	std::ofstream _file1(_fileName);
	_file1 << "First line of second pass";

	_file1.close();
}





TEST_F(NetworkingUnitTest, DISABLED_incomeStatementTest)
{	
	// 1] ==== CREATE CLIENT AND THE FILL FILE WITH HTML INFO ====

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Net Income
    // /quote/AAPL/financials?p=AAPL

    std::string server("finance.yahoo.com");
    std::string path("/quote/AAPL/financials?p=AAPL");

    // Create Temp Client
    Networking::HTTPSClient HTTPSClient(io_service, ctx, server, path);
    io_service.run();


    // 2] ==== OPEN CREATED FILE AND PARSE INFO FROM CASH FLOW STATEMENT ====

	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;

	// Revenue
	// .*?  - Capture everything until
	std::regex regexLineRevenue("\"annualTotalRevenue\".*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+)");
	std::regex regexLineConstOfRevenue("\"annualReconciledCostOfRevenue\".*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+)");
	std::regex regexLineNetIncome("\"annualNetIncomeContinuousOperations\".*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+)");

	if (file.is_open()) 
	{
		std::cout << "OK, File is open!" << '\n';
	}

	// Start parsing file
	auto start = std::chrono::steady_clock::now();

	while (std::getline(file, lineFromFile)) 
	{
		// Do regex stuff
		// Parse Revenue
		if (std::regex_search(lineFromFile, match, regexLineRevenue))
		{
			std::cout << "match.str(1): " << match.str(1) << '\n';
			std::cout << "match.str(2): " << match.str(2) << '\n';
			std::cout << "match.str(3): " << match.str(3) << '\n';
			std::cout << "match.str(4): " << match.str(4) << '\n';

			// Parse Const Of Revenue
			if (std::regex_search(lineFromFile, match, regexLineConstOfRevenue))
			{
				std::cout << "match.str(1): " << match.str(1) << '\n';
				std::cout << "match.str(2): " << match.str(2) << '\n';
				std::cout << "match.str(3): " << match.str(3) << '\n';
				std::cout << "match.str(4): " << match.str(4) << '\n';
			}

			// Parse Net Income
			if (std::regex_search(lineFromFile, match, regexLineNetIncome))
			{
				std::cout << "match.str(1): " << match.str(1) << '\n';
				std::cout << "match.str(2): " << match.str(2) << '\n';
				std::cout << "match.str(3): " << match.str(3) << '\n';
				std::cout << "match.str(4): " << match.str(4) << '\n';
			}
		}
	}

	// End parsing file measurement
	auto end = std::chrono::steady_clock::now();

	std::cout << "Elapsed time in nanoseconds: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " ns" << '\n';

    std::cout << "Elapsed time in milliseconds: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << '\n';

	// Close file
	file.close();

}


TEST_F(NetworkingUnitTest, DISABLED_balanceSheetTest)
{	
	// 1] ==== CREATE CLIENT AND THE FILL FILE WITH HTML INFO ====

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Balance Sheet
    // /quote/AAPL/balance-sheet?p=AAPL

    std::string server("finance.yahoo.com");
    std::string path("/quote/AAPL/balance-sheet?p=AAPL");

    // Create Temp Client
    Networking::HTTPSClient HTTPSClient(io_service, ctx, server, path);
    io_service.run();


    // 2] ==== OPEN CREATED FILE AND PARSE INFO FROM CASH FLOW STATEMENT ====

	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;

	// .*?  - Capture everything until
	std::regex regexLineTotalEquityGrossMinorityInterest("\"annualTotalEquityGrossMinorityInterest\".*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+)");
	std::regex regexLineTotalDebt("\"annualTotalDebt\".*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+)");
	std::regex regexLineShareIssued("\"annualShareIssued\".*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+).*?\"raw\":(\\d+)");
	
	if (file.is_open()) 
	{
		std::cout << "OK, File is open!" << '\n';
	}

	// Start parsing file measurement
	auto start = std::chrono::steady_clock::now();

	while (std::getline(file, lineFromFile)) 
	{
		// Do regex stuff
		// Get Total Debt
		if (std::regex_search(lineFromFile, match, regexLineTotalEquityGrossMinorityInterest))
		{
			std::cout << "match.str(1): " << match.str(1) << '\n';
			std::cout << "match.str(2): " << match.str(2) << '\n';
			std::cout << "match.str(3): " << match.str(3) << '\n';
			std::cout << "match.str(4): " << match.str(4) << '\n';

			// Parse Total Shares Issued
			if (std::regex_search(lineFromFile, match, regexLineTotalDebt))
			{
				std::cout << "match.str(1): " << match.str(1) << '\n';
				std::cout << "match.str(2): " << match.str(2) << '\n';
				std::cout << "match.str(3): " << match.str(3) << '\n';
				std::cout << "match.str(4): " << match.str(4) << '\n';
			}
			
			// Parse Net Income
			if (std::regex_search(lineFromFile, match, regexLineShareIssued))
			{
				std::cout << "match.str(1): " << match.str(1) << '\n';
				std::cout << "match.str(2): " << match.str(2) << '\n';
				std::cout << "match.str(3): " << match.str(3) << '\n';
				std::cout << "match.str(4): " << match.str(4) << '\n';
			}
		}
	}

	// End parsing file measurement
	auto end = std::chrono::steady_clock::now();

	std::cout << "Elapsed time in nanoseconds: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " ns" << '\n';

    std::cout << "Elapsed time in milliseconds: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << '\n';

	// Close file
	file.close();
}


TEST_F(NetworkingUnitTest, DISABLED_cashFlowStatementTest)
{	

	// 1] ==== CREATE CLIENT AND THE FILL FILE WITH HTML INFO ====

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Cash Flow
    // /quote/AAPL/cash-flow?p=AAPL
    std::string server("finance.yahoo.com");
    std::string path("/quote/AAPL/cash-flow?p=AAPL");

    // Create Temp Client
    Networking::HTTPSClient HTTPSClient(io_service, ctx, server, path);
    io_service.run();


    // 2] ==== OPEN CREATED FILE AND PARSE INFO FROM CASH FLOW STATEMENT ====

	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;

	// Free Cash Flow
	std::regex regexLineCashFlow("\"annualFreeCashFlow\".*?raw\":(\\d+).*?raw\":(\\d+).*?raw\":(\\d+).*?raw\":(\\d+)");
	std::regex regexLineStockPrice("FIN_TICKER_PRICE&quot;:&quot;(\\d*\\.?\\d+)");

	if (file.is_open()) 
	{
		std::cout << "OK, File is open!" << '\n';
	}

	// Start parsing file measurement
	auto start = std::chrono::steady_clock::now();

	while (std::getline(file, lineFromFile)) 
	{
		// Do regex stuff
		if (std::regex_search(lineFromFile, match, regexLineCashFlow))
		{
			std::cout << "Regex match!" << '\n';
			// std::cout << "match.str(0): " << match.str(0) << '\n'; // - Whole matched string
			std::cout << "match.str(1): " << match.str(1) << '\n';
			std::cout << "match.str(2): " << match.str(2) << '\n';
			std::cout << "match.str(3): " << match.str(3) << '\n';
			std::cout << "match.str(4): " << match.str(4) << '\n';

			// When we get correct line, parse also stock price in the same line
			if (std::regex_search(lineFromFile, match, regexLineStockPrice))
			{
				std::cout << "match.str(1): " << match.str(1) << '\n';
			}
		}
	}

	// End parsing file measurement
	auto end = std::chrono::steady_clock::now();

	std::cout << "Elapsed time in nanoseconds: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " ns" << '\n';

    std::cout << "Elapsed time in milliseconds: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << '\n';

	// Close file
	file.close();	   
}


TEST_F(NetworkingUnitTest, DISABLED_futureAnalysisStockTest)
{	

	// 1] ==== CREATE CLIENT AND THE FILL FILE WITH HTML INFO ====

	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

    boost::asio::io_service io_service;

    // Analysis
    // /quote/AAPL/analysis?p=AAPL
    std::string server("finance.yahoo.com");
    std::string path("/quote/AAPL/analysis?p=AAPL");

    // Create Temp Client
    Networking::HTTPSClient HTTPSClient(io_service, ctx, server, path);
    io_service.run();


    // 2] ==== OPEN CREATED FILE AND PARSE INFO FROM CASH FLOW STATEMENT ====

	std::string _fileName("_HTTPSContent.txt");

	// Test regex in file
	std::ifstream file(_fileName);

	std::string lineFromFile;
	std::smatch match;

	// Analysis
	std::regex regexLineRevenueEstimate("\"revenueEstimate\".*?raw\":(\\d+).*?\"revenueEstimate\".*?raw\":(\\d+).*?\"revenueEstimate\".*?raw\":(\\d+).*?\"revenueEstimate\".*?raw\":(\\d+)");
	std::regex regexLineEarningsEstimate("\"earningsEstimate\".*?raw\":(\\d+).*?\"earningsEstimate\".*?raw\":(\\d+).*?\"earningsEstimate\".*?raw\":(\\d+)(\\.\\d+).*?\"earningsEstimate\".*?raw\":(\\d+)(\\.\\d+)");


	if (file.is_open()) 
	{
		std::cout << "OK, File is open!" << '\n';
	}

	// Start parsing file measurement
	auto start = std::chrono::steady_clock::now();

	while (std::getline(file, lineFromFile)) 
	{
		// Do regex stuff
		if (std::regex_search(lineFromFile, match, regexLineRevenueEstimate))
		{
			// Revenue Estimate
			std::cout << "match.str(3): " << match.str(3) << '\n'; // Current year revenue
			std::cout << "match.str(4): " << match.str(4) << '\n'; // Next year revenue

			// Earnings Estimate
			if (std::regex_search(lineFromFile, match, regexLineEarningsEstimate))
			{
				// Sum Up
				std::cout << "match.str(3): " << match.str(3) << '\n'; // Current year earnings
				std::cout << "match.str(4): " << match.str(4) << '\n';
				// Sum Up
				std::cout << "match.str(5): " << match.str(5) << '\n'; // Next year earnings
				std::cout << "match.str(6): " << match.str(6) << '\n';
			}
			
		}
	}

	// End parsing file measurement
	auto end = std::chrono::steady_clock::now();

	std::cout << "Elapsed time in nanoseconds: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " ns" << '\n';

    std::cout << "Elapsed time in milliseconds: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << '\n';

	// Close file
	file.close();	   
}


// ==== HTTPS Proxy Srv Test ====
// INCOME STATEMENT TEST
TEST_F(NetworkingUnitTest, HTTPSProxySrv_NetIncome_Test)
{	
	std::cout << "HTTPSProxySrv_NetIncome_Test test ..." << '\n';
	Service::HTTPSProxySrv httpsProxySrvTemp("Test", "Test");

	std::vector<double> revenueVec;
	std::vector<double> grossProfitVec;
	std::vector<double> netIncomeVec;

	std::string stockTicker("AAPL");
	httpsProxySrvTemp.getFromIncomeStatement(stockTicker, revenueVec, grossProfitVec, netIncomeVec);

	std::cout << "Revenue:" << '\n';
	for(const auto& s : revenueVec)
	{
		std::cout << s << '\n';
	}

	std::cout << "Gross Profit:" << '\n';
	for(const auto& s : grossProfitVec)
	{
		std::cout << s << '\n';
	}

	std::cout << "Net Income:" << '\n';
	for(const auto& s : netIncomeVec)
	{
		std::cout << s << '\n';
	}
}


// FUTURE REVENUE AND EPS STATEMENT TEST
TEST_F(NetworkingUnitTest, DISABLED_HTTPSProxySrv_FutureRevenueAndEPS_Test)
{	
	std::cout << "HTTPSProxySrv_FutureRevenueAndEPS_Test test ..." << '\n';
	Service::HTTPSProxySrv httpsProxySrvTemp("Test", "Test");

	std::vector<double> futureRevenueVec;
	std::vector<double> futureEPSVec;

	std::string stockTicker("AAPL");
	httpsProxySrvTemp.getRevenueAndEPSPrediction(stockTicker, futureRevenueVec, futureEPSVec);

	std::cout << "Future Revenue:" << '\n';
	for(const auto& s : futureRevenueVec)
	{
		std::cout << s << '\n';
	}

	std::cout << "Future EPS:" << '\n';
	for(const auto& s : futureEPSVec)
	{
		std::cout << s << '\n';
	}
}


// BALANCE SHEET TEST
TEST_F(NetworkingUnitTest, DISABLED_HTTPSProxySrv_BalanceSheet_Test)
{	
	std::cout << "HTTPSProxySrv_BalanceSheet_Test test ..." << '\n';
	Service::HTTPSProxySrv httpsProxySrvTemp("Test", "Test");

	std::vector<double> bookValueVec;
	std::vector<double> totalDebtVec;
	std::vector<double> shareIssuedVec;

	std::string stockTicker("AAPL");
	httpsProxySrvTemp.getFromBalanceSheet(stockTicker, bookValueVec, totalDebtVec, shareIssuedVec);

	std::cout << "Book value:" << '\n';
	for(const auto& s : bookValueVec)
	{
		std::cout << s << '\n';
	}

	std::cout << "Total Debt:" << '\n';
	for(const auto& s : totalDebtVec)
	{
		std::cout << s << '\n';
	}

	std::cout << "Shares issued:" << '\n';
	for(const auto& s : shareIssuedVec)
	{
		std::cout << s << '\n';
	}
}


// CASH FLOW TEST
TEST_F(NetworkingUnitTest, DISABLED_HTTPSProxySrv_CashFlow_Test)
{	
	std::cout << "HTTPSProxySrv_CashFlow_Test test ..." << '\n';
	Service::HTTPSProxySrv httpsProxySrvTemp("Test", "Test");

	std::vector<double> bookValueVec;
	double sharePrice;

	std::string stockTicker("AAPL");
	httpsProxySrvTemp.getFromCashFlowStatement(stockTicker, bookValueVec, sharePrice);

	std::cout << "Free Cash Flow value:" << '\n';
	for(const auto& s : bookValueVec)
	{
		std::cout << s << '\n';
	}

	std::cout << "Share Price:" << sharePrice << '\n';
}
*/