#include <iostream>

#include "FactoryIf.h"
#include "Factory.h"
// #include "FactoryImpl.h"

//#include "ServiceIf.h"
//#include "Service0.h"

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

int main()
{
	// Create Factory
	Common::Factory::getInstance();

	// Move to Unit Test
	Service::Service0 srv0_0("dbPath", "instName");
	
	// Test Factroy
	srv0_0.testFactory();

	// Set FactroyImpl
	srv0_0.setFactoryImplPtr(Common::FactoryIf::getInstance());

	// Test FactoryImpl
	srv0_0.testFactoryImpl();

	// ---- NETWORKING ----
	asio::error_code ec;

	// Create a "context" - The platform specific interface
	asio::io_context context;

	// Get address of somewhere we wish to connect to
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);

	// Create a socket, the context will deliver the implementation
	asio::ip::tcp::socket socket(context);

	// Tell socket to try and connect
	socket.connect(endpoint, ec);

	if(!ec)
	{
		std::cout << "Connected!" << '\n';
	} 
	else 
	{
		std::cout << "Failed to connect to address:\n" << ec.message() << '\n';
	}

	if(socket.is_open())
	{
		std::string sRequest = 
		"GET /index.html HTTP/1.1\r\n"
		"Host: example.com\r\n"
		"Connection: close\r\n\r\n";

		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

		// This is bad - just wait for response for example purpose
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(200ms);

		size_t bytes = socket.available();
		std::cout << "Bytes Available: " << bytes << '\n';

		if(bytes > 0)
		{
			std::vector<char> vBuffer(bytes);
			socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);

			for(auto c : vBuffer)
			{
				std::cout << c;
			}
		}
	}



	return 0;
}