#include <gmock/gmock.h>
#include <memory>
#include <map>
#include <regex>
#include <fstream>

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


TEST_F(NetworkingUnitTest, DISABLED_firstmyClassTest)
{
	// HTTPS (SSL) Clinet example
	// https://github.com/alexandruc/SimpleHttpsClient/blob/master/https_client.cpp

	// Socket programming using asio
	// https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T

	
	boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
	ctx.set_default_verify_paths();

	std::cout << "xx MB xx - My first test!" << '\n';

    boost::asio::io_service io_service;

    // std::cout << "Usage: https_client <server> <path>\n";
    std::string server("finance.yahoo.com");
    std::string path("/quote/AAPL?p=AAPL&.tsrc=fin-srch");

    // client c(io_service, ctx, server, path);
    Networking::HTTPSClient c(io_service, ctx, server, path);
    io_service.run();

    std::cout << " xxxx End of service " << '\n';

}


TEST_F(NetworkingUnitTest, DISABLED_readStockContent)
{
	// Open and read file line by line (Should be only one line)
	std::string line;
  	std::ifstream myfile("logfile.txt");

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
	std::string _fileName("AAPL.txt");

	// Create and open file
	std::ofstream _file(_fileName);

	// Write to the file
	_file << " Files can be tricky, but it is fun enough!";
	_file << " Next paraf!";
	_file << " Add more text in the same line ";
	_file << " Third addition";

	// Close the file
	_file.close();

	// Open and read file line by line (Should be only one line)
	std::string line;
  	std::ifstream myfile("AAPL.txt");

	if (myfile.is_open())
    {
	   while ( getline (myfile,line) )
	   {
	   	std::cout << "Count reading file" << '\n';
	     std::cout << line << '\n';
	   }
	   myfile.close();
	}

}


TEST_F(NetworkingUnitTest, regexTest)
{
	std::string fileLine0("Test TestReg Test");
	// [{"date":2018,"revenue":{"raw":265595000000,"fmt":"265.6B","longFmt":"265,595,000,000"},"earnings":{"raw":59531000000,"fmt":"59.53B","longFmt":"59,531,000,000"}},{"date":2019,"revenue":{"raw":260174000000,"fmt":"260.17B","longFmt":"260,174,000,000"},"earnings":{"raw":55256000000,"fmt":"55.26B","longFmt":"55,256,000,000"}},{"date":2020,"revenue":{"raw":274515000000,"fmt":"274.51B","longFmt":"274,515,000,000"},"earnings":{"raw":57411000000,"fmt":"57.41B","longFmt":"57,411,000,000"}},{"date":2021,"revenue
	// std::string fileLine1("Test Test Test");

	std::regex regexLine("TestReg");

	std::smatch match;
	// std::string wantedString;

	/*
	if (std::regex_search(fileLine0, match, regexLine))
	{
		std::cout << "Regex match!" << '\n';
		// wantedString = match.str(1); // match.str(0) - Whole matched string
		std::cout << "match.str(0): " << match.str(0) << '\n';
		std::cout << "match.str(1): " << match.str(1) << '\n';
	}
	*/

	// TO TEST
	// Regex
	// {"date":(\d+),"revenue":{"raw":(\d+)
	// std::regex regexLine1(" date\\":(\\d+),\\"revenue\\":{\\"raw\\":(\\d+) " );

	// std::string fileLine2("Test \"TestReg\" Test");
	std::string fileLine2("\"revenue\":{\"raw\":265595000000");
	std::regex regexLine2("\"revenue\":.\"raw\":(\\d+)");

	
	if (std::regex_search(fileLine2, match, regexLine2))
	{
		std::cout << "Regex match!" << '\n';
		// wantedString = match.str(1); // match.str(0) - Whole matched string
		std::cout << "match.str(0): " << match.str(0) << '\n';
		std::cout << "match.str(1): " << match.str(1) << '\n';
	}

	// Test regex in file
	std::ifstream file("test.txt");
	std::string lineFromFile;

	if (file.is_open()) 
	{
		std::cout << "OK, File is open!" << '\n';
	}

	while (std::getline(file, lineFromFile)) 
	{
		std::cout << lineFromFile.c_str() << '\n';

		// Do regex stuff
	}


	// Close file
	file.close();	
}